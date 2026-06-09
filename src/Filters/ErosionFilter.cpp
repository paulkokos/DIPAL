// src/Filters/ErosionFilter.cpp
#include "../../include/DIPAL/Filters/ErosionFilter.hpp"
#include "../../include/DIPAL/Image/GrayscaleImage.hpp"
#include "../../include/DIPAL/Image/ColorImage.hpp"
#include "../../include/DIPAL/Image/ImageFactory.hpp"

#include <algorithm>
#include <format>
#include <limits>

namespace DIPAL {

ErosionFilter::ErosionFilter(StructuringElement se) : m_se(std::move(se)) {}

Result<std::unique_ptr<Image>> ErosionFilter::apply(const Image& image) const {
    const int width  = image.getWidth();
    const int height = image.getHeight();

    if (width == 0 || height == 0)
        return makeErrorResult<std::unique_ptr<Image>>(
            ErrorCode::InvalidParameter, "Cannot erode an empty image");

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
                    uint8_t minVal = std::numeric_limits<uint8_t>::max();
                    for (int ky = 0; ky < kH; ++ky) {
                        for (int kx = 0; kx < kW; ++kx) {
                            if (!m_se.at(ky, kx)) continue;
                            const int sy = std::clamp(y + ky - halfH, 0, height - 1);
                            const int sx = std::clamp(x + kx - halfW, 0, width  - 1);
                            auto pr = src.getPixel(sx, sy);
                            if (!pr) return makeErrorResult<std::unique_ptr<Image>>(
                                pr.error().code(), pr.error().message());
                            minVal = std::min(minVal, pr.value());
                        }
                    }
                    auto sr = out.setPixel(x, y, minVal);
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
                    uint8_t minR = 255, minG = 255, minB = 255;
                    for (int ky = 0; ky < kH; ++ky) {
                        for (int kx = 0; kx < kW; ++kx) {
                            if (!m_se.at(ky, kx)) continue;
                            const int sy = std::clamp(y + ky - halfH, 0, height - 1);
                            const int sx = std::clamp(x + kx - halfW, 0, width  - 1);
                            uint8_t r, g, b, a;
                            auto pr = src.getPixel(sx, sy, r, g, b, a);
                            if (!pr) return makeErrorResult<std::unique_ptr<Image>>(
                                pr.error().code(), pr.error().message());
                            minR = std::min(minR, r);
                            minG = std::min(minG, g);
                            minB = std::min(minB, b);
                        }
                    }
                    // Preserve alpha from the center pixel
                    uint8_t cr, cg, cb, ca;
                    auto cp = src.getPixel(x, y, cr, cg, cb, ca);
                    if (!cp) return makeErrorResult<std::unique_ptr<Image>>(
                        cp.error().code(), cp.error().message());
                    auto sr = out.setPixel(x, y, minR, minG, minB, ca);
                    if (!sr) return makeErrorResult<std::unique_ptr<Image>>(
                        sr.error().code(), sr.error().message());
                }
            }
            return makeSuccessResult<std::unique_ptr<Image>>(std::move(outR.value()));
        }

        return makeErrorResult<std::unique_ptr<Image>>(
            ErrorCode::UnsupportedFormat,
            std::format("ErosionFilter: unsupported image type {}",
                        static_cast<int>(image.getType())));

    } catch (const std::exception& e) {
        return makeErrorResult<std::unique_ptr<Image>>(
            ErrorCode::ProcessingFailed,
            std::format("ErosionFilter failed: {}", e.what()));
    }
}

std::string_view ErosionFilter::getName() const { return "ErosionFilter"; }

std::unique_ptr<FilterStrategy> ErosionFilter::clone() const {
    return std::make_unique<ErosionFilter>(m_se);
}

} // namespace DIPAL
