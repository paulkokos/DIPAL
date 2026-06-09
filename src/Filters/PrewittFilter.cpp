// src/Filters/PrewittFilter.cpp
#include "../../include/DIPAL/Filters/PrewittFilter.hpp"
#include "../../include/DIPAL/Filters/Kernel.hpp"
#include "../../include/DIPAL/Image/GrayscaleImage.hpp"
#include "../../include/DIPAL/Image/ColorImage.hpp"
#include "../../include/DIPAL/Image/ImageFactory.hpp"

#include <algorithm>
#include <cmath>
#include <format>
#include <vector>

namespace DIPAL {

PrewittFilter::PrewittFilter(bool normalize) : m_normalize(normalize) {}

Result<std::unique_ptr<Image>> PrewittFilter::apply(const Image& image) const {
    const int width  = image.getWidth();
    const int height = image.getHeight();

    if (width == 0 || height == 0) {
        return makeErrorResult<std::unique_ptr<Image>>(
            ErrorCode::InvalidParameter,
            "Cannot apply Prewitt filter to an empty image");
    }

    try {
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
                std::format("PrewittFilter: unsupported image type {}",
                            static_cast<int>(image.getType())));
        }

        const Kernel kx = Kernel::prewittX();
        const Kernel ky = Kernel::prewittY();
        const int halfW = kx.getHalfWidth();
        const int halfH = kx.getHalfHeight();
        const int kW    = kx.getWidth();
        const int kH    = kx.getHeight();

        std::vector<float> magnitudes(static_cast<size_t>(width * height));
        float maxMag = 0.0f;

        for (int y = 0; y < height; ++y) {
            for (int x = 0; x < width; ++x) {
                float gx = 0.0f, gy = 0.0f;
                for (int r = 0; r < kH; ++r) {
                    for (int c = 0; c < kW; ++c) {
                        const int sy = std::clamp(y + r - halfH, 0, height - 1);
                        const int sx = std::clamp(x + c - halfW, 0, width  - 1);
                        auto pr = gray->getPixel(sx, sy);
                        if (!pr)
                            return makeErrorResult<std::unique_ptr<Image>>(
                                pr.error().code(), pr.error().message());
                        const float p = static_cast<float>(pr.value());
                        gx += kx.at(r, c) * p;
                        gy += ky.at(r, c) * p;
                    }
                }
                const float mag = std::sqrt(gx * gx + gy * gy);
                magnitudes[static_cast<size_t>(y * width + x)] = mag;
                maxMag = std::max(maxMag, mag);
            }
        }

        auto outResult = ImageFactory::createGrayscale(width, height);
        if (!outResult)
            return makeErrorResult<std::unique_ptr<Image>>(
                outResult.error().code(), outResult.error().message());
        auto& out = *outResult.value();

        for (int y = 0; y < height; ++y) {
            for (int x = 0; x < width; ++x) {
                float mag = magnitudes[static_cast<size_t>(y * width + x)];
                uint8_t val;
                if (m_normalize && maxMag > 0.0f)
                    val = static_cast<uint8_t>((mag / maxMag) * 255.0f);
                else
                    val = static_cast<uint8_t>(std::min(mag, 255.0f));

                auto sr = out.setPixel(x, y, val);
                if (!sr)
                    return makeErrorResult<std::unique_ptr<Image>>(
                        sr.error().code(), sr.error().message());
            }
        }

        return makeSuccessResult<std::unique_ptr<Image>>(std::move(outResult.value()));

    } catch (const std::exception& e) {
        return makeErrorResult<std::unique_ptr<Image>>(
            ErrorCode::ProcessingFailed,
            std::format("PrewittFilter failed: {}", e.what()));
    }
}

std::string_view PrewittFilter::getName() const { return "PrewittFilter"; }

std::unique_ptr<FilterStrategy> PrewittFilter::clone() const {
    return std::make_unique<PrewittFilter>(m_normalize);
}

} // namespace DIPAL
