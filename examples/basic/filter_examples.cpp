// examples/basic/filter_example.cpp
#include <DIPAL/DIPAL.hpp>
#include <DIPAL/Filters/GaussianBlurFilter.hpp>
#include <DIPAL/ImageProcessor/FilterCommand.hpp>
#include <DIPAL/ImageProcessor/ImageProcessor.hpp>
#include <DIPAL/Observer/ProcessingObserver.hpp>
#include <iostream>
#include <memory>

using namespace DIPAL;

int main() {
    try {
        // Create a color image with a simple pattern
        auto image = ImageFactory::createColor(300, 200);

        // Fill with a gradient pattern
        for (int y = 0; y < image->getHeight(); ++y) {
            for (int x = 0; x < image->getWidth(); ++x) {
                uint8_t r = static_cast<uint8_t>((static_cast<float>(x) / image->getWidth()) * 255);
                uint8_t g =
                    static_cast<uint8_t>((static_cast<float>(y) / image->getHeight()) * 255);
                uint8_t b = static_cast<uint8_t>(((static_cast<float>(x) + static_cast<float>(y)) /
                                                  (image->getWidth() + image->getHeight())) *
                                                 255);

                image->setPixel(x, y, r, g, b);
            }
        }

        std::cout << "Created a color image with dimensions " << image->getWidth() << "x"
                  << image->getHeight() << std::endl;

        // Create an image processor with a console observer
        ImageProcessor processor;
        auto observer = std::make_shared<ConsoleObserver>();
        processor.addObserver(observer);

        // Create a Gaussian blur filter and command
        auto gaussianFilter = std::make_unique<GaussianBlurFilter>(2.0f, 5);
        auto filterCommand = std::make_unique<FilterCommand>(std::move(gaussianFilter));

        // Process the image
        std::cout << "Applying Gaussian blur..." << std::endl;
        auto processedImage = processor.process(*image, std::move(filterCommand));

        std::cout << "Processing complete!" << std::endl;

        // Convert to grayscale for another example
        std::cout << "Converting to grayscale..." << std::endl;
        auto grayscaleImage =
            ImageFactory::toGrayscale(*dynamic_cast<ColorImage*>(processedImage.get()));

        std::cout << "All operations completed successfully!" << std::endl;

        // In a real application, we would save these images to files
        // For this example, we'll just report success

        return 0;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
}
