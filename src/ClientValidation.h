#pragma once

namespace Validation {

enum class Status {
    Ok,
    NotTrusted,
    InvalidSubjectName,
    UnkownError,
};

// Checks if the current client executable has a valid digital signature with IOI as the
// subject. Return of Status::Ok indicates a successful signature validation.
Status isValidClient();

} // namespace Validation