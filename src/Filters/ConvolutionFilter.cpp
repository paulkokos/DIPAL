// src/Filters/ConvolutionFilter.cpp
#include "../../include/DIPAL/Filters/ConvolutionFilter.hpp"
#include "../../include/DIPAL/Image/GrayscaleImage.hpp"
#include "../../include/DIPAL/Image/ColorImage.hpp"
#include "../../include/DIPAL/Image/ImageFactory.hpp"

#include <algorithm>
#include <cmath>
#include <format>

namespace DIPAL {

ConvolutionFilter::ConvolutionFilter(Kernel kernel, BorderMode borderMode, std::string name)
    : m_kernel(std::move(kernel))
    , m_borderMode(borderMode)
    , m_name(std::move(name))
{}

std::string_view ConvolutionFilter::getName() const {
    return m_name;
}

std::unique_ptr<FilterStrategy> ConvolutionFilter::clone() const {
    return std::make_unique<ConvolutionFilter>(m_kernel, m_borderMode, m_name);
}

int ConvolutionFilter::resolveCoord(int coord, int size) const noexcept {
    if (coord >= 0 && coord < size)
        return coord;

    switch (m_borderMode) {
        case BorderMode::Clamp:
            return std::clamp(coord, 0, size - 1);

        case BorderMode::Reflect: {
            if (coord < 0)
                return -coord - 1;
            // coord >= size
            return 2 * size - coord - 1;
        }

        case BorderMode::Wrap: {
            int r = coord % size;
            return r < 0 ? r + size : r;
        }

        case BorderMode::Zero:
            return -1; // sentinel: use 0.0f for this pixel
    }
    return std::clamp(coord, 0, size - 1);
}

Result<std::unique_ptr<Image>> ConvolutionFilter::apply(const Image& image) const {
    const int width  = image.getWidth();
    const int height = image.getHeight();

    if (width == 0 || height == 0) {
        return makeErrorResult<std::unique_ptr<Image>>(
            ErrorCode::InvalidParameter,
            "Cannot apply convolution to an empty image");
    }

    const int halfW = m_kernel.getHalfWidth();
    const int halfH = m_kernel.getHalfHeight();
    const int kW    = m_kernel.getWidth();
    const int kH    = m_kernel.getHeight();

    try {
        if (image.getType() == Image::Type::Grayscale) {
            const auto& src = static_cast<const GrayscaleImage&>(image);

            auto outResult = ImageFactory::createGrayscale(width, height);
            if (!outResult)
                return makeErrorResult<std::unique_ptr<Image>>(
                    outResult.error().code(), outResult.error().message());
            auto& out = *outResult.value();

            for (int y = 0; y < height; ++y) {
                for (int x = 0; x < width; ++x) {
                    float acc = 0.0f;
                    for (int ky = 0; ky < kH; ++ky) {
                        for (int kx = 0; kx < kW; ++kx) {
                            const int sy = resolveCoord(y + ky - halfH, height);
                            const int sx = resolveCoord(x + kx - halfW, width);

                            float pix = 0.0f;
                            if (sy >= 0 && sx >= 0) {
                                auto pr = src.getPixel(sx, sy);
                                if (!pr)
                                    return makeErrorResult<std::unique_ptr<Image>>(
                                        pr.error().code(), pr.error().message());
                                pix = static_cast<float>(pr.value());
                            }
                            acc += m_kernel.at(ky, kx) * pix;
                        }
                    }

                    const auto val = static_cast<uint8_t>(
                        std::clamp(std::round(acc), 0.0f, 255.0f));
                    auto sr = out.setPixel(x, y, val);
                    if (!sr)
                        return makeErrorResult<std::unique_ptr<Image>>(
                            sr.error().code(), sr.error().message());
                }
            }
            return makeSuccessResult<std::unique_ptr<Image>>(std::move(outResult.value()));
        }

        if (image.getType() == Image::Type::RGB || image.getType() == Image::Type::RGBA) {
            const auto& src = static_cast<const ColorImage&>(image);
            const bool  hasAlpha = (image.getType() == Image::Type::RGBA);

            auto outResult = ImageFactory::createColor(width, height, hasAlpha);
            if (!outResult)
                return makeErrorResult<std::unique_ptr<Image>>(
                    outResult.error().code(), outResult.error().message());
            auto& out = *outResult.value();

            for (int y = 0; y < height; ++y) {
                for (int x = 0; x < width; ++x) {
                    float accR = 0.0f, accG = 0.0f, accB = 0.0f;

                    for (int ky = 0; ky < kH; ++ky) {
                        for (int kx = 0; kx < kW; ++kx) {
                            const int sy = resolveCoord(y + ky - halfH, height);
                            const int sx = resolveCoord(x + kx - halfW, width);

                            float pr = 0.0f, pg = 0.0f, pb = 0.0f;
                            if (sy >= 0 && sx >= 0) {
                                uint8_t r, g, b, a;
                                auto gr = src.getPixel(sx, sy, r, g, b, a);
                                if (!gr)
                                    return makeErrorResult<std::unique_ptr<Image>>(
                                        gr.error().code(), gr.error().message());
                                pr = static_cast<float>(r);
                                pg = static_cast<float>(g);
                                pb = static_cast<float>(b);
                            }
                            const float w = m_kernel.at(ky, kx);
                            accR += w * pr;
                            accG += w * pg;
                            accB += w * pb;
                        }
                    }

                    // Fetch original alpha for pass-through
                    uint8_t origR, origG, origB, origA;
                    auto ga = src.getPixel(x, y, origR, origG, origB, origA);
                    if (!ga)
                        return makeErrorResult<std::unique_ptr<Image>>(
                            ga.error().code(), ga.error().message());

                    const uint8_t outR = static_cast<uint8_t>(
                        std::clamp(std::round(accR), 0.0f, 255.0f));
                    const uint8_t outG = static_cast<uint8_t>(
                        std::clamp(std::round(accG), 0.0f, 255.0f));
                    const uint8_t outB = static_cast<uint8_t>(
                        std::clamp(std::round(accB), 0.0f, 255.0f));

                    auto sr = out.setPixel(x, y, outR, outG, outB, origA);
                    if (!sr)
                        return makeErrorResult<std::unique_ptr<Image>>(
                            sr.error().code(), sr.error().message());
                }
            }
            return makeSuccessResult<std::unique_ptr<Image>>(std::move(outResult.value()));
        }

        return makeErrorResult<std::unique_ptr<Image>>(
            ErrorCode::UnsupportedFormat,
            std::format("ConvolutionFilter: unsupported image type {}",
                        static_cast<int>(image.getType())));

    } catch (const std::exception& e) {
        return makeErrorResult<std::unique_ptr<Image>>(
            ErrorCode::ProcessingFailed,
            std::format("ConvolutionFilter failed: {}", e.what()));
    }
}

} // namespace DIPAL
