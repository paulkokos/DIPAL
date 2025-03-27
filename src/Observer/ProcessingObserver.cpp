// src/Observer/ProcessingObserver.cpp
#include "../../include/DIPAL/Observer/ProcessingObserver.hpp"

#include <iomanip>
#include <iostream>
#include <format>

namespace DIPAL {

void ConsoleObserver::onProcessingStarted(std::string_view operationName) {
    std::cout << std::format("Started: {}\n", operationName);
}

void ConsoleObserver::onProgressUpdated(float progress) {
    int barWidth = 50;
    int pos = static_cast<int>(barWidth * progress);

    std::cout << "[";
    for (int i = 0; i < barWidth; ++i) {
        if (i < pos) {
            std::cout << "=";
        } else if (i == pos) {
            std::cout << ">";
        } else {
            std::cout << " ";
        }
    }

    std::cout << std::format("] {:.1f}%\r", progress * 100.0f);
    std::cout.flush();

    if (progress >= 1.0f) {
        std::cout << std::endl;
    }
}

void ConsoleObserver::onProcessingCompleted(std::string_view operationName, bool success) {
    std::cout << std::format("{}: {}\n", 
                          success ? "Completed" : "Failed", 
                          operationName);
}

void ConsoleObserver::onError(std::string_view errorMessage) {
    std::cerr << std::format("Error: {}\n", errorMessage);
}

} // namespace DIPAL
