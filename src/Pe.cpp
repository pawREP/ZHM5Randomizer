#pragma once
#include "Pe.h"
#include "Process.h"
#include <memory>
#include <optional>
// clang-format off
#include <windows.h>
#include <Softpub.h>
#include <wincrypt.h>
#include <wintrust.h>
#include <stdexcept>
// clang-format on

using namespace PE;

class SubjectCertificate::PeSubjectCertImpl {
public:
    explicit PeSubjectCertImpl(const wchar_t* path);
    PeSubjectCertImpl(const PeSubjectCertImpl&) = delete;
    PeSubjectCertImpl(PeSubjectCertImpl&&) noexcept = delete;
    PeSubjectCertImpl& operator=(const PeSubjectCertImpl&) = delete;
    PeSubjectCertImpl& operator=(PeSubjectCertImpl&&) noexcept = delete;
    ~PeSubjectCertImpl();

    std::optional<std::string> getString(CertNameTypes type, int flag);

private:
    PCCERT_CONTEXT cert;

    PCMSG_SIGNER_INFO getSignerInfo(HCRYPTMSG hMsg);
};

SubjectCertificate::PeSubjectCertImpl::PeSubjectCertImpl(const wchar_t* path) {
    HCERTSTORE hStore = nullptr;
    HCRYPTMSG hMsg = nullptr;

    // Get message handle and store handle from the signed file.
    auto result =
    CryptQueryObject(CERT_QUERY_OBJECT_FILE, path, CERT_QUERY_CONTENT_FLAG_PKCS7_SIGNED_EMBED,
                     CERT_QUERY_FORMAT_FLAG_BINARY, 0, nullptr, nullptr, nullptr, &hStore, &hMsg, nullptr);
    if(!result)
        throw std::runtime_error("Failed to optain embedded signature");

    PCMSG_SIGNER_INFO pSignerInfo = getSignerInfo(hMsg);
    if(!pSignerInfo) {
        CertCloseStore(hStore, 0);
        CryptMsgClose(hMsg);
        throw std::runtime_error("Failed to optain signer info");
    }

    // Search for the signer certificate in the temporary
    // certificate store.
    CERT_INFO CertInfo;
    CertInfo.Issuer = pSignerInfo->Issuer;
    CertInfo.SerialNumber = pSignerInfo->SerialNumber;

    cert = CertFindCertificateInStore(hStore, X509_ASN_ENCODING | PKCS_7_ASN_ENCODING, 0,
                                      CERT_FIND_SUBJECT_CERT, (PVOID)&CertInfo, nullptr);
    if(!cert) {
        CertCloseStore(hStore, 0);
        CryptMsgClose(hMsg);
        LocalFree(pSignerInfo);
        throw std::runtime_error("Failed to find certificate in store");
    }

    CertCloseStore(hStore, 0);
    CryptMsgClose(hMsg);
    LocalFree(pSignerInfo);
}

SubjectCertificate::PeSubjectCertImpl::~PeSubjectCertImpl() {
    CertFreeCertificateContext(cert);
}

std::optional<std::string> SubjectCertificate::PeSubjectCertImpl::getString(CertNameTypes type, int flag) {
    DWORD dwData = 0;
    LPSTR pszTempName = NULL;

    dwData = CertGetNameStringA(cert, (DWORD)type, flag, NULL, NULL, 0);
    if(!dwData)
        return std::optional<std::string>();

    auto tempName = std::make_unique<char[]>(dwData * sizeof(CHAR));
    if(!tempName)
        return std::optional<std::string>();

    dwData = CertGetNameStringA(cert, (DWORD)type, flag, NULL, tempName.get(), dwData);
    if(!dwData)
        return std::optional<std::string>();

    return std::optional<std::string>(tempName.get());
}

// Returns signer info. This function transfers ownership of the return value to the caller.
// Return value has to be freed with LocalFree if non zero.
PCMSG_SIGNER_INFO SubjectCertificate::PeSubjectCertImpl::getSignerInfo(HCRYPTMSG hMsg) {
    // Get signer information size.
    DWORD signerInfoSize;
    auto result = CryptMsgGetParam(hMsg, CMSG_SIGNER_INFO_PARAM, 0, nullptr, &signerInfoSize);
    if(!result)
        return nullptr;

    // Allocate memory for signer information.
    PCMSG_SIGNER_INFO pSignerInfo = (PCMSG_SIGNER_INFO)LocalAlloc(LPTR, signerInfoSize);

    if(!pSignerInfo)
        return nullptr;

    // Get Signer Information.
    result = CryptMsgGetParam(hMsg, CMSG_SIGNER_INFO_PARAM, 0, (PVOID)pSignerInfo, &signerInfoSize);
    if(!result) {
        LocalFree(pSignerInfo);
        return nullptr;
    }

    return pSignerInfo;
}

SubjectCertificate::SubjectCertificate(const wchar_t* path) {
    impl = std::make_unique<PeSubjectCertImpl>(path);
}

std::optional<std::string> SubjectCertificate::getString(CertNameTypes type, int flag) const {
    return impl->getString(type, flag);
}

SubjectCertificate::~SubjectCertificate() = default;

bool PE::verifySignature(const wchar_t* path) {

    WINTRUST_FILE_INFO FileData{};
    FileData.cbStruct = sizeof(WINTRUST_FILE_INFO);
    FileData.pcwszFilePath = path;
    FileData.hFile = nullptr;
    FileData.pgKnownSubject = nullptr;

    WINTRUST_DATA WinTrustData{};
    WinTrustData.cbStruct = sizeof(WinTrustData);
    WinTrustData.pPolicyCallbackData = nullptr;
    WinTrustData.pSIPClientData = nullptr;
    WinTrustData.dwUIChoice = WTD_UI_NONE;
    WinTrustData.fdwRevocationChecks = WTD_REVOKE_NONE;
    WinTrustData.dwUnionChoice = WTD_CHOICE_FILE;
    WinTrustData.dwStateAction = WTD_STATEACTION_VERIFY;
    WinTrustData.hWVTStateData = nullptr;
    WinTrustData.pwszURLReference = nullptr;
    WinTrustData.dwUIContext = 0;
    WinTrustData.pFile = &FileData;
    WinTrustData.dwProvFlags = WTD_CACHE_ONLY_URL_RETRIEVAL;

    GUID WVTPolicyGUID = WINTRUST_ACTION_GENERIC_VERIFY_V2;
    auto status = WinVerifyTrust(nullptr, &WVTPolicyGUID, &WinTrustData);

    WinTrustData.dwStateAction = WTD_STATEACTION_CLOSE;
    WinVerifyTrust(nullptr, &WVTPolicyGUID, &WinTrustData);

    return status == ERROR_SUCCESS;
}

int PE::getTimestamp() {
    void* image_base = GetModuleHandle(NULL);
    auto dos_header = reinterpret_cast<IMAGE_DOS_HEADER*>(image_base);
    auto nt_header = reinterpret_cast<IMAGE_NT_HEADERS*>((uintptr_t)image_base + dos_header->e_lfanew);
    return nt_header->FileHeader.TimeDateStamp;
}
