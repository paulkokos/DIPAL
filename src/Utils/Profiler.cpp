// src/Utils/Profiler.cpp
#include "../../include/DIPAL/Utils/Profiler.hpp"

#include <algorithm>
#include <numeric>

namespace DIPAL {

Profiler::Profiler() : m_enabled(false) {}

Profiler& Profiler::getInstance() {
    static Profiler instance;
    return instance;
}

void Profiler::setEnabled(bool enabled) {
    std::lock_guard<std::mutex> lock(m_mutex);
    m_enabled = enabled;
}

bool Profiler::isEnabled() const {
    return m_enabled;
}

void Profiler::setCallback(std::function<void(const TimingRecord&)> callback) {
    std::lock_guard<std::mutex> lock(m_mutex);
    m_callback = callback;
}

void Profiler::recordTiming(std::string_view name, std::chrono::microseconds duration) {
    if (!m_enabled) {
        return;
    }
    
    std::lock_guard<std::mutex> lock(m_mutex);
    
    m_timings[std::string(name)].push_back(duration);
    
    if (m_callback) {
        TimingRecord record{
            std::string(name),
            duration,
            std::chrono::system_clock::now()
        };
        m_callback(record);
    }
}

std::unique_ptr<Profiler::ScopeTimer> Profiler::createTimer(std::string_view name) {
    if (!m_enabled) {
        return nullptr;
    }
    
    return std::make_unique<ScopeTimer>(name, *this);
}

std::vector<Profiler::TimingSummary> Profiler::getTimingSummaries() const {
    std::lock_guard<std::mutex> lock(m_mutex);
    
    std::vector<TimingSummary> summaries;
    summaries.reserve(m_timings.size());
    
    for (const auto& [name, durations] : m_timings) {
        if (durations.empty()) {
            continue;
        }
        
        auto minDuration = *std::min_element(durations.begin(), durations.end());
        auto maxDuration = *std::max_element(durations.begin(), durations.end());
        auto totalDuration = std::accumulate(durations.begin(), durations.end(), 
                                          std::chrono::microseconds(0));
        auto avgDuration = totalDuration / durations.size();
        
        summaries.push_back({
            name,
            totalDuration,
            minDuration,
            maxDuration,
            avgDuration,
            durations.size()
        });
    }
    
    // Sort by total duration (descending)
    std::sort(summaries.begin(), summaries.end(), 
             [](const TimingSummary& a, const TimingSummary& b) {
                 return a.totalDuration > b.totalDuration;
             });
    
    return summaries;
}

Profiler::TimingSummary Profiler::getTimingSummary(std::string_view name) const {
    std::lock_guard<std::mutex> lock(m_mutex);
    
    std::string nameStr(name);
    if (m_timings.count(nameStr) == 0 || m_timings.at(nameStr).empty()) {
        return {std::string(name), {}, {}, {}, {}, 0};
    }
    
    const auto& durations = m_timings.at(nameStr);
    auto minDuration = *std::min_element(durations.begin(), durations.end());
    auto maxDuration = *std::max_element(durations.begin(), durations.end());
    auto totalDuration = std::accumulate(durations.begin(), durations.end(), 
                                      std::chrono::microseconds(0));
    auto avgDuration = totalDuration / durations.size();
    
    return {
        std::string(name),
        totalDuration,
        minDuration,
        maxDuration,
        avgDuration,
        durations.size()
    };
}

void Profiler::reset() {
    std::lock_guard<std::mutex> lock(m_mutex);
    m_timings.clear();
}


void Profiler::logSummaries(LogLevel level) {
    if (!m_enabled) {
        return;
    }

    auto summaries = getTimingSummaries();
    if (summaries.empty()) {
        Logger::getInstance().log(level, "No timing data available");
        return;
    }

    Logger::getInstance().log(level, "===== Profiling Summary =====");
    Logger::getInstance().log(level, std::format("{:<30} {:>10} {:>10} {:>10} {:>10} {:>8}",
                                       "Operation", "Total(ms)", "Avg(ms)", "Min(ms)", "Max(ms)", "Count"));
    Logger::getInstance().log(level, std::string(80, '-'));

    for (const auto& summary : summaries) {
        double totalMs = summary.totalDuration.count() / 1000.0;
        double avgMs = summary.avgDuration.count() / 1000.0;
        double minMs = summary.minDuration.count() / 1000.0;
        double maxMs = summary.maxDuration.count() / 1000.0;

        Logger::getInstance().log(level, std::format("{:<30} {:10.2f} {:10.2f} {:10.2f} {:10.2f} {:8}",
                                           summary.name, totalMs, avgMs, minMs, maxMs, summary.callCount));
    }

    Logger::getInstance().log(level, "=============================");
}

} // namespace DIPAL
