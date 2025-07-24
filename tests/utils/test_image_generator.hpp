// tests/utils/test_image_generator.hpp
// Procedural test image generation utilities

#pragma once

#include <DIPAL/DIPAL.hpp>
#include <memory>

namespace DIPAL {
namespace TestImageGenerator {

enum class GradientType {
    Horizontal,
    Vertical,
    Radial
};

/**
 * @brief Generate a grayscale image with random noise
 */
std::unique_ptr<GrayscaleImage> generateNoiseImage(int width, int height, uint8_t mean = 128, uint8_t stddev = 32);

/**
 * @brief Generate a grayscale image with sine wave pattern
 */
std::unique_ptr<GrayscaleImage> generateSineWaveImage(int width, int height, double frequency = 1.0);

/**
 * @brief Generate a grayscale image with a circle
 */
std::unique_ptr<GrayscaleImage> generateCircleImage(int width, int height, int centerX, int centerY, int radius);

/**
 * @brief Generate a color image with gradient
 */
std::unique_ptr<ColorImage> generateColorGradientImage(int width, int height, GradientType type = GradientType::Horizontal);

/**
 * @brief Generate a test pattern image for debugging
 */
template<typename ImageType>
std::unique_ptr<ImageType> generateTestPatternImage(int width, int height);

} // namespace TestImageGenerator
} // namespace DIPAL
