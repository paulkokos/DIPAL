// examples/basic/hello_dipal.cpp
// Simple "Hello DIPAL" example demonstrating basic library usage

#include <DIPAL/DIPAL.hpp>
#include <iostream>

int main() {
    using namespace DIPAL;

    std::cout << "=== DIPAL Library Hello World ===" << std::endl;
    std::cout << "Version: " << getVersion() << std::endl;
    std::cout << std::endl;

    // Create a simple grayscale image
    std::cout << "Creating a 100x100 grayscale image..." << std::endl;
    auto imgResult = ImageFactory::createGrayscale(100, 100);

    if (!imgResult) {
        std::cerr << "Error creating image: " << imgResult.error().toString() << std::endl;
        return 1;
    }

    auto& image = imgResult.value();
    std::cout << "✓ Image created: " << image->getWidth() << "x" << image->getHeight() << std::endl;

    // Set some pixel values to create a gradient
    std::cout << "Populating image with gradient pattern..." << std::endl;
    for (int y = 0; y < 100; ++y) {
        for (int x = 0; x < 100; ++x) {
            uint8_t value = static_cast<uint8_t>((x + y) / 2);
            if (!image->setPixel(x, y, value).has_value()) {
                std::cerr << "Error setting pixel at (" << x << ", " << y << ")" << std::endl;
                return 1;
            }
        }
    }
    std::cout << "✓ Image populated with gradient" << std::endl;

    // Apply a Gaussian blur filter
    std::cout << "Applying Gaussian blur filter..." << std::endl;
    GaussianBlurFilter blurFilter(1.0f, 3);
    auto blurResult = blurFilter.apply(*image);

    if (!blurResult) {
        std::cerr << "Error applying filter: " << blurResult.error().toString() << std::endl;
        return 1;
    }
    std::cout << "✓ Filter applied successfully" << std::endl;

    // Verify the filtered image
    std::cout << "Filtered image size: " << blurResult.value()->getWidth()
              << "x" << blurResult.value()->getHeight() << std::endl;

    // Demonstrate error handling
    std::cout << std::endl << "Demonstrating error handling..." << std::endl;
    auto invalidImgResult = ImageFactory::createGrayscale(0, 100);
    if (!invalidImgResult) {
        std::cout << "✓ Correctly rejected invalid image size" << std::endl;
        std::cout << "  Error: " << invalidImgResult.error().toString() << std::endl;
    }

    std::cout << std::endl << "=== Hello DIPAL Example Complete ===" << std::endl;
    return 0;
}
