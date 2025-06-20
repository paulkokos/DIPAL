// src/Transformation/RotateTransform.cpp
#include "../../include/DIPAL/Transformation/RotateTransform.hpp"

#include "../../include/DIPAL/Image/ColorImage.hpp"
#include "../../include/DIPAL/Image/GrayscaleImage.hpp"
#include "../../include/DIPAL/Image/ImageFactory.hpp"
#include "../../include/DIPAL/Transformation/Interpolation.hpp"

#include <algorithm>
#include <cmath>
#include <format>

namespace DIPAL {

RotateTransform::RotateTransform(float angle,
                                 RotationCenter center,
                                 InterpolationMethod method,
                                 bool resizeOutput)
    : m_angle(angle),
      m_centerType(center),
      m_centerX(0.0f),
      m_centerY(0.0f),
      m_method(method),
      m_resizeOutput(resizeOutput) {
    // Center values will be set during the transform based on image dimensions
}

RotateTransform::RotateTransform(float angle,
                                 float centerX,
                                 float centerY,
                                 InterpolationMethod method,
                                 bool resizeOutput)
    : m_angle(angle),
      m_centerType(RotationCenter::Custom),
      m_centerX(centerX),
      m_centerY(centerY),
      m_method(method),
      m_resizeOutput(resizeOutput) {}

Result<std::unique_ptr<Image>> RotateTransform::apply(const Image& image) const {
    // Check for empty image
    if (image.isEmpty()) {
        return makeErrorResult<std::unique_ptr<Image>>(ErrorCode::InvalidParameter,
                                                       "Cannot rotate an empty image");
    }

    try {
        // Convert angle to radians for calculations
        float angleRadians = m_angle * (M_PI / 180.0f);

        // Determine source image dimensions
        int srcWidth = image.getWidth();
        int srcHeight = image.getHeight();

        // Calculate rotation center in source image
        float centerX, centerY;
        switch (m_centerType) {
            case RotationCenter::Center:
                centerX = srcWidth / 2.0f;
                centerY = srcHeight / 2.0f;
                break;
            case RotationCenter::TopLeft:
                centerX = 0.0f;
                centerY = 0.0f;
                break;
            case RotationCenter::Custom:
                centerX = m_centerX;
                centerY = m_centerY;
                break;
            default:
                return makeErrorResult<std::unique_ptr<Image>>(ErrorCode::InvalidParameter,
                                                               "Invalid rotation center type");
        }

        // Calculate dimensions for the output image
        int dstWidth, dstHeight;
        if (m_resizeOutput) {
            // Calculate dimensions that will contain the entire rotated image
            auto [width, height] = calculateRotatedDimensions(srcWidth, srcHeight, angleRadians);
            dstWidth = width;
            dstHeight = height;
        } else {
            // Keep same dimensions as source
            dstWidth = srcWidth;
            dstHeight = srcHeight;
        }

        // Create transformation mapping function
        auto rotationFunc = createRotationMapping(angleRadians, centerX, centerY);
        auto mappingFunc =
            Interpolation::createMapping(srcWidth, srcHeight, dstWidth, dstHeight, rotationFunc);

        // Create output image of the appropriate type
        std::unique_ptr<Image> outputImage;

        switch (image.getType()) {
            case Image::Type::Grayscale: {
                // Create grayscale output image
                auto result = ImageFactory::createGrayscale(dstWidth, dstHeight);
                if (!result) {
                    return makeErrorResult<std::unique_ptr<Image>>(result.error().code(),
                                                                   result.error().message());
                }

                auto& output = static_cast<GrayscaleImage&>(*result.value());
                const auto& input = static_cast<const GrayscaleImage&>(image);

                // Apply rotation with interpolation
                for (int y = 0; y < dstHeight; ++y) {
                    for (int x = 0; x < dstWidth; ++x) {
                        // Get source coordinates
                        auto [srcX, srcY] = mappingFunc(x, y);

                        // Skip if source coordinates are outside image bounds
                        if (srcX < 0 || srcX >= srcWidth || srcY < 0 || srcY >= srcHeight) {
                            continue;
                        }

                        // Get interpolated value
                        auto valueResult =
                            Interpolation::interpolateGray(input, srcX, srcY, m_method);

                        if (!valueResult) {
                            return makeErrorResult<std::unique_ptr<Image>>(
                                valueResult.error().code(), valueResult.error().message());
                        }

                        // Set output pixel
                        auto setResult = output.setPixel(x, y, valueResult.value());
                        if (!setResult) {
                            return makeErrorResult<std::unique_ptr<Image>>(
                                setResult.error().code(), setResult.error().message());
                        }
                    }
                }

                outputImage = std::move(result.value());
                break;
            }

            case Image::Type::RGB:
            case Image::Type::RGBA: {
                // Create color output image
                bool hasAlpha = (image.getType() == Image::Type::RGBA);
                auto result = ImageFactory::createColor(dstWidth, dstHeight, hasAlpha);
                if (!result) {
                    return makeErrorResult<std::unique_ptr<Image>>(result.error().code(),
                                                                   result.error().message());
                }

                auto& output = static_cast<ColorImage&>(*result.value());
                const auto& input = static_cast<const ColorImage&>(image);

                // Apply rotation with interpolation
                for (int y = 0; y < dstHeight; ++y) {
                    for (int x = 0; x < dstWidth; ++x) {
                        // Get source coordinates
                        auto [srcX, srcY] = mappingFunc(x, y);

                        // Skip if source coordinates are outside image bounds
                        if (srcX < 0 || srcX >= srcWidth || srcY < 0 || srcY >= srcHeight) {
                            // For transparent images, set fully transparent
                            if (hasAlpha) {
                                output.setPixel(x, y, 0, 0, 0, 0);
                            }
                            continue;
                        }

                        // Get interpolated value
                        uint8_t r, g, b, a;
                        auto interpolateResult = Interpolation::interpolateColor(
                            input, srcX, srcY, r, g, b, a, m_method);

                        if (!interpolateResult) {
                            return makeErrorResult<std::unique_ptr<Image>>(
                                interpolateResult.error().code(),
                                interpolateResult.error().message());
                        }

                        // Set output pixel
                        auto setResult = output.setPixel(x, y, r, g, b, a);
                        if (!setResult) {
                            return makeErrorResult<std::unique_ptr<Image>>(
                                setResult.error().code(), setResult.error().message());
                        }
                    }
                }

                outputImage = std::move(result.value());
                break;
            }

            case Image::Type::Binary:
            default:
                return makeErrorResult<std::unique_ptr<Image>>(
                    ErrorCode::UnsupportedFormat,
                    std::format("Unsupported image type for rotation: {}",
                                static_cast<int>(image.getType())));
        }

        return makeSuccessResult(std::move(outputImage));
    } catch (const std::exception& e) {
        return makeErrorResult<std::unique_ptr<Image>>(
            ErrorCode::ProcessingFailed, std::format("Rotation failed: {}", e.what()));
    }
}

std::string_view RotateTransform::getName() const {
    return "RotateTransform";
}

float RotateTransform::getAngle() const noexcept {
    return m_angle;
}

RotationCenter RotateTransform::getCenter() const noexcept {
    return m_centerType;
}

float RotateTransform::getCenterX() const noexcept {
    return m_centerX;
}

float RotateTransform::getCenterY() const noexcept {
    return m_centerY;
}

InterpolationMethod RotateTransform::getMethod() const noexcept {
    return m_method;
}

bool RotateTransform::isResizeOutput() const noexcept {
    return m_resizeOutput;
}

std::pair<int, int> RotateTransform::calculateRotatedDimensions(int width,
                                                                int height,
                                                                float angleRadians) {
    // Calculate the rotated dimensions of a rectangle
    // This uses the fact that a rotated rectangle's bounding box has:
    // width = |w*cos(a)| + |h*sin(a)|
    // height = |w*sin(a)| + |h*cos(a)|

    float cosA = std::abs(std::cos(angleRadians));
    float sinA = std::abs(std::sin(angleRadians));

    int newWidth = static_cast<int>(std::ceil(width * cosA + height * sinA));
    int newHeight = static_cast<int>(std::ceil(width * sinA + height * cosA));

    return {newWidth, newHeight};
}

std::function<std::pair<float, float>(float, float, int, int, int, int)>
RotateTransform::createRotationMapping(float angle, float centerX, float centerY) {
    return [angle, centerX, centerY](
               float normX, float normY, int srcWidth, int srcHeight, int dstWidth, int dstHeight)
               -> std::pair<float, float> {
        // Convert from normalized destination coordinates to pixel coordinates
        float dstX = normX * (dstWidth - 1);
        float dstY = normY * (dstHeight - 1);

        // Translate destination coordinates to rotation center
        float transX = dstX - dstWidth / 2.0f;
        float transY = dstY - dstHeight / 2.0f;

        // Apply rotation (counter-clockwise)
        float cosA = std::cos(angle);
        float sinA = std::sin(angle);
        float rotX = transX * cosA - transY * sinA;
        float rotY = transX * sinA + transY * cosA;

        // Translate back to source image coordinates
        float srcX = rotX + centerX;
        float srcY = rotY + centerY;

        // Convert to normalized source coordinates
        float normSrcX = srcX / (srcWidth - 1);
        float normSrcY = srcY / (srcHeight - 1);

        return {normSrcX, normSrcY};
    };
}

}  // namespace DIPAL
