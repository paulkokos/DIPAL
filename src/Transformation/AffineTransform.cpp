// src/Transformation/AffineTransform.cpp
#include "../../include/DIPAL/Transformation/AffineTransform.hpp"

#include "../../include/DIPAL/Image/ColorImage.hpp"
#include "../../include/DIPAL/Image/GrayscaleImage.hpp"
#include "../../include/DIPAL/Image/ImageFactory.hpp"
#include "../../include/DIPAL/Transformation/Interpolation.hpp"

#include <algorithm>
#include <cmath>
#include <format>
#include <limits>

namespace DIPAL {

AffineTransform::AffineTransform(const std::array<float, 6>& matrix,
                                 InterpolationMethod method,
                                 int outputWidth,
                                 int outputHeight)
    : m_matrix(matrix),
      m_method(method),
      m_outputWidth(outputWidth),
      m_outputHeight(outputHeight) {}

AffineTransform AffineTransform::identity() {
    return AffineTransform({1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f});
}

AffineTransform AffineTransform::scaling(float scaleX, float scaleY, InterpolationMethod method) {
    return AffineTransform({scaleX, 0.0f, 0.0f, 0.0f, scaleY, 0.0f}, method);
}

AffineTransform AffineTransform::rotation(float angleDegrees,
                                          float centerX,
                                          float centerY,
                                          InterpolationMethod method) {
    float angleRadians = angleDegrees * (M_PI / 180.0f);
    float cosA = std::cos(angleRadians);
    float sinA = std::sin(angleRadians);

    // Rotation around a point (cx,cy) is:
    // 1. Translate so origin is at (cx,cy)
    // 2. Rotate
    // 3. Translate back

    // This gives:
    // x' = cosA * (x - cx) - sinA * (y - cy) + cx
    // y' = sinA * (x - cx) + cosA * (y - cy) + cy

    // Expand to get matrix form:
    // x' = cosA * x - cosA * cx - sinA * y + sinA * cy + cx
    // y' = sinA * x - sinA * cx + cosA * y - cosA * cy + cy

    // Rearrange:
    // x' = cosA * x - sinA * y + (- cosA * cx + sinA * cy + cx)
    // y' = sinA * x + cosA * y + (- sinA * cx - cosA * cy + cy)

    // Matrix form [a, b, c, d, e, f]:
    // a = cosA, b = -sinA, c = -cosA * cx + sinA * cy + cx
    // d = sinA, e = cosA,  f = -sinA * cx - cosA * cy + cy

    float a = cosA;
    float b = -sinA;
    float c = -cosA * centerX + sinA * centerY + centerX;
    float d = sinA;
    float e = cosA;
    float f = -sinA * centerX - cosA * centerY + centerY;

    return AffineTransform({a, b, c, d, e, f}, method);
}

AffineTransform AffineTransform::translation(float dx, float dy) {
    return AffineTransform({1.0f, 0.0f, dx, 0.0f, 1.0f, dy});
}

AffineTransform AffineTransform::shearing(float shearX, float shearY, InterpolationMethod method) {
    return AffineTransform({1.0f, shearX, 0.0f, shearY, 1.0f, 0.0f}, method);
}

Result<std::unique_ptr<Image>> AffineTransform::apply(const Image& image) const {
    // Check for empty image
    if (image.isEmpty()) {
        return makeErrorResult<std::unique_ptr<Image>>(
            ErrorCode::InvalidParameter, "Cannot apply affine transform to an empty image");
    }

    try {
        // Determine source image dimensions
        int srcWidth = image.getWidth();
        int srcHeight = image.getHeight();

        // Calculate output dimensions if not specified
        int dstWidth, dstHeight;
        if (m_outputWidth <= 0 || m_outputHeight <= 0) {
            auto [width, height] = calculateBounds(srcWidth, srcHeight);
            dstWidth = (m_outputWidth <= 0) ? width : m_outputWidth;
            dstHeight = (m_outputHeight <= 0) ? height : m_outputHeight;
        } else {
            dstWidth = m_outputWidth;
            dstHeight = m_outputHeight;
        }

        // Create transformation mapping function
        auto mappingFunc = Interpolation::createMapping(
            srcWidth, srcHeight, dstWidth, dstHeight, createMappingFunction());

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

                // Apply transformation with interpolation
                for (int y = 0; y < dstHeight; ++y) {
                    for (int x = 0; x < dstWidth; ++x) {
                        // Get source coordinates
                        auto [srcX, srcY] = mappingFunc(x, y);

                        // Skip if source coordinates are outside image bounds
                        if (srcX < 0 || srcX >= srcWidth || srcY < 0 || srcY >= srcHeight) {
                            // For transparent images, set fully transparent
                            if (hasAlpha) {
                                    
                                   if (auto result = output.setPixel(x, y, 0, 0, 0, 0); !result) {
    return makeErrorResult<std::unique_ptr<Image>>(
        ErrorCode::ProcessingFailed,
        std::format("Failed to set pixel at ({}, {}): {}", x, y, result.error().message())
    );
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
                    std::format("Unsupported image type for affine transform: {}",
                                static_cast<int>(image.getType())));
        }

        return makeSuccessResult(std::move(outputImage));
    } catch (const std::exception& e) {
        return makeErrorResult<std::unique_ptr<Image>>(
            ErrorCode::ProcessingFailed, std::format("Affine transform failed: {}", e.what()));
    }
}

std::string_view AffineTransform::getName() const {
    return "AffineTransform";
}

const std::array<float, 6>& AffineTransform::getMatrix() const noexcept {
    return m_matrix;
}

InterpolationMethod AffineTransform::getMethod() const noexcept {
    return m_method;
}

int AffineTransform::getOutputWidth() const noexcept {
    return m_outputWidth;
}

int AffineTransform::getOutputHeight() const noexcept {
    return m_outputHeight;
}

AffineTransform AffineTransform::compose(const AffineTransform& other) const {
    // This composes transformations: first apply 'this', then 'other'
    // Let's call 'this' matrix [a,b,c,d,e,f] and 'other' matrix [a',b',c',d',e',f']
    // For 2D points:
    // this:  (x, y) -> (a*x + b*y + c, d*x + e*y + f)
    // other: (x, y) -> (a'*x + b'*y + c', d'*x + e'*y + f')
    //
    // Composition:
    // other(this(x, y)) = other(a*x + b*y + c, d*x + e*y + f)
    //                   = (a'*(a*x + b*y + c) + b'*(d*x + e*y + f) + c',
    //                      d'*(a*x + b*y + c) + e'*(d*x + e*y + f) + f')
    //                   = (a'*a*x + a'*b*y + a'*c + b'*d*x + b'*e*y + b'*f + c',
    //                      d'*a*x + d'*b*y + d'*c + e'*d*x + e'*e*y + e'*f + f')
    //                   = ((a'*a + b'*d)*x + (a'*b + b'*e)*y + (a'*c + b'*f + c'),
    //                      (d'*a + e'*d)*x + (d'*b + e'*e)*y + (d'*c + e'*f + f'))
    //
    // So the composed matrix [a",b",c",d",e",f"] is:
    // a" = a'*a + b'*d
    // b" = a'*b + b'*e
    // c" = a'*c + b'*f + c'
    // d" = d'*a + e'*d
    // e" = d'*b + e'*e
    // f" = d'*c + e'*f + f'

    const auto& m1 = m_matrix;
    const auto& m2 = other.getMatrix();

    float a = m2[0] * m1[0] + m2[1] * m1[3];
    float b = m2[0] * m1[1] + m2[1] * m1[4];
    float c = m2[0] * m1[2] + m2[1] * m1[5] + m2[2];
    float d = m2[3] * m1[0] + m2[4] * m1[3];
    float e = m2[3] * m1[1] + m2[4] * m1[4];
    float f = m2[3] * m1[2] + m2[4] * m1[5] + m2[5];

    // Use the most precise interpolation method from either transform
    InterpolationMethod method = (m_method > other.getMethod()) ? m_method : other.getMethod();

    // Keep output size if specified, otherwise auto-calculate
    int outputWidth = (m_outputWidth > 0) ? m_outputWidth : other.getOutputWidth();
    int outputHeight = (m_outputHeight > 0) ? m_outputHeight : other.getOutputHeight();

    return AffineTransform({a, b, c, d, e, f}, method, outputWidth, outputHeight);
}

Result<AffineTransform> AffineTransform::inverse() const {
    // For an affine transform [a,b,c,d,e,f], the inverse is:
    //
    // [ a b c ]^-1   [ e  -b  b*f-c*e ]
    // [ d e f ]    = [-d   a  c*d-a*f ] / det
    // [ 0 0 1 ]      [ 0   0     det  ]
    //
    // Where det = a*e - b*d
    //
    // So the inverse matrix [a',b',c',d',e',f'] is:
    // a' = e / det
    // b' = -b / det
    // c' = (b*f - c*e) / det
    // d' = -d / det
    // e' = a / det
    // f' = (c*d - a*f) / det

    float a = m_matrix[0];
    float b = m_matrix[1];
    float c = m_matrix[2];
    float d = m_matrix[3];
    float e = m_matrix[4];
    float f = m_matrix[5];

    float det = a * e - b * d;

    // Check if the matrix is invertible (det != 0)
    const float eps = 1e-6f;
    if (std::abs(det) < eps) {
        return makeErrorResult<AffineTransform>(
            ErrorCode::ProcessingFailed, "Cannot invert affine transformation: matrix is singular");
    }

    float invDet = 1.0f / det;

    float a_inv = e * invDet;
    float b_inv = -b * invDet;
    float c_inv = (b * f - c * e) * invDet;
    float d_inv = -d * invDet;
    float e_inv = a * invDet;
    float f_inv = (c * d - a * f) * invDet;

    return makeSuccessResult(AffineTransform(
        {a_inv, b_inv, c_inv, d_inv, e_inv, f_inv}, m_method, m_outputWidth, m_outputHeight));
}

std::pair<int, int> AffineTransform::calculateBounds(int width, int height) const {
    // Transform the four corners of the image to find the bounding box
    float a = m_matrix[0];
    float b = m_matrix[1];
    float c = m_matrix[2];
    float d = m_matrix[3];
    float e = m_matrix[4];
    float f = m_matrix[5];

    // Calculate transformed corners:
    // (0,0), (width-1,0), (0,height-1), (width-1,height-1)
    float corners[4][2] = {
        {c, f},                                        // (0,0) transforms to (c, f)
        {a * (width - 1) + c, d * (width - 1) + f},    // (width-1,0)
        {b * (height - 1) + c, e * (height - 1) + f},  // (0,height-1)
        {a * (width - 1) + b * (height - 1) + c,
         d * (width - 1) + e * (height - 1) + f}  // (width-1,height-1)
    };

    // Find min/max x and y to determine bounds
    float minX = std::numeric_limits<float>::max();
    float minY = std::numeric_limits<float>::max();
    float maxX = std::numeric_limits<float>::lowest();
    float maxY = std::numeric_limits<float>::lowest();

    for (const auto& corner : corners) {
        minX = std::min(minX, corner[0]);
        minY = std::min(minY, corner[1]);
        maxX = std::max(maxX, corner[0]);
        maxY = std::max(maxY, corner[1]);
    }

    // Calculate dimensions, ensuring at least 1x1
    int outWidth = std::max(1, static_cast<int>(std::ceil(maxX - minX + 1)));
    int outHeight = std::max(1, static_cast<int>(std::ceil(maxY - minY + 1)));

    return {outWidth, outHeight};
}

std::function<std::pair<float, float>(float, float, int, int, int, int)>
AffineTransform::createMappingFunction() const {
    return
        [this](float normX, float normY, int srcWidth, int srcHeight, int dstWidth, int dstHeight)
            -> std::pair<float, float> {
            // Convert from normalized destination coordinates to pixel coordinates
            float dstX = normX * (dstWidth - 1);
            float dstY = normY * (dstHeight - 1);

            // Apply inverse transform to get source coordinates
            // For the affine transform [a,b,c,d,e,f], we have:
            // dstX = a * srcX + b * srcY + c
            // dstY = d * srcX + e * srcY + f
            //
            // Solving for srcX, srcY:
            // srcX = (e * (dstX - c) - b * (dstY - f)) / (a * e - b * d)
            // srcY = (a * (dstY - f) - d * (dstX - c)) / (a * e - b * d)

            float a = m_matrix[0];
            float b = m_matrix[1];
            float c = m_matrix[2];
            float d = m_matrix[3];
            float e = m_matrix[4];
            float f = m_matrix[5];

            float det = a * e - b * d;

            // Check for singular matrix (can't invert)
            if (std::abs(det) < 1e-6f) {
                // Return a coordinate outside the image to skip this pixel
                return {-1.0f, -1.0f};
            }

            float srcX = (e * (dstX - c) - b * (dstY - f)) / det;
            float srcY = (a * (dstY - f) - d * (dstX - c)) / det;

            // Convert to normalized source coordinates
            float normSrcX = srcX / (srcWidth - 1);
            float normSrcY = srcY / (srcHeight - 1);

            return {normSrcX, normSrcY};
        };
}

}  // namespace DIPAL
