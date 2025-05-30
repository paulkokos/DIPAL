// src/Image/ImageFactory.cpp
#include "../../include/DIPAL/Image/ImageFactory.hpp"

#include "../../include/DIPAL/IO/ImageIO.hpp"
#include "../../include/DIPAL/Image/BinaryImage.hpp"
#include "../../include/DIPAL/Image/ColorImage.hpp"
#include "../../include/DIPAL/Image/GrayscaleImage.hpp"

#include <filesystem>
#include <format>
#include <memory>
#include <stdexcept>
#include <string>

namespace DIPAL {

Result<std::unique_ptr<Image>> ImageFactory::create(int width, int height, Image::Type type) {
    if (width <= 0 || height <= 0) {
        return makeErrorResult<std::unique_ptr<Image>>(
            ErrorCode::InvalidParameter, std::format("Invalid dimensions: {}x{}", width, height));
    }

    try {
        switch (type) {
            case Image::Type::Binary: {
                auto result = createBinary(width, height);
                if (!result) {
                    return makeErrorResult<std::unique_ptr<Image>>(result.error().code(),
                                                                   result.error().message());
                }
                return makeSuccessResult<std::unique_ptr<Image>>(std::move(result.value()));
            }
            case Image::Type::Grayscale: {
                auto result = createGrayscale(width, height);
                if (!result) {
                    return makeErrorResult<std::unique_ptr<Image>>(result.error().code(),
                                                                   result.error().message());
                }
                return makeSuccessResult<std::unique_ptr<Image>>(std::move(result.value()));
            }
            case Image::Type::RGB: {
                auto result = createColor(width, height, false);
                if (!result) {
                    return makeErrorResult<std::unique_ptr<Image>>(result.error().code(),
                                                                   result.error().message());
                }
                return makeSuccessResult<std::unique_ptr<Image>>(std::move(result.value()));
            }
            case Image::Type::RGBA: {
                auto result = createColor(width, height, true);
                if (!result) {
                    return makeErrorResult<std::unique_ptr<Image>>(result.error().code(),
                                                                   result.error().message());
                }
                return makeSuccessResult<std::unique_ptr<Image>>(std::move(result.value()));
            }
            default:
                return makeErrorResult<std::unique_ptr<Image>>(
                    ErrorCode::InvalidParameter,
                    std::format("Unsupported image type: {}", static_cast<int>(type)));
        }
    } catch (const std::exception& e) {
        return makeErrorResult<std::unique_ptr<Image>>(
            ErrorCode::InternalError, std::format("Failed to create image: {}", e.what()));
    }
}

Result<std::unique_ptr<BinaryImage>> ImageFactory::createBinary(int width, int height) {
    if (width <= 0 || height <= 0) {
        return makeErrorResult<std::unique_ptr<BinaryImage>>(
            ErrorCode::InvalidParameter, std::format("Invalid dimensions: {}x{}", width, height));
    }

    try {
        auto image = std::make_unique<BinaryImage>(width, height);

        // Initialize all pixels to black (false)
        auto fillResult = image->fill(false);
        if (!fillResult) {
            return makeErrorResult<std::unique_ptr<BinaryImage>>(
                fillResult.error().code(),
                std::format("Failed to initialize binary image: {}", fillResult.error().message()));
        }

        return makeSuccessResult(std::move(image));
    } catch (const std::exception& e) {
        return makeErrorResult<std::unique_ptr<BinaryImage>>(
            ErrorCode::InternalError, std::format("Failed to create binary image: {}", e.what()));
    }
}

Result<std::unique_ptr<GrayscaleImage>> ImageFactory::createGrayscale(int width, int height) {
    if (width <= 0 || height <= 0) {
        return makeErrorResult<std::unique_ptr<GrayscaleImage>>(
            ErrorCode::InvalidParameter, std::format("Invalid dimensions: {}x{}", width, height));
    }

    try {
        auto image = std::make_unique<GrayscaleImage>(width, height);
        return makeSuccessResult(std::move(image));
    } catch (const std::exception& e) {
        return makeErrorResult<std::unique_ptr<GrayscaleImage>>(
            ErrorCode::InternalError,
            std::format("Failed to create grayscale image: {}", e.what()));
    }
}

Result<std::unique_ptr<ColorImage>> ImageFactory::createColor(int width,
                                                              int height,
                                                              bool hasAlpha) {
    if (width <= 0 || height <= 0) {
        return makeErrorResult<std::unique_ptr<ColorImage>>(
            ErrorCode::InvalidParameter, std::format("Invalid dimensions: {}x{}", width, height));
    }

    try {
        auto image = std::make_unique<ColorImage>(width, height, hasAlpha);
        return makeSuccessResult(std::move(image));
    } catch (const std::exception& e) {
        return makeErrorResult<std::unique_ptr<ColorImage>>(
            ErrorCode::InternalError, std::format("Failed to create color image: {}", e.what()));
    }
}

Result<std::unique_ptr<GrayscaleImage>> ImageFactory::toGrayscale(const ColorImage& image) {
    try {
        auto result = createGrayscale(image.getWidth(), image.getHeight());
        if (!result) {
            return result;
        }

        auto& grayscale = *result.value();

        for (int y = 0; y < image.getHeight(); ++y) {
            for (int x = 0; x < image.getWidth(); ++x) {
                uint8_t r, g, b, a;
                auto pixelResult = image.getPixel(x, y, r, g, b, a);

                if (!pixelResult) {
                    return makeErrorResult<std::unique_ptr<GrayscaleImage>>(
                        pixelResult.error().code(), pixelResult.error().message());
                }

                // Standard grayscale conversion formula (luminance)
                uint8_t gray = static_cast<uint8_t>(0.299f * r + 0.587f * g + 0.114f * b);

                auto setResult = grayscale.setPixel(x, y, gray);
                if (!setResult) {
                    return makeErrorResult<std::unique_ptr<GrayscaleImage>>(
                        setResult.error().code(), setResult.error().message());
                }
            }
        }

        return result;
    } catch (const std::exception& e) {
        return makeErrorResult<std::unique_ptr<GrayscaleImage>>(
            ErrorCode::InternalError, std::format("Failed to convert to grayscale: {}", e.what()));
    }
}

Result<std::unique_ptr<ColorImage>> ImageFactory::toColor(const GrayscaleImage& image,
                                                          bool hasAlpha) {
    try {
        auto result = createColor(image.getWidth(), image.getHeight(), hasAlpha);
        if (!result) {
            return result;
        }

        auto& colorImage = *result.value();

        for (int y = 0; y < image.getHeight(); ++y) {
            for (int x = 0; x < image.getWidth(); ++x) {
                auto pixelResult = image.getPixel(x, y);

                if (!pixelResult) {
                    return makeErrorResult<std::unique_ptr<ColorImage>>(
                        pixelResult.error().code(), pixelResult.error().message());
                }

                uint8_t gray = pixelResult.value();

                auto setResult = colorImage.setPixel(x, y, gray, gray, gray);
                if (!setResult) {
                    return makeErrorResult<std::unique_ptr<ColorImage>>(
                        setResult.error().code(), setResult.error().message());
                }
            }
        }

        return result;
    } catch (const std::exception& e) {
        return makeErrorResult<std::unique_ptr<ColorImage>>(
            ErrorCode::InternalError, std::format("Failed to convert to color: {}", e.what()));
    }
}

Result<std::unique_ptr<BinaryImage>> ImageFactory::toBinary(const GrayscaleImage& image,
                                                            uint8_t threshold,
                                                            bool invert) {
    return BinaryImage::fromGrayscale(image, threshold, invert);
}

Result<std::unique_ptr<GrayscaleImage>> ImageFactory::fromBinary(const BinaryImage& image,
                                                                 uint8_t whiteValue,
                                                                 uint8_t blackValue) {
    try {
        auto result = std::make_unique<GrayscaleImage>(image.getWidth(), image.getHeight());

        for (int y = 0; y < image.getHeight(); ++y) {
            for (int x = 0; x < image.getWidth(); ++x) {
                auto pixelResult = image.getPixel(x, y);
                if (!pixelResult) {
                    return makeErrorResult<std::unique_ptr<GrayscaleImage>>(
                        pixelResult.error().code(), pixelResult.error().message());
                }

                bool isWhite = pixelResult.value();
                uint8_t grayValue = isWhite ? whiteValue : blackValue;

                auto setResult = result->setPixel(x, y, grayValue);
                if (!setResult) {
                    return makeErrorResult<std::unique_ptr<GrayscaleImage>>(
                        setResult.error().code(), setResult.error().message());
                }
            }
        }

        return makeSuccessResult(std::move(result));
    } catch (const std::exception& e) {
        return makeErrorResult<std::unique_ptr<GrayscaleImage>>(
            ErrorCode::ProcessingFailed,
            std::format("Failed to create grayscale image from binary: {}", e.what()));
    }
}

Result<std::unique_ptr<Image>> ImageFactory::loadImage(std::string_view filename) {
    try {
        // Check if file exists
        if (!std::filesystem::exists(filename)) {
            return makeErrorResult<std::unique_ptr<Image>>(
                ErrorCode::FileNotFound, std::format("File not found: {}", filename));
        }

        // Use ImageIO to load the image based on file extension
        return ImageIO::load(filename);
    } catch (const std::exception& e) {
        return makeErrorResult<std::unique_ptr<Image>>(
            ErrorCode::InternalError, std::format("Failed to load image: {}", e.what()));
    }
}

VoidResult ImageFactory::saveImage(const Image& image, std::string_view filename) {
    try {
        // Use ImageIO to save the image based on file extension
        return ImageIO::save(image, filename);
    } catch (const std::exception& e) {
        return makeVoidErrorResult(ErrorCode::InternalError,
                                   std::format("Failed to save image: {}", e.what()));
    }
}

// Add an overload for quality-based saving
VoidResult ImageFactory::saveImage(const Image& image, std::string_view filename, int quality) {
    try {
        // Use ImageIO to save the image based on file extension with quality parameter
        return ImageIO::save(image, filename, quality);
    } catch (const std::exception& e) {
        return makeVoidErrorResult(ErrorCode::InternalError,
                                   std::format("Failed to save image: {}", e.what()));
    }
}

}  // namespace DIPAL
