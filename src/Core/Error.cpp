// src/Core/Error.cpp
#include "../../include/DIPAL/Core/Error.hpp"

namespace DIPAL {

std::string_view Error::getDefaultMessage(ErrorCode code) {
    switch (code) {
        case ErrorCode::Success:
            return "Operation successful";

        case ErrorCode::Unknown:
            return "Unknown error";

        case ErrorCode::FileNotFound:
            return "File not found";

        case ErrorCode::FileAccessDenied:
            return "Access denied to file";

        case ErrorCode::InvalidFormat:
            return "Invalid file format";

        case ErrorCode::AllocationFailed:
            return "Memory allocation failed";

        case ErrorCode::InvalidAccess:
            return "Invalid memory access";

        case ErrorCode::InvalidParameter:
            return "Invalid parameter";

        case ErrorCode::OutOfRange:
            return "Value out of range";

        case ErrorCode::UnsupportedFormat:
            return "Unsupported format";

        case ErrorCode::CorruptedData:
            return "Data is corrupted";

        case ErrorCode::ProcessingFailed:
            return "Processing operation failed";

        case ErrorCode::NotImplemented:
            return "Feature not implemented";

        case ErrorCode::InternalError:
            return "Internal error";

        default:
            return "Unspecified error";
    }
}

}  // namespace DIPAL
