// src/Filters/CannyFilter.cpp
#include "../../include/DIPAL/Filters/CannyFilter.hpp"
#include "../../include/DIPAL/Filters/GaussianBlurFilter.hpp"
#include "../../include/DIPAL/Image/GrayscaleImage.hpp"
#include "../../include/DIPAL/Image/ColorImage.hpp"
#include "../../include/DIPAL/Image/ImageFactory.hpp"

#include <algorithm>
#include <cmath>
#include <format>
#include <numbers>
#include <queue>
#include <vector>

namespace DIPAL {

CannyFilter::CannyFilter(float lowThreshold, float highThreshold, float sigma, int kernelSize)
    : m_lowThreshold(lowThreshold)
    , m_highThreshold(highThreshold)
    , m_sigma(sigma)
    , m_kernelSize(kernelSize)
{}

Result<std::unique_ptr<Image>> CannyFilter::apply(const Image& image) const {
    const int width  = image.getWidth();
    const int height = image.getHeight();

    if (width == 0 || height == 0) {
        return makeErrorResult<std::unique_ptr<Image>>(
            ErrorCode::InvalidParameter,
            "Cannot apply Canny filter to an empty image");
    }

    try {
        // ---------------------------------------------------------------
        // Stage 1: Convert to grayscale
        // ---------------------------------------------------------------
        std::unique_ptr<GrayscaleImage> gray;
        if (image.getType() == Image::Type::Grayscale) {
            auto c = image.clone();
            gray.reset(static_cast<GrayscaleImage*>(c.release()));
        } else if (image.getType() == Image::Type::RGB ||
                   image.getType() == Image::Type::RGBA) {
            auto r = ImageFactory::toGrayscale(static_cast<const ColorImage&>(image));
            if (!r)
                return makeErrorResult<std::unique_ptr<Image>>(
                    r.error().code(), r.error().message());
            gray = std::move(r.value());
        } else {
            return makeErrorResult<std::unique_ptr<Image>>(
                ErrorCode::UnsupportedFormat,
                std::format("CannyFilter: unsupported image type {}",
                            static_cast<int>(image.getType())));
        }

        // ---------------------------------------------------------------
        // Stage 2: Gaussian smoothing
        // ---------------------------------------------------------------
        int kSize = m_kernelSize;
        if (kSize <= 0)
            kSize = static_cast<int>(std::ceil(6.0f * m_sigma)) | 1;
        if (kSize % 2 == 0)
            ++kSize;

        GaussianBlurFilter gaussFilter(m_sigma, kSize);
        auto blurResult = gaussFilter.apply(*gray);
        if (!blurResult)
            return makeErrorResult<std::unique_ptr<Image>>(
                blurResult.error().code(), blurResult.error().message());
        auto& blurred = static_cast<GrayscaleImage&>(*blurResult.value());

        // ---------------------------------------------------------------
        // Stage 3: Sobel gradient magnitude and direction
        // ---------------------------------------------------------------
        const int sobelX[3][3] = {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}};
        const int sobelY[3][3] = {{-1,-2,-1}, { 0, 0, 0}, { 1, 2, 1}};

        std::vector<float> magnitude(static_cast<size_t>(width * height), 0.0f);
        std::vector<float> direction(static_cast<size_t>(width * height), 0.0f);

        for (int y = 0; y < height; ++y) {
            for (int x = 0; x < width; ++x) {
                float gx = 0.0f, gy = 0.0f;
                for (int ky = -1; ky <= 1; ++ky) {
                    for (int kx = -1; kx <= 1; ++kx) {
                        const int sy = std::clamp(y + ky, 0, height - 1);
                        const int sx = std::clamp(x + kx, 0, width  - 1);
                        auto pr = blurred.getPixel(sx, sy);
                        if (!pr)
                            return makeErrorResult<std::unique_ptr<Image>>(
                                pr.error().code(), pr.error().message());
                        const float p = static_cast<float>(pr.value());
                        gx += sobelX[ky + 1][kx + 1] * p;
                        gy += sobelY[ky + 1][kx + 1] * p;
                    }
                }
                const size_t idx = static_cast<size_t>(y * width + x);
                magnitude[idx] = std::sqrt(gx * gx + gy * gy);
                direction[idx] = std::atan2(gy, gx);
            }
        }

        // ---------------------------------------------------------------
        // Stage 4: Non-maximum suppression
        // ---------------------------------------------------------------
        std::vector<float> suppressed(static_cast<size_t>(width * height), 0.0f);

        for (int y = 1; y < height - 1; ++y) {
            for (int x = 1; x < width - 1; ++x) {
                const size_t idx = static_cast<size_t>(y * width + x);
                const float  mag = magnitude[idx];
                // Quantise angle to 0°, 45°, 90°, 135°
                float angle = direction[idx] * (180.0f / std::numbers::pi_v<float>);
                if (angle < 0.0f) angle += 180.0f;

                float mag1 = 0.0f, mag2 = 0.0f;
                if (angle < 22.5f || angle >= 157.5f) {
                    // 0° — horizontal edge
                    mag1 = magnitude[static_cast<size_t>(y * width + (x - 1))];
                    mag2 = magnitude[static_cast<size_t>(y * width + (x + 1))];
                } else if (angle < 67.5f) {
                    // 45°
                    mag1 = magnitude[static_cast<size_t>((y - 1) * width + (x + 1))];
                    mag2 = magnitude[static_cast<size_t>((y + 1) * width + (x - 1))];
                } else if (angle < 112.5f) {
                    // 90° — vertical edge
                    mag1 = magnitude[static_cast<size_t>((y - 1) * width + x)];
                    mag2 = magnitude[static_cast<size_t>((y + 1) * width + x)];
                } else {
                    // 135°
                    mag1 = magnitude[static_cast<size_t>((y - 1) * width + (x - 1))];
                    mag2 = magnitude[static_cast<size_t>((y + 1) * width + (x + 1))];
                }

                suppressed[idx] = (mag >= mag1 && mag >= mag2) ? mag : 0.0f;
            }
        }

        // ---------------------------------------------------------------
        // Stage 5: Double-threshold hysteresis
        // ---------------------------------------------------------------
        // Pixel states: 0 = background, 128 = weak, 255 = strong
        auto outResult = ImageFactory::createGrayscale(width, height);
        if (!outResult)
            return makeErrorResult<std::unique_ptr<Image>>(
                outResult.error().code(), outResult.error().message());
        auto& out = *outResult.value();

        constexpr uint8_t kStrong = 255;
        constexpr uint8_t kWeak   = 128;

        std::queue<std::pair<int,int>> strongQueue;

        for (int y = 0; y < height; ++y) {
            for (int x = 0; x < width; ++x) {
                const float mag = suppressed[static_cast<size_t>(y * width + x)];
                uint8_t val = 0;
                if (mag >= m_highThreshold) {
                    val = kStrong;
                    strongQueue.push({x, y});
                } else if (mag >= m_lowThreshold) {
                    val = kWeak;
                }
                auto sr = out.setPixel(x, y, val);
                if (!sr)
                    return makeErrorResult<std::unique_ptr<Image>>(
                        sr.error().code(), sr.error().message());
            }
        }

        // BFS from strong pixels — promote connected weak pixels
        const int dx[8] = {-1, 0, 1, -1, 1, -1, 0, 1};
        const int dy[8] = {-1,-1,-1,  0, 0,  1, 1, 1};

        while (!strongQueue.empty()) {
            auto [cx, cy] = strongQueue.front();
            strongQueue.pop();

            for (int d = 0; d < 8; ++d) {
                const int nx = cx + dx[d];
                const int ny = cy + dy[d];
                if (nx < 0 || nx >= width || ny < 0 || ny >= height)
                    continue;

                auto pr = out.getPixel(nx, ny);
                if (!pr || pr.value() != kWeak)
                    continue;

                auto sr = out.setPixel(nx, ny, kStrong);
                if (!sr)
                    return makeErrorResult<std::unique_ptr<Image>>(
                        sr.error().code(), sr.error().message());
                strongQueue.push({nx, ny});
            }
        }

        // Clear remaining weak pixels
        for (int y = 0; y < height; ++y) {
            for (int x = 0; x < width; ++x) {
                auto pr = out.getPixel(x, y);
                if (pr && pr.value() == kWeak) {
                    auto sr = out.setPixel(x, y, 0);
                    if (!sr)
                        return makeErrorResult<std::unique_ptr<Image>>(
                            sr.error().code(), sr.error().message());
                }
            }
        }

        return makeSuccessResult<std::unique_ptr<Image>>(std::move(outResult.value()));

    } catch (const std::exception& e) {
        return makeErrorResult<std::unique_ptr<Image>>(
            ErrorCode::ProcessingFailed,
            std::format("CannyFilter failed: {}", e.what()));
    }
}

std::string_view CannyFilter::getName() const { return "CannyFilter"; }

std::unique_ptr<FilterStrategy> CannyFilter::clone() const {
    return std::make_unique<CannyFilter>(
        m_lowThreshold, m_highThreshold, m_sigma, m_kernelSize);
}

} // namespace DIPAL
