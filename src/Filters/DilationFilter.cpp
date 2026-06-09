// src/Filters/DilationFilter.cpp
#include "../../include/DIPAL/Filters/DilationFilter.hpp"
#include "../../include/DIPAL/Image/GrayscaleImage.hpp"
#include "../../include/DIPAL/Image/ColorImage.hpp"
#include "../../include/DIPAL/Image/ImageFactory.hpp"

#include <algorithm>
#include <format>

namespace DIPAL {

DilationFilter::DilationFilter(StructuringElement se) : m_se(std::move(se)) {}

Result<std::unique_ptr<Image>> DilationFilter::apply(const Image& image) const {
    const int width  = image.getWidth();
    const int height = image.getHeight();

    if (width == 0 || height == 0)
        return makeErrorResult<std::unique_ptr<Image>>(
            ErrorCode::InvalidParameter, "Cannot dilate an empty image");

    const int halfW = m_se.getHalfWidth();
    const int halfH = m_se.getHalfHeight();
    const int kW    = m_se.getWidth();
    const int kH    = m_se.getHeight();

    try {
        if (image.getType() == Image::Type::Grayscale) {
            const auto& src = static_cast<const GrayscaleImage&>(image);
            auto outR = ImageFactory::createGrayscale(width, height);
            if (!outR) return makeErrorResult<std::unique_ptr<Image>>(
                outR.error().code(), outR.error().message());
            auto& out = *outR.value();

            for (int y = 0; y < height; ++y) {
                for (int x = 0; x < width; ++x) {
                    uint8_t maxVal = 0;
                    for (int ky = 0; ky < kH; ++ky) {
                        for (int kx = 0; kx < kW; ++kx) {
                            if (!m_se.at(ky, kx)) continue;
                            const int sy = std::clamp(y + ky - halfH, 0, height - 1);
                            const int sx = std::clamp(x + kx - halfW, 0, width  - 1);
                            auto pr = src.getPixel(sx, sy);
                            if (!pr) return makeErrorResult<std::unique_ptr<Image>>(
                                pr.error().code(), pr.error().message());
                            maxVal = std::max(maxVal, pr.value());
                        }
                    }
                    auto sr = out.setPixel(x, y, maxVal);
                    if (!sr) return makeErrorResult<std::unique_ptr<Image>>(
                        sr.error().code(), sr.error().message());
                }
            }
            return makeSuccessResult<std::unique_ptr<Image>>(std::move(outR.value()));
        }

        if (image.getType() == Image::Type::RGB || image.getType() == Image::Type::RGBA) {
            const auto& src  = static_cast<const ColorImage&>(image);
            const bool  hasA = (image.getType() == Image::Type::RGBA);
            auto outR = ImageFactory::createColor(width, height, hasA);
            if (!outR) return makeErrorResult<std::unique_ptr<Image>>(
                outR.error().code(), outR.error().message());
            auto& out = *outR.value();

            for (int y = 0; y < height; ++y) {
                for (int x = 0; x < width; ++x) {
                    uint8_t maxR = 0, maxG = 0, maxB = 0;
                    for (int ky = 0; ky < kH; ++ky) {
                        for (int kx = 0; kx < kW; ++kx) {
                            if (!m_se.at(ky, kx)) continue;
                            const int sy = std::clamp(y + ky - halfH, 0, height - 1);
                            const int sx = std::clamp(x + kx - halfW, 0, width  - 1);
                            uint8_t r, g, b, a;
                            auto pr = src.getPixel(sx, sy, r, g, b, a);
                            if (!pr) return makeErrorResult<std::unique_ptr<Image>>(
                                pr.error().code(), pr.error().message());
                            maxR = std::max(maxR, r);
                            maxG = std::max(maxG, g);
                            maxB = std::max(maxB, b);
                        }
                    }
                    // Preserve alpha from center pixel
                    uint8_t cr, cg, cb, ca;
                    auto cp = src.getPixel(x, y, cr, cg, cb, ca);
                    if (!cp) return makeErrorResult<std::unique_ptr<Image>>(
                        cp.error().code(), cp.error().message());
                    auto sr = out.setPixel(x, y, maxR, maxG, maxB, ca);
                    if (!sr) return makeErrorResult<std::unique_ptr<Image>>(
                        sr.error().code(), sr.error().message());
                }
            }
            return makeSuccessResult<std::unique_ptr<Image>>(std::move(outR.value()));
        }

        return makeErrorResult<std::unique_ptr<Image>>(
            ErrorCode::UnsupportedFormat,
            std::format("DilationFilter: unsupported image type {}",
                        static_cast<int>(image.getType())));

    } catch (const std::exception& e) {
        return makeErrorResult<std::unique_ptr<Image>>(
            ErrorCode::ProcessingFailed,
            std::format("DilationFilter failed: {}", e.what()));
    }
}

std::string_view DilationFilter::getName() const { return "DilationFilter"; }

std::unique_ptr<FilterStrategy> DilationFilter::clone() const {
    return std::make_unique<DilationFilter>(m_se);
}

} // namespace DIPAL
