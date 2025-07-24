// tests/utils/test_image_generator.cpp
// Procedural test image generation utilities

#include "test_image_generator.hpp"

#include <DIPAL/DIPAL.hpp>

#include <cmath>
#include <random>

namespace DIPAL {
namespace TestImageGenerator {

std::unique_ptr<GrayscaleImage> generateNoiseImage(int width,
                                                   int height,
                                                   uint8_t mean,
                                                   uint8_t stddev) {
    auto result = ImageFactory::createGrayscale(width, height);
    if (!result)
        return nullptr;

    auto image = std::move(result.value());

    std::random_device rd;
    std::mt19937 gen(rd());
    std::normal_distribution<float> dist(mean, stddev);

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            float value = dist(gen);
            uint8_t pixel = static_cast<uint8_t>(std::clamp(value, 0.0f, 255.0f));
            auto setResult = image->setPixel(x, y, pixel);
            if (!setResult) {
                return nullptr;
            }
        }
    }

    return image;
}
std::unique_ptr<GrayscaleImage> generateSineWaveImage(int width, int height, double frequency) {
    auto result = ImageFactory::createGrayscale(width, height);
    if (!result)
        return nullptr;

    auto image = std::move(result.value());

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            double value = std::sin(x * frequency * 2.0 * M_PI / width) * 127.5 + 127.5;
            uint8_t pixel = static_cast<uint8_t>(std::clamp(value, 0.0, 255.0));
            auto setResult = image->setPixel(x, y, pixel);
            if (!setResult) {
                return nullptr;
            }
        }
    }

    return image;
}
std::unique_ptr<GrayscaleImage> generateCircleImage(int width,
                                                    int height,
                                                    int centerX,
                                                    int centerY,
                                                    int radius) {
    auto result = ImageFactory::createGrayscale(width, height);
    if (!result)
        return nullptr;

    auto image = std::move(result.value());

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            double distance =
                std::sqrt((x - centerX) * (x - centerX) + (y - centerY) * (y - centerY));
            uint8_t pixel = (distance <= radius) ? 255 : 0;
            auto setResult = image->setPixel(x, y, pixel);
            if (!setResult) {
                return nullptr;
            }
        }
    }

    return image;
}
std::unique_ptr<ColorImage> generateColorGradientImage(int width, int height, GradientType type) {
    auto result = ImageFactory::createColor(width, height);
    if (!result)
        return nullptr;

    auto image = std::move(result.value());

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            uint8_t r = 0, g = 0, b = 0;

            switch (type) {
                case GradientType::Horizontal:
                    r = static_cast<uint8_t>((static_cast<double>(x) / width) * 255);
                    g = 128;
                    b = 255 - r;
                    break;
                case GradientType::Vertical:
                    g = static_cast<uint8_t>((static_cast<double>(y) / height) * 255);
                    r = 128;
                    b = 255 - g;
                    break;
                case GradientType::Radial: {
                    double centerX = width / 2.0;
                    double centerY = height / 2.0;
                    double maxDistance = std::sqrt(centerX * centerX + centerY * centerY);
                    double distance =
                        std::sqrt((x - centerX) * (x - centerX) + (y - centerY) * (y - centerY));
                    double normalized = distance / maxDistance;
                    r = static_cast<uint8_t>(normalized * 255);
                    g = static_cast<uint8_t>((1.0 - normalized) * 255);
                    b = 128;
                } break;
            }

            auto setResult = image->setPixel(x, y, r, g, b);
            if (!setResult) {
                return nullptr;
            }
        }
    }

    return image;
}
}  // namespace TestImageGenerator
}  // namespace DIPAL
