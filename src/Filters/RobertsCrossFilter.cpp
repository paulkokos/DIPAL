// src/Filters/RobertsCrossFilter.cpp
#include "../../include/DIPAL/Filters/RobertsCrossFilter.hpp"
#include "../../include/DIPAL/Filters/Kernel.hpp"
#include "../../include/DIPAL/Image/GrayscaleImage.hpp"
#include "../../include/DIPAL/Image/ColorImage.hpp"
#include "../../include/DIPAL/Image/ImageFactory.hpp"

#include <algorithm>
#include <cmath>
#include <format>
#include <vector>

namespace DIPAL {

RobertsCrossFilter::RobertsCrossFilter(bool normalize) : m_normalize(normalize) {}

Result<std::unique_ptr<Image>> RobertsCrossFilter::apply(const Image& image) const {
    const int width  = image.getWidth();
    const int height = image.getHeight();

    if (width == 0 || height == 0) {
        return makeErrorResult<std::unique_ptr<Image>>(
            ErrorCode::InvalidParameter,
            "Cannot apply Roberts Cross filter to an empty image");
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
                std::format("RobertsCrossFilter: unsupported image type {}",
                            static_cast<int>(image.getType())));
        }

        // Roberts Cross kernels (embedded in 3x3):
        //  kx = [ 1  0  0]    ky = [ 0  1  0]
        //       [ 0 -1  0]         [-1  0  0]
        //       [ 0  0  0]         [ 0  0  0]
        // Effectively: gx = I(x,y) - I(x+1,y+1)
        //              gy = I(x+1,y) - I(x,y+1)

        std::vector<float> magnitudes(static_cast<size_t>(width * height));
        float maxMag = 0.0f;

        for (int y = 0; y < height; ++y) {
            for (int x = 0; x < width; ++x) {
                const int x1 = std::min(x + 1, width  - 1);
                const int y1 = std::min(y + 1, height - 1);

                auto p00r = gray->getPixel(x,  y);
                auto p11r = gray->getPixel(x1, y1);
                auto p10r = gray->getPixel(x1, y);
                auto p01r = gray->getPixel(x,  y1);

                if (!p00r || !p11r || !p10r || !p01r)
                    return makeErrorResult<std::unique_ptr<Image>>(
                        ErrorCode::ProcessingFailed,
                        "RobertsCrossFilter: failed to read pixel");

                const float p00 = static_cast<float>(p00r.value());
                const float p11 = static_cast<float>(p11r.value());
                const float p10 = static_cast<float>(p10r.value());
                const float p01 = static_cast<float>(p01r.value());

                const float gx = p00 - p11;
                const float gy = p10 - p01;
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
            std::format("RobertsCrossFilter failed: {}", e.what()));
    }
}

std::string_view RobertsCrossFilter::getName() const { return "RobertsCrossFilter"; }

std::unique_ptr<FilterStrategy> RobertsCrossFilter::clone() const {
    return std::make_unique<RobertsCrossFilter>(m_normalize);
}

} // namespace DIPAL
