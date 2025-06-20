// src/Transformation/WarpTransform.cpp
#include "../../include/DIPAL/Transformation/WarpTransform.hpp"

#include "../../include/DIPAL/Image/ColorImage.hpp"
#include "../../include/DIPAL/Image/GrayscaleImage.hpp"
#include "../../include/DIPAL/Image/ImageFactory.hpp"
#include "../../include/DIPAL/Transformation/Interpolation.hpp"

#include <algorithm>
#include <cmath>
#include <format>
#include <numeric>
#include <unordered_map>

namespace DIPAL {

WarpTransform::WarpTransform(const std::vector<Point>& sourcePoints,
                             const std::vector<Point>& destPoints,
                             WarpMethod method,
                             InterpolationMethod interpolation)
    : m_sourcePoints(sourcePoints),
      m_destPoints(destPoints),
      m_warpMethod(method),
      m_interpolationMethod(interpolation) {
    if (sourcePoints.size() != destPoints.size()) {
        throw std::invalid_argument(
            std::format("Source and destination point count mismatch: {} vs {}",
                        sourcePoints.size(),
                        destPoints.size()));
    }

    if (sourcePoints.empty()) {
        throw std::invalid_argument("Control points cannot be empty");
    }
}

WarpTransform WarpTransform::createMeshWarp(int meshWidth,
                                            int meshHeight,
                                            const std::vector<Point>& sourcePoints,
                                            const std::vector<Point>& destPoints,
                                            InterpolationMethod interpolation) {
    // Validate mesh dimensions
    if (meshWidth < 2 || meshHeight < 2) {
        throw std::invalid_argument(
            std::format("Invalid mesh dimensions: {}x{}", meshWidth, meshHeight));
    }

    // Validate control points
    if (sourcePoints.size() != static_cast<size_t>(meshWidth * meshHeight) ||
        destPoints.size() != static_cast<size_t>(meshWidth * meshHeight)) {
        throw std::invalid_argument(
            std::format("Control points count must match mesh size ({}x{}={})",
                        meshWidth,
                        meshHeight,
                        meshWidth * meshHeight));
    }

    return WarpTransform(sourcePoints, destPoints, WarpMethod::MeshWarp, interpolation);
}

WarpTransform WarpTransform::createThinPlateSpline(const std::vector<Point>& sourcePoints,
                                                   const std::vector<Point>& destPoints,
                                                   float stiffness,
                                                   InterpolationMethod interpolation) {
    // Validate stiffness parameter
    if (stiffness < 0.0f) {
        throw std::invalid_argument(std::format("Stiffness must be non-negative: {}", stiffness));
    }

    // Minimum number of control points for TPS
    if (sourcePoints.size() < 3 || destPoints.size() < 3) {
        throw std::invalid_argument(
            "At least 3 control points are required for thin-plate spline warping");
    }

    // We'll store stiffness in an extra control point (as a special marker)
    std::vector<Point> srcPts = sourcePoints;
    std::vector<Point> dstPts = destPoints;

    // Add a special control point to store the stiffness parameter
    // We use a negative coordinate that won't be used in actual warping
    srcPts.push_back({-1, -1});
    dstPts.push_back({static_cast<int>(stiffness * 1000), -1});

    return WarpTransform(srcPts, dstPts, WarpMethod::ThinPlateSpline, interpolation);
}

Result<std::unique_ptr<Image>> WarpTransform::apply(const Image& image) const {
    // Check for empty image
    if (image.isEmpty()) {
        return makeErrorResult<std::unique_ptr<Image>>(
            ErrorCode::InvalidParameter, "Cannot apply warp transform to an empty image");
    }

    try {
        // Apply the appropriate warping algorithm
        switch (m_warpMethod) {
            case WarpMethod::ThinPlateSpline:
                return applyThinPlateSpline(image);
            case WarpMethod::MeshWarp:
                return applyMeshWarp(image);
            case WarpMethod::Triangulation:
                return applyTriangulation(image);
            default:
                return makeErrorResult<std::unique_ptr<Image>>(
                    ErrorCode::InvalidParameter,
                    std::format("Unsupported warp method: {}", static_cast<int>(m_warpMethod)));
        }
    } catch (const std::exception& e) {
        return makeErrorResult<std::unique_ptr<Image>>(
            ErrorCode::ProcessingFailed, std::format("Warp transform failed: {}", e.what()));
    }
}

std::string_view WarpTransform::getName() const {
    return "WarpTransform";
}

const std::vector<Point>& WarpTransform::getSourcePoints() const noexcept {
    return m_sourcePoints;
}

const std::vector<Point>& WarpTransform::getDestPoints() const noexcept {
    return m_destPoints;
}

WarpMethod WarpTransform::getWarpMethod() const noexcept {
    return m_warpMethod;
}

InterpolationMethod WarpTransform::getInterpolationMethod() const noexcept {
    return m_interpolationMethod;
}

Result<std::unique_ptr<Image>> WarpTransform::applyThinPlateSpline(const Image& image) const {
    // Thin-plate spline implementation

    // Check if we have the special marker for stiffness
    float stiffness = 0.0f;
    std::vector<Point> srcPoints = m_sourcePoints;
    std::vector<Point> dstPoints = m_destPoints;

    if (!m_sourcePoints.empty() && m_sourcePoints.back().x == -1 && m_sourcePoints.back().y == -1) {
        // Extract stiffness from the special control point
        stiffness = static_cast<float>(m_destPoints.back().x) / 1000.0f;

        // Remove the special control point
        srcPoints.pop_back();
        dstPoints.pop_back();
    }

    int srcWidth = image.getWidth();
    int srcHeight = image.getHeight();

    // Create output image with same dimensions
    std::unique_ptr<Image> outputImage;

    // In a real implementation, we would compute the TPS coefficients here
    // For this simplified version, we'll implement a basic approximation
    // using a weighted average of control points

    // Calculate bounding box of destination points to determine output size
    int minX = std::numeric_limits<int>::max();
    int minY = std::numeric_limits<int>::max();
    int maxX = std::numeric_limits<int>::min();
    int maxY = std::numeric_limits<int>::min();

    for (const auto& pt : dstPoints) {
        minX = std::min(minX, pt.x);
        minY = std::min(minY, pt.y);
        maxX = std::max(maxX, pt.x);
        maxY = std::max(maxY, pt.y);
    }

    // Ensure dimensions are at least 1x1
    int dstWidth = std::max(1, maxX - minX + 1);
    int dstHeight = std::max(1, maxY - minY + 1);

    // If destination is too small, use source dimensions
    if (dstWidth < 10 || dstHeight < 10) {
        dstWidth = srcWidth;
        dstHeight = srcHeight;
    }

    // Create the output image based on type
    Result<std::unique_ptr<Image>> result;

    switch (image.getType()) {
        case Image::Type::Grayscale:
            result = ImageFactory::createGrayscale(dstWidth, dstHeight);
            break;
        case Image::Type::RGB:
            result = ImageFactory::createColor(dstWidth, dstHeight, false);
            break;
        case Image::Type::RGBA:
            result = ImageFactory::createColor(dstWidth, dstHeight, true);
            break;
        default:
            return makeErrorResult<std::unique_ptr<Image>>(
                ErrorCode::UnsupportedFormat,
                std::format("Unsupported image type for warping: {}",
                            static_cast<int>(image.getType())));
    }

    if (!result) {
        return result;
    }

    outputImage = std::move(result.value());

    // Implement a simplified inverse warping (backward mapping)
    // For each pixel in the destination, find its position in the source

    // Helper lambda for TPS radial basis function
    auto rbf = [stiffness](float r) -> float {
        if (r < 1e-10f)
            return 0.0f;
        return r * r * std::log(r) + stiffness * r * r;
    };

    // Prepare source and destination points as floating-point values
    std::vector<std::pair<float, float>> srcPts;
    std::vector<std::pair<float, float>> dstPts;

    for (size_t i = 0; i < srcPoints.size(); i++) {
        srcPts.emplace_back(static_cast<float>(srcPoints[i].x), static_cast<float>(srcPoints[i].y));
        dstPts.emplace_back(static_cast<float>(dstPoints[i].x), static_cast<float>(dstPoints[i].y));
    }

    // Apply the warping based on image type
    if (image.getType() == Image::Type::Grayscale) {
        auto& output = static_cast<GrayscaleImage&>(*outputImage);
        const auto& input = static_cast<const GrayscaleImage&>(image);

        // Process each pixel in the output image
        for (int y = 0; y < dstHeight; y++) {
            for (int x = 0; x < dstWidth; x++) {
                // For each output pixel, calculate the corresponding input pixel
                float totalWeight = 0.0f;
                float srcXWeighted = 0.0f;
                float srcYWeighted = 0.0f;

                // Simple inverse distance weighting for demonstration
                for (size_t i = 0; i < dstPts.size(); i++) {
                    float dx = static_cast<float>(x) - dstPts[i].first;
                    float dy = static_cast<float>(y) - dstPts[i].second;
                    float distance = std::sqrt(dx * dx + dy * dy);

                    // Apply radial basis function
                    float weight = rbf(distance);

                    // Add weighted contribution
                    srcXWeighted += srcPts[i].first * weight;
                    srcYWeighted += srcPts[i].second * weight;
                    totalWeight += weight;
                }

                // Normalize by total weight
                float srcX, srcY;
                if (totalWeight > 1e-10f) {
                    srcX = srcXWeighted / totalWeight;
                    srcY = srcYWeighted / totalWeight;
                } else {
                    // If all control points are too far, use direct mapping
                    srcX = static_cast<float>(x) * srcWidth / dstWidth;
                    srcY = static_cast<float>(y) * srcHeight / dstHeight;
                }

                // Check if source coordinates are valid
                if (srcX >= 0 && srcX < srcWidth && srcY >= 0 && srcY < srcHeight) {
                    // Use interpolation to get the pixel value
                    auto valueResult =
                        Interpolation::interpolateGray(input, srcX, srcY, m_interpolationMethod);

                    if (valueResult) {
                        output.setPixel(x, y, valueResult.value());
                    }
                }
            }
        }
    } else if (image.getType() == Image::Type::RGB || image.getType() == Image::Type::RGBA) {
        auto& output = static_cast<ColorImage&>(*outputImage);
        const auto& input = static_cast<const ColorImage&>(image);
        bool hasAlpha = (image.getType() == Image::Type::RGBA);

        // Process each pixel in the output image
        for (int y = 0; y < dstHeight; y++) {
            for (int x = 0; x < dstWidth; x++) {
                // For each output pixel, calculate the corresponding input pixel
                float totalWeight = 0.0f;
                float srcXWeighted = 0.0f;
                float srcYWeighted = 0.0f;

                // Simple inverse distance weighting for demonstration
                for (size_t i = 0; i < dstPts.size(); i++) {
                    float dx = static_cast<float>(x) - dstPts[i].first;
                    float dy = static_cast<float>(y) - dstPts[i].second;
                    float distance = std::sqrt(dx * dx + dy * dy);

                    // Apply radial basis function
                    float weight = rbf(distance);

                    // Add weighted contribution
                    srcXWeighted += srcPts[i].first * weight;
                    srcYWeighted += srcPts[i].second * weight;
                    totalWeight += weight;
                }

                // Normalize by total weight
                float srcX, srcY;
                if (totalWeight > 1e-10f) {
                    srcX = srcXWeighted / totalWeight;
                    srcY = srcYWeighted / totalWeight;
                } else {
                    // If all control points are too far, use direct mapping
                    srcX = static_cast<float>(x) * srcWidth / dstWidth;
                    srcY = static_cast<float>(y) * srcHeight / dstHeight;
                }

                // Check if source coordinates are valid
                if (srcX >= 0 && srcX < srcWidth && srcY >= 0 && srcY < srcHeight) {
                    // Use interpolation to get the pixel value
                    uint8_t r, g, b, a;
                    auto colorResult = Interpolation::interpolateColor(
                        input, srcX, srcY, r, g, b, a, m_interpolationMethod);

                    if (colorResult) {
                        output.setPixel(x, y, r, g, b, hasAlpha ? a : 255);
                    }
                } else if (hasAlpha) {
                    // Set transparent pixel for out-of-bounds coordinates
                    output.setPixel(x, y, 0, 0, 0, 0);
                }
            }
        }
    }

    return makeSuccessResult(std::move(outputImage));
}

Result<std::unique_ptr<Image>> WarpTransform::applyMeshWarp(const Image& image) const {
    // For this simplified implementation, we'll just create a grid-based warping
    // where each grid cell is defined by four control points

    int srcWidth = image.getWidth();
    int srcHeight = image.getHeight();

    // Determine grid dimensions from control points
    // This should be passed in from the createMeshWarp method
    int meshWidth = static_cast<int>(std::sqrt(m_sourcePoints.size()));
    int meshHeight = static_cast<int>(m_sourcePoints.size()) / meshWidth;

    // If we can't determine a proper grid, fall back to TPS
    if (meshWidth * meshHeight != static_cast<int>(m_sourcePoints.size())) {
        return applyThinPlateSpline(image);
    }

    // Create output image with same dimensions as input
    int dstWidth = srcWidth;
    int dstHeight = srcHeight;

    // Create the output image based on type
    Result<std::unique_ptr<Image>> result;

    switch (image.getType()) {
        case Image::Type::Grayscale:
            result = ImageFactory::createGrayscale(dstWidth, dstHeight);
            break;
        case Image::Type::RGB:
            result = ImageFactory::createColor(dstWidth, dstHeight, false);
            break;
        case Image::Type::RGBA:
            result = ImageFactory::createColor(dstWidth, dstHeight, true);
            break;
        default:
            return makeErrorResult<std::unique_ptr<Image>>(
                ErrorCode::UnsupportedFormat,
                std::format("Unsupported image type for warping: {}",
                            static_cast<int>(image.getType())));
    }

    if (!result) {
        return result;
    }

    auto outputImage = std::move(result.value());

    // For each pixel in the destination image, find its position in the source image
    if (image.getType() == Image::Type::Grayscale) {
        auto& output = static_cast<GrayscaleImage&>(*outputImage);
        const auto& input = static_cast<const GrayscaleImage&>(image);

        // Process each pixel in the output image
        for (int y = 0; y < dstHeight; y++) {
            for (int x = 0; x < dstWidth; x++) {
                // Find which mesh cell this destination point is in
                int cellX = x * (meshWidth - 1) / dstWidth;
                int cellY = y * (meshHeight - 1) / dstHeight;

                // Get normalized position within this cell
                float u = static_cast<float>(x * (meshWidth - 1) % dstWidth) / dstWidth;
                float v = static_cast<float>(y * (meshHeight - 1) % dstHeight) / dstHeight;

                // Get the four corners of this cell in the destination mesh
                int idx00 = cellY * meshWidth + cellX;
                int idx10 = cellY * meshWidth + cellX + 1;
                int idx01 = (cellY + 1) * meshWidth + cellX;
                int idx11 = (cellY + 1) * meshWidth + cellX + 1;

                // Make sure indices are valid
                idx00 = std::min(idx00, static_cast<int>(m_destPoints.size() - 1));
                idx10 = std::min(idx10, static_cast<int>(m_destPoints.size() - 1));
                idx01 = std::min(idx01, static_cast<int>(m_destPoints.size() - 1));
                idx11 = std::min(idx11, static_cast<int>(m_destPoints.size() - 1));

                // Get the corresponding points in the source mesh
                auto& src00 = m_sourcePoints[idx00];
                auto& src10 = m_sourcePoints[idx10];
                auto& src01 = m_sourcePoints[idx01];
                auto& src11 = m_sourcePoints[idx11];

                // Bilinear interpolation to find the source position
                float srcX = (1 - u) * (1 - v) * src00.x + u * (1 - v) * src10.x +
                             (1 - u) * v * src01.x + u * v * src11.x;

                float srcY = (1 - u) * (1 - v) * src00.y + u * (1 - v) * src10.y +
                             (1 - u) * v * src01.y + u * v * src11.y;

                // Check if source coordinates are valid
                if (srcX >= 0 && srcX < srcWidth && srcY >= 0 && srcY < srcHeight) {
                    // Use interpolation to get the pixel value
                    auto valueResult =
                        Interpolation::interpolateGray(input, srcX, srcY, m_interpolationMethod);

                    if (valueResult) {
                        output.setPixel(x, y, valueResult.value());
                    }
                }
            }
        }
    } else if (image.getType() == Image::Type::RGB || image.getType() == Image::Type::RGBA) {
        auto& output = static_cast<ColorImage&>(*outputImage);
        const auto& input = static_cast<const ColorImage&>(image);
        bool hasAlpha = (image.getType() == Image::Type::RGBA);

        // Process each pixel in the output image
        for (int y = 0; y < dstHeight; y++) {
            for (int x = 0; x < dstWidth; x++) {
                // Find which mesh cell this destination point is in
                int cellX = x * (meshWidth - 1) / dstWidth;
                int cellY = y * (meshHeight - 1) / dstHeight;

                // Get normalized position within this cell
                float u = static_cast<float>(x * (meshWidth - 1) % dstWidth) / dstWidth;
                float v = static_cast<float>(y * (meshHeight - 1) % dstHeight) / dstHeight;

                // Get the four corners of this cell in the destination mesh
                int idx00 = cellY * meshWidth + cellX;
                int idx10 = cellY * meshWidth + cellX + 1;
                int idx01 = (cellY + 1) * meshWidth + cellX;
                int idx11 = (cellY + 1) * meshWidth + cellX + 1;

                // Make sure indices are valid
                idx00 = std::min(idx00, static_cast<int>(m_destPoints.size() - 1));
                idx10 = std::min(idx10, static_cast<int>(m_destPoints.size() - 1));
                idx01 = std::min(idx01, static_cast<int>(m_destPoints.size() - 1));
                idx11 = std::min(idx11, static_cast<int>(m_destPoints.size() - 1));

                // Get the corresponding points in the source mesh
                auto& src00 = m_sourcePoints[idx00];
                auto& src10 = m_sourcePoints[idx10];
                auto& src01 = m_sourcePoints[idx01];
                auto& src11 = m_sourcePoints[idx11];

                // Bilinear interpolation to find the source position
                float srcX = (1 - u) * (1 - v) * src00.x + u * (1 - v) * src10.x +
                             (1 - u) * v * src01.x + u * v * src11.x;

                float srcY = (1 - u) * (1 - v) * src00.y + u * (1 - v) * src10.y +
                             (1 - u) * v * src01.y + u * v * src11.y;

                // Check if source coordinates are valid
                if (srcX >= 0 && srcX < srcWidth && srcY >= 0 && srcY < srcHeight) {
                    // Use interpolation to get the pixel value
                    uint8_t r, g, b, a;
                    auto colorResult = Interpolation::interpolateColor(
                        input, srcX, srcY, r, g, b, a, m_interpolationMethod);

                    if (colorResult) {
                        output.setPixel(x, y, r, g, b, hasAlpha ? a : 255);
                    }
                } else if (hasAlpha) {
                    // Set transparent pixel for out-of-bounds coordinates
                    output.setPixel(x, y, 0, 0, 0, 0);
                }
            }
        }
    }

    return makeSuccessResult(std::move(outputImage));
}

Result<std::unique_ptr<Image>> WarpTransform::applyTriangulation(const Image& image) const {
    // This is a simplified placeholder for triangulation-based warping
    // A full implementation would involve:
    // 1. Creating a Delaunay triangulation of the control points
    // 2. For each triangle, computing the affine transformation from source to dest
    // 3. Using the appropriate transformation for each pixel based on which triangle it's in

    // For this implementation, we'll just fall back to thin-plate spline warping
    return applyThinPlateSpline(image);
}

}  // namespace DIPAL
