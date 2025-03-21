// src/Observer/ProcessingObserver.cpp
#include "../../include/DIPAL/Observer/ProcessingObserver.hpp"

#include <iomanip>
#include <iostream>

namespace DIPAL {

void ConsoleObserver::onProcessingStarted(const std::string& operationName) {
    std::cout << "Started: " << operationName << std::endl;
}

void ConsoleObserver::onProgressUpdated(float progress) {
    int barWidth = 50;
    int pos = static_cast<int>(barWidth * progress);

    std::cout << "[";
    for (int i = 0; i < barWidth; ++i) {
        if (i < pos)
            std::cout << "=";
        else if (i == pos)
            std::cout << ">";
        else
            std::cout << " ";
    }

    std::cout << "] " << std::fixed << std::setprecision(1) << (progress * 100.0) << "%\r";
    std::cout.flush();

    if (progress >= 1.0f) {
        std::cout << std::endl;
    }
}

void ConsoleObserver::onProcessingCompleted(const std::string& operationName, bool success) {
    std::cout << (success ? "Completed: " : "Failed: ") << operationName << std::endl;
}

void ConsoleObserver::onError(const std::string& errorMessage) {
    std::cerr << "Error: " << errorMessage << std::endl;
}

}  // namespace DIPAL
