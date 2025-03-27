// include/DIPAL/Utils/Profiler.hpp
#ifndef DIPAL_PROFILER_HPP
#define DIPAL_PROFILER_HPP

#include <string>
#include <string_view>
#include <chrono>
#include <unordered_map>
#include <vector>
#include <mutex>
#include <functional>
#include <memory>
#include <format>
#include "Logger.hpp"

namespace DIPAL {

/**
 * @brief Performance profiler for the DIPAL library
 * 
 * Provides timing measurement for operations to help identify bottlenecks
 */
class Profiler {
public:
    /**
     * @brief Timing record for a single operation
     */
    struct TimingRecord {
        std::string name;
        std::chrono::microseconds duration;
        std::chrono::system_clock::time_point timestamp;
    };
    
    /**
     * @brief Summary statistics for an operation
     */
    struct TimingSummary {
        std::string name;
        std::chrono::microseconds totalDuration;
        std::chrono::microseconds minDuration;
        std::chrono::microseconds maxDuration;
        std::chrono::microseconds avgDuration;
        size_t callCount;
    };
    
    /**
     * @brief RAII scope timer that records duration on destruction
     */
    class ScopeTimer {
    public:
        /**
         * @brief Create a scope timer
         * @param name Operation name
         * @param profiler Parent profiler
         */
        ScopeTimer(std::string_view name, Profiler& profiler)
            : m_name(name), m_profiler(profiler), m_start(std::chrono::high_resolution_clock::now()) {}
        
        /**
         * @brief Destructor - records timing
         */
        ~ScopeTimer() {
            auto end = std::chrono::high_resolution_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - m_start);
            m_profiler.recordTiming(m_name, duration);
        }
        
    private:
        std::string m_name;
        Profiler& m_profiler;
        std::chrono::high_resolution_clock::time_point m_start;
    };
    
    /**
     * @brief Get the profiler instance
     * @return Reference to the profiler singleton
     */
    static Profiler& getInstance();
    
    /**
     * @brief Enable or disable profiling
     * @param enabled True to enable, false to disable
     */
    void setEnabled(bool enabled);
    
    /**
     * @brief Check if profiling is enabled
     * @return True if enabled, false otherwise
     */
    [[nodiscard]] bool isEnabled() const;
    
    /**
     * @brief Set a callback function to receive timing records
     * @param callback Function to call for each timing record
     */
    void setCallback(std::function<void(const TimingRecord&)> callback);
    
    /**
     * @brief Record a timing measurement
     * @param name Operation name
     * @param duration Operation duration
     */
    void recordTiming(std::string_view name, std::chrono::microseconds duration);
    
    /**
     * @brief Create a scope timer for RAII timing
     * @param name Operation name
     * @return Unique pointer to a scope timer
     */
    [[nodiscard]] std::unique_ptr<ScopeTimer> createTimer(std::string_view name);
    
    /**
     * @brief Get timing statistics for all operations
     * @return Vector of timing summaries
     */
    [[nodiscard]] std::vector<TimingSummary> getTimingSummaries() const;
    
    /**
     * @brief Get timing statistics for a specific operation
     * @param name Operation name
     * @return Timing summary
     */
    [[nodiscard]] TimingSummary getTimingSummary(std::string_view name) const;
    
    /**
     * @brief Reset all timing statistics
     */
    void reset();
    
    /**
     * @brief Log timing summaries at the specified log level
     * @param level Log level
     */
    void logSummaries(LogLevel level = LogLevel::Info);
    
private:
    // Private constructor for singleton
    Profiler();
    
    // Prevent copying and moving
    Profiler(const Profiler&) = delete;
    Profiler& operator=(const Profiler&) = delete;
    Profiler(Profiler&&) = delete;
    Profiler& operator=(Profiler&&) = delete;
    
    bool m_enabled;
    std::function<void(const TimingRecord&)> m_callback;
    std::unordered_map<std::string, std::vector<std::chrono::microseconds>> m_timings;
    mutable std::mutex m_mutex;
};

// Macro to create a scoped timer with the current function name
#define DIPAL_PROFILE_FUNCTION() \
    std::unique_ptr<DIPAL::Profiler::ScopeTimer> _profiler_timer; \
    if (DIPAL::Profiler::getInstance().isEnabled()) { \
        _profiler_timer = DIPAL::Profiler::getInstance().createTimer(__func__); \
    }

// Macro to create a named scoped timer
#define DIPAL_PROFILE_SCOPE(name) \
    std::unique_ptr<DIPAL::Profiler::ScopeTimer> _profiler_timer_##__LINE__; \
    if (DIPAL::Profiler::getInstance().isEnabled()) { \
        _profiler_timer_##__LINE__ = DIPAL::Profiler::getInstance().createTimer(name); \
    }

} // namespace DIPAL

#endif // DIPAL_PROFILER_HPP
