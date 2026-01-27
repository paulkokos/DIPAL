// examples/basic/transform_example.cpp
#include <DIPAL/DIPAL.hpp>
#include <iostream>
#include <memory>

using namespace DIPAL;

int main() {
    try {
        // Create a color image with a gradient pattern
        auto image = ImageFactory::createColor(400, 300);

        for (int y = 0; y < image->getHeight(); ++y) {
            for (int x = 0; x < image->getWidth(); ++x) {
                auto r = static_cast<uint8_t>((static_cast<float>(x) / image->getWidth()) * 255);
                auto g = static_cast<uint8_t>((static_cast<float>(y) / image->getHeight()) * 255);
                auto b = static_cast<uint8_t>(128);
                image->setPixel(x, y, r, g, b);
            }
        }

        std::cout << "Created image: " << image->getWidth() << "x"
                  << image->getHeight() << std::endl;

        // Resize to half dimensions using bilinear interpolation
        ResizeTransform resize(200, 150, InterpolationMethod::Bilinear);
        auto resized = resize.apply(*image);
        if (!resized) {
            std::cerr << "Resize failed: " << resized.error().toString() << std::endl;
            return 1;
        }
        std::cout << "Resized to: " << resized.value()->getWidth() << "x"
                  << resized.value()->getHeight() << std::endl;

        // Rotate by 45 degrees
        RotateTransform rotate(45.0f);
        auto rotated = rotate.apply(*image);
        if (!rotated) {
            std::cerr << "Rotation failed: " << rotated.error().toString() << std::endl;
            return 1;
        }
        std::cout << "Rotated to: " << rotated.value()->getWidth() << "x"
                  << rotated.value()->getHeight() << std::endl;

        std::cout << "All transformations completed successfully!" << std::endl;
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
}
