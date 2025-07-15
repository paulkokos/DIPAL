// tests/utils/image_test_utils.hpp
// Image-specific test utilities header for DIPAL Library

#pragma once

#include <DIPAL/DIPAL.hpp>
#include <memory>

namespace DIPAL {
namespace ImageTestUtils {

/**
 * @brief Create a test grayscale image filled with a specific value
 */
std::unique_ptr<GrayscaleImage> createTestGrayscaleImage(int width, int height, uint8_t fillValue = 128);

/**
 * @brief Create a test color image filled with a specific color
 */
std::unique_ptr<ColorImage> createTestColorImage(int width, int height, uint8_t r = 128, uint8_t g = 128, uint8_t b = 128);

/**
 * @brief Create a grayscale image with a gradient pattern
 */
std::unique_ptr<GrayscaleImage> createGradientImage(int width, int height);

/**
 * @brief Create a color image with a rainbow pattern
 */
std::unique_ptr<ColorImage> createRainbowImage(int width, int height);

/**
 * @brief Create a checkerboard pattern image
 */
template<typename ImageType>
std::unique_ptr<ImageType> createCheckerboardImage(int width, int height, int squareSize = 8);

/**
 * @brief Validate image properties
 */
template<typename ImageType>
bool validateImageProperties(const ImageType& image, int expectedWidth, int expectedHeight);

} // namespace ImageTestUtils
} // namespace DIPAL
