// src/Filters/HighPassFilter.cpp
#include "../../include/DIPAL/Filters/HighPassFilter.hpp"
#include "../../include/DIPAL/Filters/GaussianBlurFilter.hpp"
#include "../../include/DIPAL/Image/GrayscaleImage.hpp"
#include "../../include/DIPAL/Image/ColorImage.hpp"
#include "../../include/DIPAL/Image/ImageFactory.hpp"

#include <algorithm>
#include <cmath>
#include <format>

namespace DIPAL {

HighPassFilter::HighPassFilter(float sigma, int kernelSize)
    : m_sigma(sigma), m_kernelSize(kernelSize)
{
    if (sigma <= 0.0f)
        throw std::invalid_argument(
            std::format("HighPassFilter: sigma must be positive, got {}", sigma));
}

Result<std::unique_ptr<Image>> HighPassFilter::apply(const Image& image) const {
    const int width  = image.getWidth();
    const int height = image.getHeight();

    if (width == 0 || height == 0)
        return makeErrorResult<std::unique_ptr<Image>>(
            ErrorCode::InvalidParameter, "Cannot apply high-pass filter to empty image");

    int kSize = m_kernelSize;
    if (kSize <= 0) kSize = static_cast<int>(std::ceil(6.0f * m_sigma)) | 1;
    if (kSize % 2 == 0) ++kSize;

    // Compute blurred version
    GaussianBlurFilter blur(m_sigma, kSize);
    auto blurResult = blur.apply(image);
    if (!blurResult) return blurResult;

    // high-pass = original - blurred + 128  (per channel)
    try {
        if (image.getType() == Image::Type::Grayscale) {
            const auto& src = static_cast<const GrayscaleImage&>(image);
            const auto& low = static_cast<const GrayscaleImage&>(*blurResult.value());
            auto outR = ImageFactory::createGrayscale(width, height);
            if (!outR) return makeErrorResult<std::unique_ptr<Image>>(
                outR.error().code(), outR.error().message());
            auto& out = *outR.value();

            for (int y = 0; y < height; ++y) {
                for (int x = 0; x < width; ++x) {
                    auto sp = src.getPixel(x, y);
                    auto lp = low.getPixel(x, y);
                    if (!sp || !lp) return makeErrorResult<std::unique_ptr<Image>>(
                        ErrorCode::ProcessingFailed, "Failed to read pixel");
                    const int val = static_cast<int>(sp.value()) - lp.value() + 128;
                    auto sr = out.setPixel(x, y,
                        static_cast<uint8_t>(std::clamp(val, 0, 255)));
                    if (!sr) return makeErrorResult<std::unique_ptr<Image>>(
                        sr.error().code(), sr.error().message());
                }
            }
            return makeSuccessResult<std::unique_ptr<Image>>(std::move(outR.value()));
        }

        if (image.getType() == Image::Type::RGB || image.getType() == Image::Type::RGBA) {
            const auto& src  = static_cast<const ColorImage&>(image);
            const auto& low  = static_cast<const ColorImage&>(*blurResult.value());
            const bool  hasA = (image.getType() == Image::Type::RGBA);
            auto outR = ImageFactory::createColor(width, height, hasA);
            if (!outR) return makeErrorResult<std::unique_ptr<Image>>(
                outR.error().code(), outR.error().message());
            auto& out = *outR.value();

            for (int y = 0; y < height; ++y) {
                for (int x = 0; x < width; ++x) {
                    uint8_t sr2, sg, sb, sa, lr, lg, lb, la;
                    auto sp = src.getPixel(x, y, sr2, sg, sb, sa);
                    auto lp = low.getPixel(x, y, lr, lg, lb, la);
                    if (!sp || !lp) return makeErrorResult<std::unique_ptr<Image>>(
                        ErrorCode::ProcessingFailed, "Failed to read pixel");
                    const auto clamp8 = [](int v) {
                        return static_cast<uint8_t>(std::clamp(v, 0, 255));
                    };
                    auto res = out.setPixel(x, y,
                        clamp8(sr2 - lr + 128),
                        clamp8(sg  - lg + 128),
                        clamp8(sb  - lb + 128), sa);
                    if (!res) return makeErrorResult<std::unique_ptr<Image>>(
                        res.error().code(), res.error().message());
                }
            }
            return makeSuccessResult<std::unique_ptr<Image>>(std::move(outR.value()));
        }

        return makeErrorResult<std::unique_ptr<Image>>(
            ErrorCode::UnsupportedFormat,
            std::format("HighPassFilter: unsupported image type {}",
                        static_cast<int>(image.getType())));

    } catch (const std::exception& e) {
        return makeErrorResult<std::unique_ptr<Image>>(
            ErrorCode::ProcessingFailed,
            std::format("HighPassFilter failed: {}", e.what()));
    }
}

std::string_view HighPassFilter::getName() const { return "HighPassFilter"; }

std::unique_ptr<FilterStrategy> HighPassFilter::clone() const {
    return std::make_unique<HighPassFilter>(m_sigma, m_kernelSize);
}

} // namespace DIPAL
