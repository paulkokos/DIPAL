// src/Transformation/Interpolation.cpp
#include "../../include/DIPAL/Transformation/Interpolation.hpp"

#include <algorithm>
#include <cmath>
#include <format>

namespace DIPAL {

Result<uint8_t> Interpolation::interpolateGray(const GrayscaleImage& image,
                                               float x,
                                               float y,
                                               InterpolationMethod method) {
    try {
        switch (method) {
            case InterpolationMethod::NearestNeighbor:
                return nearestNeighborGray(image, x, y);
            case InterpolationMethod::Bilinear:
                return bilinearGray(image, x, y);
            case InterpolationMethod::Bicubic:
                return bicubicGray(image, x, y);
            default:
                return makeErrorResult<uint8_t>(
                    ErrorCode::InvalidParameter,
                    std::format("Unknown interpolation method: {}", static_cast<int>(method)));
        }
    } catch (const std::exception& e) {
        return makeErrorResult<uint8_t>(ErrorCode::ProcessingFailed,
                                        std::format("Interpolation failed: {}", e.what()));
    }
}

VoidResult Interpolation::interpolateColor(const ColorImage& image,
                                           float x,
                                           float y,
                                           uint8_t& r,
                                           uint8_t& g,
                                           uint8_t& b,
                                           uint8_t& a,
                                           InterpolationMethod method) {
    try {
        switch (method) {
            case InterpolationMethod::NearestNeighbor:
                return nearestNeighborColor(image, x, y, r, g, b, a);
            case InterpolationMethod::Bilinear:
                return bilinearColor(image, x, y, r, g, b, a);
            case InterpolationMethod::Bicubic:
                return bicubicColor(image, x, y, r, g, b, a);
            default:
                return makeVoidErrorResult(
                    ErrorCode::InvalidParameter,
                    std::format("Unknown interpolation method: {}", static_cast<int>(method)));
        }
    } catch (const std::exception& e) {
        return makeVoidErrorResult(ErrorCode::ProcessingFailed,
                                   std::format("Interpolation failed: {}", e.what()));
    }
}

std::function<std::pair<float, float>(int, int)> Interpolation::createMapping(
    int srcWidth,
    int srcHeight,
    int dstWidth,
    int dstHeight,
    std::function<std::pair<float, float>(float, float, int, int, int, int)> transformFunc) {
    return [=](int dstX, int dstY) -> std::pair<float, float> {
        // Normalize destination coordinates to [0, 1] range
        float normX = static_cast<float>(dstX) / (dstWidth - 1);
        float normY = static_cast<float>(dstY) / (dstHeight - 1);

        // Apply the transformation function
        auto [srcNormX, srcNormY] =
            transformFunc(normX, normY, srcWidth, srcHeight, dstWidth, dstHeight);

        // Convert back to pixel coordinates in source space
        float srcX = srcNormX * (srcWidth - 1);
        float srcY = srcNormY * (srcHeight - 1);

        return {srcX, srcY};
    };
}

Result<uint8_t> Interpolation::nearestNeighborGray(const GrayscaleImage& image, float x, float y) {
    // Round to nearest integer coordinates
    int roundedX = static_cast<int>(std::round(x));
    int roundedY = static_cast<int>(std::round(y));

    // Check bounds
    if (roundedX < 0 || roundedX >= image.getWidth() || roundedY < 0 ||
        roundedY >= image.getHeight()) {
        // Return black for out-of-bounds
        return makeSuccessResult(static_cast<uint8_t>(0));
    }

    // Get the nearest pixel value
    return image.getPixel(roundedX, roundedY);
}

Result<uint8_t> Interpolation::bilinearGray(const GrayscaleImage& image, float x, float y) {
    // Get integer and fractional parts
    int x1 = static_cast<int>(std::floor(x));
    int y1 = static_cast<int>(std::floor(y));
    int x2 = x1 + 1;
    int y2 = y1 + 1;

    float fracX = x - x1;
    float fracY = y - y1;

    // Get the four surrounding pixel values (with bounds checking)
    uint8_t p11 = getPixelSafe(image, x1, y1);
    uint8_t p21 = getPixelSafe(image, x2, y1);
    uint8_t p12 = getPixelSafe(image, x1, y2);
    uint8_t p22 = getPixelSafe(image, x2, y2);

    // Perform bilinear interpolation
    float value = p11 * (1 - fracX) * (1 - fracY) + p21 * fracX * (1 - fracY) +
                  p12 * (1 - fracX) * fracY + p22 * fracX * fracY;

    // Round to nearest integer and clamp to uint8_t range
    uint8_t result = static_cast<uint8_t>(std::clamp(std::round(value), 0.0f, 255.0f));

    return makeSuccessResult(result);
}

Result<uint8_t> Interpolation::bicubicGray(const GrayscaleImage& image, float x, float y) {
    // Get integer and fractional parts
    int ix = static_cast<int>(std::floor(x));
    int iy = static_cast<int>(std::floor(y));
    float fracX = x - ix;
    float fracY = y - iy;

    // Prepare a 4x4 grid of pixels centered around the point
    std::array<std::array<float, 4>, 4> grid;

    for (int j = 0; j < 4; j++) {
        for (int i = 0; i < 4; i++) {
            grid[j][i] = static_cast<float>(getPixelSafe(image, ix - 1 + i, iy - 1 + j));
        }
    }

    // Perform bicubic interpolation
    float value = bicubicInterpolate(grid, fracX, fracY);

    // Clamp to valid pixel value range
    uint8_t result = static_cast<uint8_t>(std::clamp(std::round(value), 0.0f, 255.0f));

    return makeSuccessResult(result);
}

VoidResult Interpolation::nearestNeighborColor(const ColorImage& image,
                                               float x,
                                               float y,
                                               uint8_t& r,
                                               uint8_t& g,
                                               uint8_t& b,
                                               uint8_t& a) {
    // Round to nearest integer coordinates
    int roundedX = static_cast<int>(std::round(x));
    int roundedY = static_cast<int>(std::round(y));

    // Check bounds
    if (roundedX < 0 || roundedX >= image.getWidth() || roundedY < 0 ||
        roundedY >= image.getHeight()) {
        // Return transparent black for out-of-bounds
        r = g = b = 0;
        a = 0;
        return makeVoidSuccessResult();
    }

    // Get the nearest pixel value
    return image.getPixel(roundedX, roundedY, r, g, b, a);
}

VoidResult Interpolation::bilinearColor(const ColorImage& image,
                                        float x,
                                        float y,
                                        uint8_t& r,
                                        uint8_t& g,
                                        uint8_t& b,
                                        uint8_t& a) {
    // Get integer and fractional parts
    int x1 = static_cast<int>(std::floor(x));
    int y1 = static_cast<int>(std::floor(y));
    int x2 = x1 + 1;
    int y2 = y1 + 1;

    float fracX = x - x1;
    float fracY = y - y1;

    // Get the four surrounding pixel values
    uint8_t r11, g11, b11, a11;
    uint8_t r21, g21, b21, a21;
    uint8_t r12, g12, b12, a12;
    uint8_t r22, g22, b22, a22;

    getPixelSafe(image, x1, y1, r11, g11, b11, a11);
    getPixelSafe(image, x2, y1, r21, g21, b21, a21);
    getPixelSafe(image, x1, y2, r12, g12, b12, a12);
    getPixelSafe(image, x2, y2, r22, g22, b22, a22);

    // Perform bilinear interpolation for each channel
    float rValue = r11 * (1 - fracX) * (1 - fracY) + r21 * fracX * (1 - fracY) +
                   r12 * (1 - fracX) * fracY + r22 * fracX * fracY;

    float gValue = g11 * (1 - fracX) * (1 - fracY) + g21 * fracX * (1 - fracY) +
                   g12 * (1 - fracX) * fracY + g22 * fracX * fracY;

    float bValue = b11 * (1 - fracX) * (1 - fracY) + b21 * fracX * (1 - fracY) +
                   b12 * (1 - fracX) * fracY + b22 * fracX * fracY;

    float aValue = a11 * (1 - fracX) * (1 - fracY) + a21 * fracX * (1 - fracY) +
                   a12 * (1 - fracX) * fracY + a22 * fracX * fracY;

    // Round to nearest integer and clamp to uint8_t range
    r = static_cast<uint8_t>(std::clamp(std::round(rValue), 0.0f, 255.0f));
    g = static_cast<uint8_t>(std::clamp(std::round(gValue), 0.0f, 255.0f));
    b = static_cast<uint8_t>(std::clamp(std::round(bValue), 0.0f, 255.0f));
    a = static_cast<uint8_t>(std::clamp(std::round(aValue), 0.0f, 255.0f));

    return makeVoidSuccessResult();
}

VoidResult Interpolation::bicubicColor(const ColorImage& image,
                                       float x,
                                       float y,
                                       uint8_t& r,
                                       uint8_t& g,
                                       uint8_t& b,
                                       uint8_t& a) {
    // Get integer and fractional parts
    int ix = static_cast<int>(std::floor(x));
    int iy = static_cast<int>(std::floor(y));
    float fracX = x - ix;
    float fracY = y - iy;

    // Prepare 4x4 grids for each channel
    std::array<std::array<float, 4>, 4> rGrid;
    std::array<std::array<float, 4>, 4> gGrid;
    std::array<std::array<float, 4>, 4> bGrid;
    std::array<std::array<float, 4>, 4> aGrid;

    // Fill the grids with pixel values
    for (int j = 0; j < 4; j++) {
        for (int i = 0; i < 4; i++) {
            uint8_t rVal, gVal, bVal, aVal;
            getPixelSafe(image, ix - 1 + i, iy - 1 + j, rVal, gVal, bVal, aVal);

            rGrid[j][i] = static_cast<float>(rVal);
            gGrid[j][i] = static_cast<float>(gVal);
            bGrid[j][i] = static_cast<float>(bVal);
            aGrid[j][i] = static_cast<float>(aVal);
        }
    }

    // Perform bicubic interpolation for each channel
    float rValue = bicubicInterpolate(rGrid, fracX, fracY);
    float gValue = bicubicInterpolate(gGrid, fracX, fracY);
    float bValue = bicubicInterpolate(bGrid, fracX, fracY);
    float aValue = bicubicInterpolate(aGrid, fracX, fracY);

    // Clamp to valid pixel value range
    r = static_cast<uint8_t>(std::clamp(std::round(rValue), 0.0f, 255.0f));
    g = static_cast<uint8_t>(std::clamp(std::round(gValue), 0.0f, 255.0f));
    b = static_cast<uint8_t>(std::clamp(std::round(bValue), 0.0f, 255.0f));
    a = static_cast<uint8_t>(std::clamp(std::round(aValue), 0.0f, 255.0f));

    return makeVoidSuccessResult();
}

float Interpolation::cubicInterpolate(const std::array<float, 4>& p, float t) {
    // Cubic interpolation coefficients (Catmull-Rom spline)
    float a = -0.5f * p[0] + 1.5f * p[1] - 1.5f * p[2] + 0.5f * p[3];
    float b = p[0] - 2.5f * p[1] + 2.0f * p[2] - 0.5f * p[3];
    float c = -0.5f * p[0] + 0.5f * p[2];
    float d = p[1];

    // Evaluate cubic polynomial
    return a * t * t * t + b * t * t + c * t + d;
}

float Interpolation::bicubicInterpolate(const std::array<std::array<float, 4>, 4>& p,
                                        float x,
                                        float y) {
    // Interpolate along rows first
    std::array<float, 4> rowInterpolations;
    for (int i = 0; i < 4; i++) {
        rowInterpolations[i] = cubicInterpolate(p[i], x);
    }

    // Then interpolate along the column
    return cubicInterpolate(rowInterpolations, y);
}

uint8_t Interpolation::getPixelSafe(const GrayscaleImage& image, int x, int y) {
    // Check bounds
    if (x < 0 || x >= image.getWidth() || y < 0 || y >= image.getHeight()) {
        return 0;  // Return black for out-of-bounds coordinates
    }

    // Get pixel value
    auto result = image.getPixel(x, y);
    if (!result) {
        return 0;  // Return black on error
    }

    return result.value();
}

void Interpolation::getPixelSafe(const ColorImage& image,
                                 int x,
                                 int y,
                                 uint8_t& r,
                                 uint8_t& g,
                                 uint8_t& b,
                                 uint8_t& a) {
    // Check bounds
    if (x < 0 || x >= image.getWidth() || y < 0 || y >= image.getHeight()) {
        r = g = b = a = 0;  // Return transparent black for out-of-bounds
        return;
    }

    // Get pixel value
    auto result = image.getPixel(x, y, r, g, b, a);
    if (!result) {
        r = g = b = a = 0;  // Return transparent black on error
    }
}

}  // namespace DIPAL
