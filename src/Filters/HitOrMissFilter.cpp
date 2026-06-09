// src/Filters/HitOrMissFilter.cpp
#include "../../include/DIPAL/Filters/HitOrMissFilter.hpp"
#include "../../include/DIPAL/Image/GrayscaleImage.hpp"
#include "../../include/DIPAL/Image/ColorImage.hpp"
#include "../../include/DIPAL/Image/ImageFactory.hpp"

#include <algorithm>
#include <format>

namespace DIPAL {

HitOrMissFilter::HitOrMissFilter(StructuringElement foreground, StructuringElement background)
    : m_foreground(std::move(foreground))
    , m_background(std::move(background))
{}

Result<std::unique_ptr<Image>> HitOrMissFilter::apply(const Image& image) const {
    const int width  = image.getWidth();
    const int height = image.getHeight();

    if (width == 0 || height == 0)
        return makeErrorResult<std::unique_ptr<Image>>(
            ErrorCode::InvalidParameter, "Cannot apply Hit-or-Miss to empty image");

    // Obtain a grayscale view (convert color if needed)
    std::unique_ptr<GrayscaleImage> gray;
    try {
        if (image.getType() == Image::Type::Grayscale) {
            auto c = image.clone();
            gray.reset(static_cast<GrayscaleImage*>(c.release()));
        } else if (image.getType() == Image::Type::RGB ||
                   image.getType() == Image::Type::RGBA) {
            auto r = ImageFactory::toGrayscale(static_cast<const ColorImage&>(image));
            if (!r) return makeErrorResult<std::unique_ptr<Image>>(
                r.error().code(), r.error().message());
            gray = std::move(r.value());
        } else {
            return makeErrorResult<std::unique_ptr<Image>>(
                ErrorCode::UnsupportedFormat,
                std::format("HitOrMissFilter: unsupported image type {}",
                            static_cast<int>(image.getType())));
        }

        auto outR = ImageFactory::createGrayscale(width, height);
        if (!outR) return makeErrorResult<std::unique_ptr<Image>>(
            outR.error().code(), outR.error().message());
        auto& out = *outR.value();

        const int fHalfW = m_foreground.getHalfWidth();
        const int fHalfH = m_foreground.getHalfHeight();
        const int fW     = m_foreground.getWidth();
        const int fH     = m_foreground.getHeight();

        for (int y = 0; y < height; ++y) {
            for (int x = 0; x < width; ++x) {
                bool match = true;

                // Check foreground SE: all covered pixels must be >= 128 (ON)
                for (int ky = 0; ky < fH && match; ++ky) {
                    for (int kx = 0; kx < fW && match; ++kx) {
                        if (!m_foreground.at(ky, kx)) continue;
                        const int sy = std::clamp(y + ky - fHalfH, 0, height - 1);
                        const int sx = std::clamp(x + kx - fHalfW, 0, width  - 1);
                        auto pr = gray->getPixel(sx, sy);
                        if (!pr || pr.value() < 128) match = false;
                    }
                }

                // Check background SE: all covered pixels must be < 128 (OFF)
                const int bHalfW = m_background.getHalfWidth();
                const int bHalfH = m_background.getHalfHeight();
                const int bW     = m_background.getWidth();
                const int bH     = m_background.getHeight();

                for (int ky = 0; ky < bH && match; ++ky) {
                    for (int kx = 0; kx < bW && match; ++kx) {
                        if (!m_background.at(ky, kx)) continue;
                        const int sy = std::clamp(y + ky - bHalfH, 0, height - 1);
                        const int sx = std::clamp(x + kx - bHalfW, 0, width  - 1);
                        auto pr = gray->getPixel(sx, sy);
                        if (!pr || pr.value() >= 128) match = false;
                    }
                }

                auto sr = out.setPixel(x, y, match ? uint8_t{255} : uint8_t{0});
                if (!sr) return makeErrorResult<std::unique_ptr<Image>>(
                    sr.error().code(), sr.error().message());
            }
        }

        return makeSuccessResult<std::unique_ptr<Image>>(std::move(outR.value()));

    } catch (const std::exception& e) {
        return makeErrorResult<std::unique_ptr<Image>>(
            ErrorCode::ProcessingFailed,
            std::format("HitOrMissFilter failed: {}", e.what()));
    }
}

std::string_view HitOrMissFilter::getName() const { return "HitOrMissFilter"; }

std::unique_ptr<FilterStrategy> HitOrMissFilter::clone() const {
    return std::make_unique<HitOrMissFilter>(m_foreground, m_background);
}

} // namespace DIPAL
