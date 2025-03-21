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
class CustomProgressObserver : public ProcessingObserver {
public:
    void onProcessingStarted(const std::string& operationName) override {
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

    void onProcessingCompleted(const std::string& operationName, bool success) override {
        std::cout << "│ " << (success ? "Completed: " : "Failed: ") << operationName
                  << std::string(34 - (success ? 11 : 8) - operationName.length(), ' ') << "│"
                  << std::endl;
        std::cout << "└─────────────────────────────────────────┘" << std::endl << std::endl;
    }

    void onError(const std::string& errorMessage) override {
        std::cout << "│ Error: " << errorMessage << std::string(35 - 7 - errorMessage.length(), ' ')
                  << "│" << std::endl;
        std::cout << "└─────────────────────────────────────────┘" << std::endl << std::endl;
    }
};

int main() {
    try {
        // Create a grayscale image with a simple pattern
        auto image = ImageFactory::createGrayscale(500, 400);

        // Fill with a circular pattern
        int centerX = image->getWidth() / 2;
        int centerY = image->getHeight() / 2;
        int maxRadius = std::min(centerX, centerY);
        (void)maxRadius;

        for (int y = 0; y < image->getHeight(); ++y) {
            for (int x = 0; x < image->getWidth(); ++x) {
                int dx = x - centerX;
                int dy = y - centerY;
                float distance = std::sqrt(dx * dx + dy * dy);

                // Create concentric circles
                uint8_t value = static_cast<uint8_t>((std::sin(distance * 0.1) + 1.0) * 127.5);

                image->setPixel(x, y, value);
            }
        }

        std::cout << "Created a grayscale image with dimensions " << image->getWidth() << "x"
                  << image->getHeight() << std::endl;

        // Create an image processor with a custom observer
        ImageProcessor processor;
        auto observer = std::make_shared<CustomProgressObserver>();
        processor.addObserver(observer);

        // Create a processing pipeline with multiple commands
        std::vector<std::unique_ptr<ProcessingCommand>> pipeline;

        // Add a Gaussian blur filter with sigma=1.5, kernel size=5
        pipeline.push_back(
            std::make_unique<FilterCommand>(std::make_unique<GaussianBlurFilter>(1.5f, 5)));

        // Apply the pipeline
        std::cout << "Applying processing pipeline with " << pipeline.size() << " operations..."
                  << std::endl;
        auto processedImage = processor.processAll(*image, std::move(pipeline));

        std::cout << "All pipeline operations completed successfully!" << std::endl;

        // Convert to color for visualization
        auto colorImage =
            ImageFactory::toColor(*dynamic_cast<GrayscaleImage*>(processedImage.get()));

        std::cout << "Converted result to color image" << std::endl;

        // In a real application, we would save these images to files
        // For this example, we'll just report success

        return 0;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
}
