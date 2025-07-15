// tests/utils/image_test_utils.cpp
// Image-specific test utilities for DIPAL Library

#include "image_test_utils.hpp"

#include <DIPAL/DIPAL.hpp>

namespace DIPAL {
namespace ImageTestUtils {

std::unique_ptr<GrayscaleImage> createTestGrayscaleImage(int width, int height, uint8_t fillValue) {
    auto result = ImageFactory::createGrayscale(width, height);
    if (!result) {
        return nullptr;
    }

    auto image = std::move(result.value());

    // Fill with the specified value
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            auto setResult = image->setPixel(x, y, fillValue);
            if (!setResult) {
                // Handle error - could return nullptr or throw
                return nullptr;
            }
        }
    }

    return image;
}

std::unique_ptr<ColorImage> createTestColorImage(int width,
                                                 int height,
                                                 uint8_t r,
                                                 uint8_t g,
                                                 uint8_t b) {
    auto result = ImageFactory::createColor(width, height);
    if (!result) {
        return nullptr;
    }

    auto image = std::move(result.value());

    // Fill with the specified color
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            auto setResult = image->setPixel(x, y, r, g, b);
            if (!setResult) {
                // Handle error - could return nullptr or throw
                return nullptr;
            }
        }
    }

    return image;
}

std::unique_ptr<GrayscaleImage> createGradientImage(int width, int height) {
    auto result = ImageFactory::createGrayscale(width, height);
    if (!result) {
        return nullptr;
    }

    auto image = std::move(result.value());

    // Create a diagonal gradient
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            uint8_t value =
                static_cast<uint8_t>((static_cast<double>(x + y) / (width + height)) * 255);
            auto setResult = image->setPixel(x, y, value);
            if (!setResult) {
                return nullptr;
            }
        }
    }

    return image;
}

std::unique_ptr<ColorImage> createRainbowImage(int width, int height) {
    auto result = ImageFactory::createColor(width, height);
    if (!result) {
        return nullptr;
    }

    auto image = std::move(result.value());

    // Create a rainbow pattern
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            uint8_t r = static_cast<uint8_t>((static_cast<double>(x) / width) * 255);
            uint8_t g = static_cast<uint8_t>((static_cast<double>(y) / height) * 255);
            uint8_t b = static_cast<uint8_t>(
                ((static_cast<double>(x) + static_cast<double>(y)) / (width + height)) * 255);

            auto setResult = image->setPixel(x, y, r, g, b);
            if (!setResult) {
                return nullptr;
            }
        }
    }

    return image;
}

}  // namespace ImageTestUtils
}  // namespace DIPAL
