// src/Filters/TopHatFilter.cpp
#include "../../include/DIPAL/Filters/TopHatFilter.hpp"
#include "../../include/DIPAL/Filters/OpeningFilter.hpp"
#include "../../include/DIPAL/Filters/ClosingFilter.hpp"
#include "../../include/DIPAL/Image/GrayscaleImage.hpp"
#include "../../include/DIPAL/Image/ColorImage.hpp"
#include "../../include/DIPAL/Image/ImageFactory.hpp"

#include <algorithm>
#include <format>

namespace DIPAL {

TopHatFilter::TopHatFilter(Type type, StructuringElement se)
    : m_type(type), m_se(std::move(se)) {}

Result<std::unique_ptr<Image>> TopHatFilter::apply(const Image& image) const {
    const int width  = image.getWidth();
    const int height = image.getHeight();

    if (width == 0 || height == 0)
        return makeErrorResult<std::unique_ptr<Image>>(
            ErrorCode::InvalidParameter, "Cannot apply top-hat to empty image");

    // White: original - opening;  Black: closing - original
    std::unique_ptr<Image> morphed;
    if (m_type == Type::White) {
        OpeningFilter op(m_se);
        auto r = op.apply(image);
        if (!r) return r;
        morphed = std::move(r.value());
    } else {
        ClosingFilter cl(m_se);
        auto r = cl.apply(image);
        if (!r) return r;
        morphed = std::move(r.value());
    }

    try {
        if (image.getType() == Image::Type::Grayscale) {
            const auto& src  = static_cast<const GrayscaleImage&>(image);
            const auto& morph = static_cast<const GrayscaleImage&>(*morphed);
            auto outR = ImageFactory::createGrayscale(width, height);
            if (!outR) return makeErrorResult<std::unique_ptr<Image>>(
                outR.error().code(), outR.error().message());
            auto& out = *outR.value();

            for (int y = 0; y < height; ++y) {
                for (int x = 0; x < width; ++x) {
                    auto sp = src.getPixel(x, y);
                    auto mp = morph.getPixel(x, y);
                    if (!sp || !mp) return makeErrorResult<std::unique_ptr<Image>>(
                        ErrorCode::ProcessingFailed, "Failed to read pixel");

                    uint8_t val;
                    if (m_type == Type::White)
                        val = static_cast<uint8_t>(
                            std::max(0, static_cast<int>(sp.value()) - mp.value()));
                    else
                        val = static_cast<uint8_t>(
                            std::max(0, static_cast<int>(mp.value()) - sp.value()));

                    auto sr = out.setPixel(x, y, val);
                    if (!sr) return makeErrorResult<std::unique_ptr<Image>>(
                        sr.error().code(), sr.error().message());
                }
            }
            return makeSuccessResult<std::unique_ptr<Image>>(std::move(outR.value()));
        }

        if (image.getType() == Image::Type::RGB || image.getType() == Image::Type::RGBA) {
            const auto& src   = static_cast<const ColorImage&>(image);
            const auto& morph = static_cast<const ColorImage&>(*morphed);
            const bool  hasA  = (image.getType() == Image::Type::RGBA);
            auto outR = ImageFactory::createColor(width, height, hasA);
            if (!outR) return makeErrorResult<std::unique_ptr<Image>>(
                outR.error().code(), outR.error().message());
            auto& out = *outR.value();

            for (int y = 0; y < height; ++y) {
                for (int x = 0; x < width; ++x) {
                    uint8_t sr2, sg, sb, sa, mr, mg, mb, ma;
                    auto sp = src.getPixel(x, y, sr2, sg, sb, sa);
                    auto mp = morph.getPixel(x, y, mr, mg, mb, ma);
                    if (!sp || !mp) return makeErrorResult<std::unique_ptr<Image>>(
                        ErrorCode::ProcessingFailed, "Failed to read pixel");

                    uint8_t outR2, outG, outB;
                    if (m_type == Type::White) {
                        outR2 = static_cast<uint8_t>(std::max(0, static_cast<int>(sr2) - mr));
                        outG  = static_cast<uint8_t>(std::max(0, static_cast<int>(sg)  - mg));
                        outB  = static_cast<uint8_t>(std::max(0, static_cast<int>(sb)  - mb));
                    } else {
                        outR2 = static_cast<uint8_t>(std::max(0, static_cast<int>(mr) - sr2));
                        outG  = static_cast<uint8_t>(std::max(0, static_cast<int>(mg) - sg));
                        outB  = static_cast<uint8_t>(std::max(0, static_cast<int>(mb) - sb));
                    }
                    auto sr = out.setPixel(x, y, outR2, outG, outB, sa);
                    if (!sr) return makeErrorResult<std::unique_ptr<Image>>(
                        sr.error().code(), sr.error().message());
                }
            }
            return makeSuccessResult<std::unique_ptr<Image>>(std::move(outR.value()));
        }

        return makeErrorResult<std::unique_ptr<Image>>(
            ErrorCode::UnsupportedFormat,
            std::format("TopHatFilter: unsupported image type {}",
                        static_cast<int>(image.getType())));

    } catch (const std::exception& e) {
        return makeErrorResult<std::unique_ptr<Image>>(
            ErrorCode::ProcessingFailed,
            std::format("TopHatFilter failed: {}", e.what()));
    }
}

std::string_view TopHatFilter::getName() const {
    return m_type == Type::White ? "WhiteTopHatFilter" : "BlackTopHatFilter";
}

std::unique_ptr<FilterStrategy> TopHatFilter::clone() const {
    return std::make_unique<TopHatFilter>(m_type, m_se);
}

} // namespace DIPAL
