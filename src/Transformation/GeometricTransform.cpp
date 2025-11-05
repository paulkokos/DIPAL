// src/Transformation/GeometricTransform.cpp
#include "../../include/DIPAL/Transformation/GeometricTransform.hpp"

#include "../../include/DIPAL/Image/ColorImage.hpp"
#include "../../include/DIPAL/Image/GrayscaleImage.hpp"
#include "../../include/DIPAL/Image/ImageFactory.hpp"
#include "../../include/DIPAL/Transformation/Interpolation.hpp"

#include <algorithm>
#include <cmath>
#include <format>

namespace DIPAL {

GeometricTransform::GeometricTransform(
    int width,
    int height,
    std::function<std::pair<float, float>(float, float)> mappingFunc,
    InterpolationMethod method)
    : m_width(width), m_height(height), m_mappingFunc(std::move(mappingFunc)), m_method(method) {
    if (width <= 0 || height <= 0) {
        throw std::invalid_argument(std::format("Invalid dimensions: {}x{}", width, height));
    }

    if (!m_mappingFunc) {
        throw std::invalid_argument("Mapping function cannot be null");
    }
}

GeometricTransform GeometricTransform::polarToCartesian(int outputWidth,
                                                        int outputHeight,
                                                        float centerX,
                                                        float centerY,
                                                        InterpolationMethod method) {
    // Creates a mapping where the output is a "polar unwrapped" version of the input
    // The horizontal axis corresponds to the angle, and the vertical axis to the radius

    auto mapping = [centerX, centerY](
                       [[maybe_unused]] float normX,
                       [[maybe_unused]] float normY) -> std::pair<float, float> {
        // Convert normalized coordinates [0,1] to polar coordinates
        float angle = normX * 2.0f * M_PI;  // Map X from [0,1] to [0,2π]
        float radius = normY;               // Map Y from [0,1] to [0,1]

        // Convert polar coordinates to Cartesian coordinates
        float x = centerX + radius * std::cos(angle);
        float y = centerY + radius * std::sin(angle);

        return {x, y};
    };

    return GeometricTransform(outputWidth, outputHeight, mapping, method);
}

GeometricTransform GeometricTransform::cartesianToPolar(int outputWidth,
                                                        int outputHeight,
                                                        float centerX,
                                                        float centerY,
                                                        InterpolationMethod method) {
    // Creates a mapping where the input is converted to polar coordinates
    // The horizontal axis corresponds to the angle, and the vertical axis to the radius

    auto mapping = [centerX, centerY](float normX, float normY) -> std::pair<float, float> {
        // Convert normalized coordinates [0,1] to angle and radius
        float angle = normX * 2.0f * M_PI;
        float radius = normY;

        // Convert angle and radius to source image coordinates
        float srcX = centerX + std::cos(angle) * radius;
        float srcY = centerY + std::sin(angle) * radius;

        return {srcX, srcY};
    };

    return GeometricTransform(outputWidth, outputHeight, mapping, method);
}

GeometricTransform GeometricTransform::barrelDistortion(float strength,
                                                        InterpolationMethod method) {
    // Creates a barrel/pincushion distortion mapping
    // Positive strength values create barrel distortion
    // Negative strength values create pincushion distortion

    auto mapping = [strength](float normX, float normY) -> std::pair<float, float> {
        // Map from [0,1] to [-1,1] range (centered at origin)
        float x = normX * 2.0f - 1.0f;
        float y = normY * 2.0f - 1.0f;

        // Calculate distance from center
        float r = std::sqrt(x * x + y * y);

        // Apply distortion
        float distortion = 1.0f + strength * r * r;

        // Apply the distortion to the coordinates
        float newX = x * distortion;
        float newY = y * distortion;

        // Map back to [0,1] range
        return {(newX + 1.0f) * 0.5f, (newY + 1.0f) * 0.5f};
    };

    // Use same input/output size since we're just distorting
    return GeometricTransform(0, 0, mapping, method);
}

GeometricTransform GeometricTransform::perspective(
    const std::array<std::pair<float, float>, 4>& srcQuad,
    const std::array<std::pair<float, float>, 4>& dstQuad,
    int outputWidth,
    int outputHeight,
    InterpolationMethod method) {
    // Calculate the homography matrix from dstQuad to srcQuad
    // This is a complex calculation involving solving a system of linear equations
    // We'll use a simplified direct approach for this implementation

    // For a proper implementation, we would solve the matrix equation:
    // [x1 y1 1 0 0 0 -x1*x1' -y1*x1']   [h11]   [x1']
    // [x2 y2 1 0 0 0 -x2*x2' -y2*x2']   [h12]   [x2']
    // [x3 y3 1 0 0 0 -x3*x3' -y3*x3']   [h13]   [x3']
    // [x4 y4 1 0 0 0 -x4*x4' -y4*x4'] * [h21] = [x4']
    // [0 0 0 x1 y1 1 -x1*y1' -y1*y1']   [h22]   [y1']
    // [0 0 0 x2 y2 1 -x2*y2' -y2*y2']   [h23]   [y2']
    // [0 0 0 x3 y3 1 -x3*y3' -y3*y3']   [h31]   [y3']
    // [0 0 0 x4 y4 1 -x4*y4' -y4*y4']   [h32]   [y4']
    //                                    [h33]   [1]

    // For simplicity, we'll use a mapping function that directly computes the transformation
    // using bilinear interpolation between the four corners.

    auto mapping = [srcQuad](float normX, float normY) -> std::pair<float, float> {
        // Bilinear interpolation between the four corners
        float u = normX;
        float v = normY;

        // Interpolate x-coordinate
        float x = (1 - u) * (1 - v) * srcQuad[0].first + u * (1 - v) * srcQuad[1].first +
                  (1 - u) * v * srcQuad[2].first + u * v * srcQuad[3].first;

        // Interpolate y-coordinate
        float y = (1 - u) * (1 - v) * srcQuad[0].second + u * (1 - v) * srcQuad[1].second +
                  (1 - u) * v * srcQuad[2].second + u * v * srcQuad[3].second;

        return {x, y};
    };

    return GeometricTransform(outputWidth, outputHeight, mapping, method);
}

GeometricTransform GeometricTransform::fishEye(float fovDegrees,
                                               int outputWidth,
                                               int outputHeight,
                                               InterpolationMethod method) {
    // Create a fish-eye lens effect
    float fovRadians = fovDegrees * (M_PI / 180.0f);

    auto mapping = [fovRadians](float normX, float normY) -> std::pair<float, float> {
        // Map from [0,1] to [-1,1] range (centered at origin)
        float x = normX * 2.0f - 1.0f;
        float y = normY * 2.0f - 1.0f;

        // Calculate distance from center and angle
        float r = std::sqrt(x * x + y * y);
        float theta = std::atan2(y, x);

        // Apply fish-eye transformation
        // r' = f * sin(r * θ_max / f)
        // where f is the focal length and θ_max is the field of view
        float focalLength = 1.0f;
        float newR = focalLength * std::sin(r * fovRadians / (2.0f * focalLength));

        // Scale to ensure we use the full range
        newR = newR / std::sin(fovRadians / 2.0f);

        // Convert back to Cartesian coordinates
        float newX = newR * std::cos(theta);
        float newY = newR * std::sin(theta);

        // Map back to [0,1] range
        return {(newX + 1.0f) * 0.5f, (newY + 1.0f) * 0.5f};
    };

    return GeometricTransform(outputWidth, outputHeight, mapping, method);
}

Result<std::unique_ptr<Image>> GeometricTransform::apply(const Image& image) const {
    // Check for empty image
    if (image.isEmpty()) {
        return makeErrorResult<std::unique_ptr<Image>>(
            ErrorCode::InvalidParameter, "Cannot apply geometric transform to an empty image");
    }

    try {
        // Determine source image dimensions
        int srcWidth = image.getWidth();
        int srcHeight = image.getHeight();

        // Determine output dimensions
        int dstWidth = (m_width > 0) ? m_width : srcWidth;
        int dstHeight = (m_height > 0) ? m_height : srcHeight;

        // Create pixel mapping function
        auto pixelMapping = createPixelMapping(srcWidth, srcHeight);

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

                // Apply transformation with interpolation
                for (int y = 0; y < dstHeight; ++y) {
                    for (int x = 0; x < dstWidth; ++x) {
                        // Get source coordinates
                        auto [srcX, srcY] = pixelMapping(x, y);

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

                // Apply transformation with interpolation
                for (int y = 0; y < dstHeight; ++y) {
                    for (int x = 0; x < dstWidth; ++x) {
                        // Get source coordinates
                        auto [srcX, srcY] = pixelMapping(x, y);

                        // Skip if source coordinates are outside image bounds
                        if (srcX < 0 || srcX >= srcWidth || srcY < 0 || srcY >= srcHeight) {
                            // For transparent images, set fully transparent
                            if (hasAlpha) {
                                auto setResult = output.setPixel(x, y, 0, 0, 0, 0);
                                if (!setResult) {
                                    return makeErrorResult<std::unique_ptr<Image>>(
                                        setResult.error().code(), setResult.error().message());
                                }
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
                    std::format("Unsupported image type for geometric transform: {}",
                                static_cast<int>(image.getType())));
        }

        return makeSuccessResult(std::move(outputImage));
    } catch (const std::exception& e) {
        return makeErrorResult<std::unique_ptr<Image>>(
            ErrorCode::ProcessingFailed, std::format("Geometric transform failed: {}", e.what()));
    }
}

std::string_view GeometricTransform::getName() const {
    return "GeometricTransform";
}

int GeometricTransform::getWidth() const noexcept {
    return m_width;
}

int GeometricTransform::getHeight() const noexcept {
    return m_height;
}

InterpolationMethod GeometricTransform::getMethod() const noexcept {
    return m_method;
}

std::function<std::pair<float, float>(int, int)> GeometricTransform::createPixelMapping(
    int srcWidth,
    int srcHeight) const {
    return [this, srcWidth, srcHeight](int x, int y) -> std::pair<float, float> {
        // Convert pixel coordinates to normalized coordinates [0,1]
        float normX = static_cast<float>(x) / (m_width - 1);
        float normY = static_cast<float>(y) / (m_height - 1);

        // Apply the mapping function to get normalized source coordinates
        auto [normSrcX, normSrcY] = m_mappingFunc(normX, normY);

        // Convert back to pixel coordinates in source image
        float srcX = normSrcX * (srcWidth - 1);
        float srcY = normSrcY * (srcHeight - 1);

        return {srcX, srcY};
    };
}

}  // namespace DIPAL
