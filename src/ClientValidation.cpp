#include "ClientValidation.h"
#include "Pe.h"
#include "Process.h"

Validation::Status Validation::isValidClient() {
    auto moduleName = Process::getModuleFilename();
    try {
        if(!PE::verifySignature(moduleName.c_str()))
            return Status::NotTrusted;

        PE::SubjectCertificate cert(moduleName.c_str());
        const auto subjName = cert.getString(PE::CertNameTypes::SIMPLE_DISPLAY_TYPE, 0);
        if(subjName != "IO INTERACTIVE A/S")
            Status::InvalidSubjectName;
    } catch(...) {
        return Status::UnkownError;
    }
    return Status::Ok;
}
