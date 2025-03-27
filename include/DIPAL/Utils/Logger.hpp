#pragma once

#include <iostream>
#include <string>
#include <format>

namespace DIPAL {

enum class LogLevel { 
    Trace, 
    Debug, 
    Info, 
    Warning, 
    Error, 
    Fatal 
};

class Logger {
public:
    // Singleton access
    static Logger& getInstance();

    // Basic logging methods
    void log(LogLevel level, const std::string& message);
    
    // Formatted logging method
    template<typename... Args>
    void logFormat(LogLevel level, const std::string& format, Args... args) {
        try {
            std::string message = std::format(format, args...);
            log(level, message);
        } catch (const std::exception& e) {
            log(LogLevel::Error, "Logging error: " + std::string(e.what()));
        }
    }

    // Convenience methods for each log level
    void trace(const std::string& message);
    void debug(const std::string& message);
    void info(const std::string& message);
    void warning(const std::string& message);
    void error(const std::string& message);
    void fatal(const std::string& message);

    // Set current log level
    void setLogLevel(LogLevel level);

private:
    // Private constructor for singleton
    Logger();

    // Convert log level to string
    static std::string_view levelToString(LogLevel level);

    // Current log level
    LogLevel m_currentLevel;
};

// Macro definitions for convenient logging
#define DIPAL_LOG_TRACE(message) DIPAL::Logger::getInstance().trace(message)
#define DIPAL_LOG_DEBUG(message) DIPAL::Logger::getInstance().debug(message)
#define DIPAL_LOG_INFO(message) DIPAL::Logger::getInstance().info(message)
#define DIPAL_LOG_WARNING(message) DIPAL::Logger::getInstance().warning(message)
#define DIPAL_LOG_ERROR(message) DIPAL::Logger::getInstance().error(message)

#define DIPAL_LOG_TRACE_FORMAT(format, ...) DIPAL::Logger::getInstance().logFormat(DIPAL::LogLevel::Trace, format, ##__VA_ARGS__)
#define DIPAL_LOG_DEBUG_FORMAT(format, ...) DIPAL::Logger::getInstance().logFormat(DIPAL::LogLevel::Debug, format, ##__VA_ARGS__)
#define DIPAL_LOG_INFO_FORMAT(format, ...) DIPAL::Logger::getInstance().logFormat(DIPAL::LogLevel::Info, format, ##__VA_ARGS__)
#define DIPAL_LOG_WARNING_FORMAT(format, ...) DIPAL::Logger::getInstance().logFormat(DIPAL::LogLevel::Warning, format, ##__VA_ARGS__)
#define DIPAL_LOG_ERROR_FORMAT(format, ...) DIPAL::Logger::getInstance().logFormat(DIPAL::LogLevel::Error, format, ##__VA_ARGS__)

} // namespace DIPAL
