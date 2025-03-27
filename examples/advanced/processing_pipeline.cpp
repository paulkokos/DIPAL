// examples/advanced/processing_pipeline.cpp
#include <DIPAL/DIPAL.hpp>
#include <DIPAL/Filters/GaussianBlurFilter.hpp>
#include <DIPAL/ImageProcessor/FilterCommand.hpp>
#include <DIPAL/ImageProcessor/ImageProcessor.hpp>
#include <DIPAL/Observer/ProcessingObserver.hpp>
#include <cmath>
#include <iostream>
#include <memory>
#include <vector>

using namespace DIPAL;

// Custom progress observer class
class CustomProgressObserver : public DIPAL::ProcessingObserver {
public:
    void onProcessingStarted(const std::string_view operationName) override {
        std::cout << "┌─────────────────────────────────────────┐" << std::endl;
        std::cout << "│ Starting: " << operationName
                  << std::string(36 - operationName.length(), ' ') << "│" << std::endl;
        std::cout << "├─────────────────────────────────────────┤" << std::endl;
    }

    void onProgressUpdated(float progress) override {
        int barWidth = 35;
        int pos = static_cast<int>(barWidth * progress);

        std::cout << "│ [";
        for (int i = 0; i < barWidth; ++i) {
            if (i < pos)
                std::cout << "=";
            else if (i == pos)
                std::cout << ">";
            else
                std::cout << " ";
        }

        std::cout << "] " << int(progress * 100.0) << "% │" << std::endl;
    }

    void onProcessingCompleted(std::string_view operationName, bool success) override {
        std::cout << "│ " << (success ? "Completed: " : "Failed: ") << operationName
                  << std::string(34 - (success ? 11 : 8) - operationName.length(), ' ') << "│"
                  << std::endl;
        std::cout << "└─────────────────────────────────────────┘" << std::endl << std::endl;
    }

    void onError(std::string_view errorMessage) override {
        std::cout << "│ Error: " << errorMessage << std::string(35 - 7 - errorMessage.length(), ' ')
                  << "│" << std::endl;
        std::cout << "└─────────────────────────────────────────┘" << std::endl << std::endl;
    }
};

int main() {
    try {
       
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
}
