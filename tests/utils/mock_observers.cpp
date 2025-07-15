// tests/utils/mock_observers.cpp
// Mock observer implementations for testing

#include "mock_observers.hpp"

#include <iostream>
#include <string_view>
#include <vector>

namespace DIPAL {
namespace MockObservers {

void MockProcessingObserver::onProcessingStarted(const std::string_view operationName) {
    events.push_back({EventType::ProcessingStarted, std::string(operationName), 0.0f, ""});
    if (verbose) {
        std::cout << "[MOCK] Processing started: " << operationName << std::endl;
    }
}

void MockProcessingObserver::onProcessingCompleted(const std::string_view operationName,
                                                   bool success) {
    events.push_back(
        {EventType::ProcessingCompleted, std::string(operationName), success ? 1.0f : 0.0f, ""});
    if (verbose) {
        std::cout << "[MOCK] Processing completed: " << operationName
                  << " (success: " << (success ? "true" : "false") << ")" << std::endl;
    }
}

void MockProcessingObserver::onProgressUpdated(float progress) {
    events.push_back({EventType::ProgressUpdated, "", progress, ""});
    if (verbose) {
        std::cout << "[MOCK] Progress updated: " << (progress * 100) << "%" << std::endl;
    }
}

void MockProcessingObserver::onError(const std::string_view errorMessage) {
    events.push_back({EventType::Error, "", 0.0f, std::string(errorMessage)});
    if (verbose) {
        std::cout << "[MOCK] Error: " << errorMessage << std::endl;
    }
}

}  // namespace MockObservers
}  // namespace DIPAL
