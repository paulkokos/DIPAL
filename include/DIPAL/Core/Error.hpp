// include/DIPAL/Color/Error.hpp
#ifndef DIPAL_ERROR_HPP
#define DIPAL_ERROR_HPP

#include <string>
#include <string_view>
#include <expected>
#include <system_error>
#include <format>

namespace DIPAL {

// Define error categories
enum class ErrorCategory {
    None,
    IO,         // Input/output errors
    Memory,     // Memory allocation/access errors
    Parameter,  // Invalid parameter errors
    Format,     // Image format errors
    Processing, // Processing errors
    Internal    // Internal library errors
};

// Define error codes
enum class ErrorCode {
    // General errors
    Success = 0,
    Unknown,
    
    // IO errors
    FileNotFound,
    FileAccessDenied,
    InvalidFormat,
    
    // Memory errors
    AllocationFailed,
    InvalidAccess,
    
    // Parameter errors
    InvalidParameter,
    OutOfRange,
    
    // Format errors
    UnsupportedFormat,
    CorruptedData,
    
    // Processing errors
    ProcessingFailed,
    
    // Internal errors
    NotImplemented,
    InternalError
};

// Error class for the library
class Error {
public:
    // Create an error with a code
    explicit Error(ErrorCode code, ErrorCategory category = ErrorCategory::None) 
        : m_code(code), m_category(category) {}
    
    // Create an error with a code and message
    Error(ErrorCode code, std::string_view message, ErrorCategory category = ErrorCategory::None) 
        : m_code(code), m_message(message), m_category(category) {}
    
    // Get the error code
    [[nodiscard]] ErrorCode code() const noexcept { return m_code; }
    
    // Get the error category
    [[nodiscard]] ErrorCategory category() const noexcept { return m_category; }
    
    // Get the error message
    [[nodiscard]] std::string_view message() const noexcept { return m_message; }
    
    // Get a formatted error string
    [[nodiscard]] std::string toString() const {
        return std::format("Error [{}:{}]: {}", 
            static_cast<int>(m_category), 
            static_cast<int>(m_code), 
            m_message.empty() ? getDefaultMessage(m_code) : m_message);
    }
    
    // Implicit conversion to string
    operator std::string() const { return toString(); }
    
private:
    ErrorCode m_code;
    std::string m_message;
    ErrorCategory m_category;
    
    // Get default message for an error code
    static std::string_view getDefaultMessage(ErrorCode code);
};

// Helper template for std::expected with Error
template <typename T>
using Result = std::expected<T, Error>;

// Helper for void results
using VoidResult = std::expected<void, Error>;

// Helper function to create an error result
template <typename T>
inline Result<T> makeErrorResult(ErrorCode code, std::string_view message = {}, 
                                 ErrorCategory category = ErrorCategory::None) {
    return std::unexpected(Error(code, message, category));
}

// Helper function to create a void error result
inline VoidResult makeVoidErrorResult(ErrorCode code, std::string_view message = {}, 
                                      ErrorCategory category = ErrorCategory::None) {
    return std::unexpected(Error(code, message, category));
}

// Helper function to create a success result
template <typename T>
inline Result<T> makeSuccessResult(T value) {
    return Result<T>(std::move(value));
}

// Helper function to create a void success result
inline VoidResult makeVoidSuccessResult() {
    return VoidResult();
}

} // namespace DIPAL



#endif  // DIPAL_ERROR_HPP 
