#include "../../include/DIPAL/Utils/Logger.hpp"

namespace DIPAL {

Logger& Logger::getInstance() {
    static Logger instance;
    return instance;
}

Logger::Logger() : m_currentLevel(LogLevel::Info) {}

void Logger::log(LogLevel level, const std::string& message) {
    if (level < m_currentLevel) return;

    // Basic console output
    std::cout << levelToString(level) << ": " << message << std::endl;
}

void Logger::setLogLevel(LogLevel level) {
    m_currentLevel = level;
}

void Logger::trace(const std::string& message) { 
    log(LogLevel::Trace, message); 
}

void Logger::debug(const std::string& message) { 
    log(LogLevel::Debug, message); 
}

void Logger::info(const std::string& message) { 
    log(LogLevel::Info, message); 
}

void Logger::warning(const std::string& message) { 
    log(LogLevel::Warning, message); 
}

void Logger::error(const std::string& message) { 
    log(LogLevel::Error, message); 
}

void Logger::fatal(const std::string& message) { 
    log(LogLevel::Fatal, message); 
}

std::string_view Logger::levelToString(LogLevel level) {
    switch (level) {
        case LogLevel::Trace: return "TRACE";
        case LogLevel::Debug: return "DEBUG";
        case LogLevel::Info: return "INFO";
        case LogLevel::Warning: return "WARNING";
        case LogLevel::Error: return "ERROR";
        case LogLevel::Fatal: return "FATAL";
        default: return "UNKNOWN";
    }
}

} // namespace DIPAL
