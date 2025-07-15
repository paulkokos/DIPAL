// tests/utils/mock_observers.hpp
// Mock observer implementations for testing

#pragma once

#include <DIPAL/DIPAL.hpp>
#include <vector>
#include <string>

namespace DIPAL {
namespace MockObservers {

enum class EventType {
    ProcessingStarted,
    ProcessingCompleted,
    ProgressUpdated,
    Error
};

struct ObserverEvent {
    EventType type;
    std::string operationName;
    float progress;
    std::string message;
};

class MockProcessingObserver : public ProcessingObserver {
public:
    explicit MockProcessingObserver(bool verbose = false) : verbose(verbose) {}
    
    void onProcessingStarted(const std::string& operationName) override;
    void onProcessingCompleted(const std::string& operationName, bool success) override;
    void onProgressUpdated(float progress) override;
    void onError(const std::string& errorMessage) override;
    
    // Test helpers
    void clear() { events.clear(); }
    size_t getEventCount() const { return events.size(); }
    const std::vector<ObserverEvent>& getEvents() const { return events; }
    
    bool hasEvent(EventType type) const {
        for (const auto& event : events) {
            if (event.type == type) return true;
        }
        return false;
    }
    
    size_t countEvents(EventType type) const {
        size_t count = 0;
        for (const auto& event : events) {
            if (event.type == type) count++;
        }
        return count;
    }
    
private:
    std::vector<ObserverEvent> events;
    bool verbose;
};

} // namespace MockObservers
} // namespace DIPAL
