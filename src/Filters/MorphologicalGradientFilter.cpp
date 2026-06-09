// src/Filters/MorphologicalGradientFilter.cpp
#include "../../include/DIPAL/Filters/MorphologicalGradientFilter.hpp"
#include "../../include/DIPAL/Filters/ErosionFilter.hpp"
#include "../../include/DIPAL/Filters/DilationFilter.hpp"
#include "../../include/DIPAL/Image/GrayscaleImage.hpp"
#include "../../include/DIPAL/Image/ColorImage.hpp"
#include "../../include/DIPAL/Image/ImageFactory.hpp"

#include <algorithm>
#include <format>

namespace DIPAL {

MorphologicalGradientFilter::MorphologicalGradientFilter(StructuringElement se)
    : m_se(std::move(se)) {}

Result<std::unique_ptr<Image>> MorphologicalGradientFilter::apply(const Image& image) const {
    if (image.getWidth() == 0 || image.getHeight() == 0)
        return makeErrorResult<std::unique_ptr<Image>>(
            ErrorCode::InvalidParameter, "Cannot compute morphological gradient of empty image");

    DilationFilter dilate(m_se);
    ErosionFilter  erode(m_se);

    auto dilated = dilate.apply(image);
    if (!dilated) return dilated;

    auto eroded = erode.apply(image);
    if (!eroded) return eroded;

    // Subtract: gradient = dilation - erosion (per pixel, clamped to [0,255])
    const int width  = image.getWidth();
    const int height = image.getHeight();

    try {
        if (image.getType() == Image::Type::Grayscale) {
            const auto& d = static_cast<const GrayscaleImage&>(*dilated.value());
            const auto& e = static_cast<const GrayscaleImage&>(*eroded.value());
            auto outR = ImageFactory::createGrayscale(width, height);
            if (!outR) return makeErrorResult<std::unique_ptr<Image>>(
                outR.error().code(), outR.error().message());
            auto& out = *outR.value();

            for (int y = 0; y < height; ++y) {
                for (int x = 0; x < width; ++x) {
                    auto dp = d.getPixel(x, y);
                    auto ep = e.getPixel(x, y);
                    if (!dp || !ep) return makeErrorResult<std::unique_ptr<Image>>(
                        ErrorCode::ProcessingFailed, "Failed to read pixel");
                    const uint8_t val = static_cast<uint8_t>(dp.value() - ep.value());
                    auto sr = out.setPixel(x, y, val);
                    if (!sr) return makeErrorResult<std::unique_ptr<Image>>(
                        sr.error().code(), sr.error().message());
                }
            }
            return makeSuccessResult<std::unique_ptr<Image>>(std::move(outR.value()));
        }

        if (image.getType() == Image::Type::RGB || image.getType() == Image::Type::RGBA) {
            const auto& d    = static_cast<const ColorImage&>(*dilated.value());
            const auto& e    = static_cast<const ColorImage&>(*eroded.value());
            const bool  hasA = (image.getType() == Image::Type::RGBA);
            auto outR = ImageFactory::createColor(width, height, hasA);
            if (!outR) return makeErrorResult<std::unique_ptr<Image>>(
                outR.error().code(), outR.error().message());
            auto& out = *outR.value();

            for (int y = 0; y < height; ++y) {
                for (int x = 0; x < width; ++x) {
                    uint8_t dr, dg, db, da, er, eg, eb, ea;
                    auto dp = d.getPixel(x, y, dr, dg, db, da);
                    auto ep = e.getPixel(x, y, er, eg, eb, ea);
                    if (!dp || !ep) return makeErrorResult<std::unique_ptr<Image>>(
                        ErrorCode::ProcessingFailed, "Failed to read pixel");
                    auto sr = out.setPixel(x, y,
                        static_cast<uint8_t>(dr - er),
                        static_cast<uint8_t>(dg - eg),
                        static_cast<uint8_t>(db - eb), da);
                    if (!sr) return makeErrorResult<std::unique_ptr<Image>>(
                        sr.error().code(), sr.error().message());
                }
            }
            return makeSuccessResult<std::unique_ptr<Image>>(std::move(outR.value()));
        }

        return makeErrorResult<std::unique_ptr<Image>>(
            ErrorCode::UnsupportedFormat,
            std::format("MorphologicalGradientFilter: unsupported image type {}",
                        static_cast<int>(image.getType())));

    } catch (const std::exception& e) {
        return makeErrorResult<std::unique_ptr<Image>>(
            ErrorCode::ProcessingFailed,
            std::format("MorphologicalGradientFilter failed: {}", e.what()));
    }
}

std::string_view MorphologicalGradientFilter::getName() const {
    return "MorphologicalGradientFilter";
}

std::unique_ptr<FilterStrategy> MorphologicalGradientFilter::clone() const {
    return std::make_unique<MorphologicalGradientFilter>(m_se);
}

} // namespace DIPAL
