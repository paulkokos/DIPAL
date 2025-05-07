// src/Filters/UnsharpMaskFilter.cpp
#include "../../include/DIPAL/Filters/UnsharpMaskFilter.hpp"

#include "../../include/DIPAL/Image/ColorImage.hpp"
#include "../../include/DIPAL/Image/GrayscaleImage.hpp"

#include <algorithm>
#include <cmath>
#include <format>


namespace DIPAL {

UnsharpMaskFilter::UnsharpMaskFilter(float amount, float radius, uint8_t threshold)
    : m_amount(amount), m_radius(radius), m_threshold(threshold) {
    if (amount < 0.0f) {
        throw std::invalid_argument(std::format("Amount must be non-negative, got {}", amount));
    }

    if (radius <= 0.0f) {
        throw std::invalid_argument(std::format("Radius must be positive, got {}", radius));
    }
}

Result<std::unique_ptr<Image>> UnsharpMaskFilter::apply(const Image& image) const {
    if (image.isEmpty()) {
        return makeErrorResult<std::unique_ptr<Image>>(
            ErrorCode::InvalidParameter, "Cannot apply unsharp mask to an empty image");
    }

    try {
        // Step 1: Create a blurred version of the original image
        GaussianBlurFilter blurFilter(m_radius, static_cast<int>(std::ceil(m_radius * 3)) * 2 + 1);

        auto blurredResult = blurFilter.apply(image);
        if (!blurredResult) {
            return makeErrorResult<std::unique_ptr<Image>>(
                blurredResult.error().code(),
                std::format("Failed to create blurred image: {}", blurredResult.error().message()));
        }

        // Step 2: Create output image as a clone of the input
        auto result = image.clone();

        // Step 3: Apply unsharp mask algorithm based on image type
        if (image.getType() == Image::Type::Grayscale) {
            const auto& srcGray = static_cast<const GrayscaleImage&>(image);
            const auto& blurredGray = static_cast<const GrayscaleImage&>(*blurredResult.value());
            auto& dstGray = static_cast<GrayscaleImage&>(*result);

            for (int y = 0; y < image.getHeight(); ++y) {
                for (int x = 0; x < image.getWidth(); ++x) {
                    auto origResult = srcGray.getPixel(x, y);
                    auto blurResult = blurredGray.getPixel(x, y);

                    if (!origResult || !blurResult) {
                        return makeErrorResult<std::unique_ptr<Image>>(
                            ErrorCode::ProcessingFailed,
                            "Failed to get pixel values during unsharp mask processing");
                    }

                    uint8_t origValue = origResult.value();
                    uint8_t blurValue = blurResult.value();

                    // Calculate the difference between original and blurred pixel
                    int diff = static_cast<int>(origValue) - blurValue;

                    // Apply threshold
                    if (std::abs(diff) <= m_threshold) {
                        continue;  // Skip pixels with differences below threshold
                    }

                    // Apply sharpening: original + amount * (original - blurred)
                    int sharpened = static_cast<int>(origValue) + static_cast<int>(m_amount * diff);

                    // Clamp to valid range
                    uint8_t finalValue = static_cast<uint8_t>(std::clamp(sharpened, 0, 255));

                    // Set the sharpened pixel
                    auto setResult = dstGray.setPixel(x, y, finalValue);
                    if (!setResult) {
                        return makeErrorResult<std::unique_ptr<Image>>(setResult.error().code(),
                                                                       setResult.error().message());
                    }
                }
            }
        } else if (image.getType() == Image::Type::RGB || image.getType() == Image::Type::RGBA) {
            const auto& srcColor = static_cast<const ColorImage&>(image);
            const auto& blurredColor = static_cast<const ColorImage&>(*blurredResult.value());
            auto& dstColor = static_cast<ColorImage&>(*result);

            for (int y = 0; y < image.getHeight(); ++y) {
                for (int x = 0; x < image.getWidth(); ++x) {
                    uint8_t origR, origG, origB, origA;
                    uint8_t blurR, blurG, blurB, blurA;

                    auto origResult = srcColor.getPixel(x, y, origR, origG, origB, origA);
                    auto blurResult = blurredColor.getPixel(x, y, blurR, blurG, blurB, blurA);

                    if (!origResult || !blurResult) {
                        return makeErrorResult<std::unique_ptr<Image>>(
                            ErrorCode::ProcessingFailed,
                            "Failed to get pixel values during unsharp mask processing");
                    }

                    // Calculate differences for each channel
                    int diffR = static_cast<int>(origR) - blurR;
                    int diffG = static_cast<int>(origG) - blurG;
                    int diffB = static_cast<int>(origB) - blurB;

                    // Check if any channel difference exceeds threshold
                    bool applySharpening = (std::abs(diffR) > m_threshold) ||
                                           (std::abs(diffG) > m_threshold) ||
                                           (std::abs(diffB) > m_threshold);

                    if (!applySharpening) {
                        continue;  // Skip pixels with differences below threshold
                    }

                    // Apply sharpening to each channel
                    int sharpenedR = static_cast<int>(origR) + static_cast<int>(m_amount * diffR);
                    int sharpenedG = static_cast<int>(origG) + static_cast<int>(m_amount * diffG);
                    int sharpenedB = static_cast<int>(origB) + static_cast<int>(m_amount * diffB);

                    // Clamp to valid range
                    uint8_t finalR = static_cast<uint8_t>(std::clamp(sharpenedR, 0, 255));
                    uint8_t finalG = static_cast<uint8_t>(std::clamp(sharpenedG, 0, 255));
                    uint8_t finalB = static_cast<uint8_t>(std::clamp(sharpenedB, 0, 255));

                    // Set the sharpened pixel (preserve alpha)
                    auto setResult = dstColor.setPixel(x, y, finalR, finalG, finalB, origA);
                    if (!setResult) {
                        return makeErrorResult<std::unique_ptr<Image>>(setResult.error().code(),
                                                                       setResult.error().message());
                    }
                }
            }
        } else {
            return makeErrorResult<std::unique_ptr<Image>>(
                ErrorCode::UnsupportedFormat,
                std::format("Unsupported image type for unsharp mask: {}",
                            static_cast<int>(image.getType())));
        }

        return makeSuccessResult(std::move(result));
    } catch (const std::exception& e) {
        return makeErrorResult<std::unique_ptr<Image>>(
            ErrorCode::ProcessingFailed, std::format("Unsharp mask filter failed: {}", e.what()));
    }
}

std::string_view UnsharpMaskFilter::getName() const {
    return "UnsharpMask";
}

std::unique_ptr<FilterStrategy> UnsharpMaskFilter::clone() const {
    return std::make_unique<UnsharpMaskFilter>(m_amount, m_radius, m_threshold);
}

float UnsharpMaskFilter::getAmount() const noexcept {
    return m_amount;
}

float UnsharpMaskFilter::getRadius() const noexcept {
    return m_radius;
}

uint8_t UnsharpMaskFilter::getThreshold() const noexcept {
    return m_threshold;
}

}  // namespace DIPAL
