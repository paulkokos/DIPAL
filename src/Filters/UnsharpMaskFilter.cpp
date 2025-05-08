// src/Filters/UnsharpMaskFilter.cpp
#include "../../include/DIPAL/Filters/UnsharpMaskFilter.hpp"

#include "../../include/DIPAL/Filters/GaussianBlurFilter.hpp"
#include "../../include/DIPAL/Image/ColorImage.hpp"
#include "../../include/DIPAL/Image/GrayscaleImage.hpp"

#include <algorithm>
#include <cmath>
#include <format>

namespace DIPAL {

UnsharpMaskFilter::UnsharpMaskFilter(float amount, float radius, uint8_t threshold)
    : m_amount(amount), m_radius(radius), m_threshold(threshold) {
    if (amount < 0.0f) {
        throw std::invalid_argument(std::format("Amount must be positive, got {}", amount));
    }
    if (radius <= 0.0f) {
        throw std::invalid_argument(std::format("Radius must be positive, got {}", radius));
    }
}

Result<std::unique_ptr<Image>> UnsharpMaskFilter::apply(const Image& image) const {
    try {
        // Create a blurred version of the image using Gaussian blur
        GaussianBlurFilter blurFilter(
            m_radius, static_cast<int>(m_radius * 3.0f) | 1);  // Ensure kernel size is odd
        auto blurredResult = blurFilter.apply(image);

        if (!blurredResult) {
            return makeErrorResult<std::unique_ptr<Image>>(
                blurredResult.error().code(),
                std::format("Unsharp mask failed in blur step: {}",
                            blurredResult.error().message()));
        }

        // Create a result image as a clone of the input
        auto result = image.clone();

        // Apply unsharp masking based on image type
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
                            "Failed to get pixel values for unsharp mask");
                    }

                    uint8_t srcValue = origResult.value();
                    uint8_t blurValue = blurResult.value();

                    // Calculate the difference for sharpening
                    int diff = static_cast<int>(srcValue) - static_cast<int>(blurValue);

                    // Apply threshold
                    if (std::abs(diff) < m_threshold) {
                        diff = 0;
                    }

                    // Apply sharpening with amount parameter
                    int newValue = static_cast<int>(srcValue) + static_cast<int>(m_amount * diff);
                    newValue = std::clamp(newValue, 0, 255);

                    auto setResult = dstGray.setPixel(x, y, static_cast<uint8_t>(newValue));
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
            bool hasAlpha = (image.getType() == Image::Type::RGBA);

            for (int y = 0; y < image.getHeight(); ++y) {
                for (int x = 0; x < image.getWidth(); ++x) {
                    uint8_t srcR, srcG, srcB, srcA;
                    uint8_t blurR, blurG, blurB, blurA;

                    auto srcResult = srcColor.getPixel(x, y, srcR, srcG, srcB, srcA);
                    auto blurResult = blurredColor.getPixel(x, y, blurR, blurG, blurB, blurA);

                    if (!srcResult || !blurResult) {
                        return makeErrorResult<std::unique_ptr<Image>>(
                            ErrorCode::ProcessingFailed,
                            "Failed to get pixel values for unsharp mask");
                    }

                    // Calculate differences for each channel
                    int diffR = static_cast<int>(srcR) - static_cast<int>(blurR);
                    int diffG = static_cast<int>(srcG) - static_cast<int>(blurG);
                    int diffB = static_cast<int>(srcB) - static_cast<int>(blurB);

                    // Apply thresholding
                    if (std::abs(diffR) < m_threshold)
                        diffR = 0;
                    if (std::abs(diffG) < m_threshold)
                        diffG = 0;
                    if (std::abs(diffB) < m_threshold)
                        diffB = 0;

                    // Apply sharpening with amount parameter
                    int newR = static_cast<int>(srcR) + static_cast<int>(m_amount * diffR);
                    int newG = static_cast<int>(srcG) + static_cast<int>(m_amount * diffG);
                    int newB = static_cast<int>(srcB) + static_cast<int>(m_amount * diffB);

                    // Clamp to valid range
                    newR = std::clamp(newR, 0, 255);
                    newG = std::clamp(newG, 0, 255);
                    newB = std::clamp(newB, 0, 255);

                    auto setResult = dstColor.setPixel(x,
                                                       y,
                                                       static_cast<uint8_t>(newR),
                                                       static_cast<uint8_t>(newG),
                                                       static_cast<uint8_t>(newB),
                                                       hasAlpha ? srcA : 255);

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
            ErrorCode::ProcessingFailed, std::format("Unsharp mask failed: {}", e.what()));
    }
}

std::string_view UnsharpMaskFilter::getName() const {
    return "UnsharpMaskFilter";
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
