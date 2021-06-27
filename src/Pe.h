#pragma once
#include <memory>
#include <optional>
#include <string >

namespace PE {

enum class CertNameTypes : int {
    EMAIL_TYPE = 1,
    RDN_TYPE = 2,
    ATTR_TYPE = 3,
    SIMPLE_DISPLAY_TYPE = 4,
    FRIENDLY_DISPLAY_TYPE = 5,
    DNS_TYPE = 6,
    URL_TYPE = 7,
    UPN_TYPE = 8,
};

class SubjectCertificate {
public:
    explicit SubjectCertificate(const wchar_t* path);
    ~SubjectCertificate();

    std::optional<std::string> getString(CertNameTypes type, int flag) const;

private:
    class PeSubjectCertImpl;
    std::unique_ptr<PeSubjectCertImpl> impl;
};

bool verifySignature(const wchar_t* path);

int getTimestamp();

} // namespace PE
