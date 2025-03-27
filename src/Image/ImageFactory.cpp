// src/Image/ImageFactory.cpp
#include "../../include/DIPAL/Image/ImageFactory.hpp"
#include <memory>
#include <stdexcept>

namespace DIPAL {

std::unique_ptr<Image> ImageFactory::create(int width, int height, Image::Type type) {
    switch (type) {
        case Image::Type::Grayscale:
            return createGrayscale(width, height);
        case Image::Type::RGB:
            return createColor(width, height, false);
        case Image::Type::RGBA:
            return createColor(width, height, true);
        default:
            throw std::invalid_argument("Unsupported image type");
    }
}

std::unique_ptr<GrayscaleImage> ImageFactory::createGrayscale(int width, int height) {
    return std::make_unique<GrayscaleImage>(width, height);
}

std::unique_ptr<ColorImage> ImageFactory::createColor(int width, int height, bool hasAlpha) {
    return std::make_unique<ColorImage>(width, height, hasAlpha);
}

std::unique_ptr<GrayscaleImage> ImageFactory::toGrayscale(const ColorImage& image) {
    auto result = std::make_unique<GrayscaleImage>(image.getWidth(), image.getHeight());

    for (int y = 0; y < image.getHeight(); ++y) {
        for (int x = 0; x < image.getWidth(); ++x) {
            uint8_t r, g, b, a;
            image.getPixel(x, y, r, g, b, a);

            // Standard grayscale conversion formula (luminance)
            uint8_t gray = static_cast<uint8_t>(0.299 * r + 0.587 * g + 0.114 * b);

            result->setPixel(x, y, gray);
        }
    }

    return result;
}

std::unique_ptr<ColorImage> ImageFactory::toColor(const GrayscaleImage& image, bool hasAlpha) {
    auto result = std::make_unique<ColorImage>(image.getWidth(), image.getHeight(), hasAlpha);

    for (int y = 0; y < image.getHeight(); ++y) {
        for (int x = 0; x < image.getWidth(); ++x) {
            uint8_t gray = image.getPixel(x, y);
            // Set each color channel to the grayscale value
            result->setPixel(x, y, gray, gray, gray);
        }
    }

    return result;
}

}  // namespace DIPAL
