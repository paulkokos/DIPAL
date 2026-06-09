// src/Filters/BilateralFilter.cpp
#include "../../include/DIPAL/Filters/BilateralFilter.hpp"
#include "../../include/DIPAL/Image/GrayscaleImage.hpp"
#include "../../include/DIPAL/Image/ColorImage.hpp"
#include "../../include/DIPAL/Image/ImageFactory.hpp"

#include <algorithm>
#include <cmath>
#include <format>
#include <vector>

namespace DIPAL {

BilateralFilter::BilateralFilter(int diameter, float sigmaColor, float sigmaSpace)
    : m_diameter(diameter), m_sigmaColor(sigmaColor), m_sigmaSpace(sigmaSpace)
{
    if (sigmaColor <= 0.0f)
        throw std::invalid_argument(
            std::format("BilateralFilter: sigmaColor must be positive, got {}", sigmaColor));
    if (sigmaSpace <= 0.0f)
        throw std::invalid_argument(
            std::format("BilateralFilter: sigmaSpace must be positive, got {}", sigmaSpace));
}

Result<std::unique_ptr<Image>> BilateralFilter::apply(const Image& image) const {
    const int width  = image.getWidth();
    const int height = image.getHeight();

    if (width == 0 || height == 0)
        return makeErrorResult<std::unique_ptr<Image>>(
            ErrorCode::InvalidParameter, "Cannot apply bilateral filter to empty image");

    // Determine actual diameter
    int d = m_diameter;
    if (d <= 0)
        d = static_cast<int>(std::ceil(2.0f * m_sigmaSpace * 3.0f)) | 1;
    if (d % 2 == 0) ++d;
    const int half = d / 2;

    // Pre-compute spatial weights (only depend on distance, same for every pixel)
    const float twoSigSpace2 = 2.0f * m_sigmaSpace * m_sigmaSpace;
    std::vector<float> spatialW(static_cast<size_t>(d * d));
    for (int dy = -half; dy <= half; ++dy)
        for (int dx = -half; dx <= half; ++dx)
            spatialW[static_cast<size_t>((dy + half) * d + (dx + half))] =
                std::exp(-(dx * dx + dy * dy) / twoSigSpace2);

    const float twoSigColor2 = 2.0f * m_sigmaColor * m_sigmaColor;

    try {
        if (image.getType() == Image::Type::Grayscale) {
            const auto& src = static_cast<const GrayscaleImage&>(image);
            auto outR = ImageFactory::createGrayscale(width, height);
            if (!outR) return makeErrorResult<std::unique_ptr<Image>>(
                outR.error().code(), outR.error().message());
            auto& out = *outR.value();

            for (int y = 0; y < height; ++y) {
                for (int x = 0; x < width; ++x) {
                    auto cp = src.getPixel(x, y);
                    if (!cp) return makeErrorResult<std::unique_ptr<Image>>(
                        cp.error().code(), cp.error().message());
                    const float center = static_cast<float>(cp.value());

                    float wSum = 0.0f, valSum = 0.0f;
                    for (int dy = -half; dy <= half; ++dy) {
                        for (int dx = -half; dx <= half; ++dx) {
                            const int sy = std::clamp(y + dy, 0, height - 1);
                            const int sx = std::clamp(x + dx, 0, width  - 1);
                            auto np = src.getPixel(sx, sy);
                            if (!np) continue;
                            const float n = static_cast<float>(np.value());
                            const float diff = n - center;
                            const float rangeW = std::exp(-(diff * diff) / twoSigColor2);
                            const float w = spatialW[static_cast<size_t>(
                                (dy + half) * d + (dx + half))] * rangeW;
                            valSum += w * n;
                            wSum   += w;
                        }
                    }
                    const uint8_t val = static_cast<uint8_t>(
                        std::clamp(std::round(valSum / wSum), 0.0f, 255.0f));
                    auto sr = out.setPixel(x, y, val);
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
                    uint8_t cr, cg, cb, ca;
                    auto cp = src.getPixel(x, y, cr, cg, cb, ca);
                    if (!cp) return makeErrorResult<std::unique_ptr<Image>>(
                        cp.error().code(), cp.error().message());
                    const float fc = static_cast<float>(cr);
                    const float gc = static_cast<float>(cg);
                    const float bc = static_cast<float>(cb);

                    float wSum = 0.0f, rSum = 0.0f, gSum = 0.0f, bSum = 0.0f;
                    for (int dy = -half; dy <= half; ++dy) {
                        for (int dx = -half; dx <= half; ++dx) {
                            const int sy = std::clamp(y + dy, 0, height - 1);
                            const int sx = std::clamp(x + dx, 0, width  - 1);
                            uint8_t nr, ng, nb, na;
                            auto np = src.getPixel(sx, sy, nr, ng, nb, na);
                            if (!np) continue;
                            const float dr = nr - fc, dg = ng - gc, db = nb - bc;
                            const float colorDist2 = dr*dr + dg*dg + db*db;
                            const float rangeW = std::exp(-colorDist2 / twoSigColor2);
                            const float w = spatialW[static_cast<size_t>(
                                (dy + half) * d + (dx + half))] * rangeW;
                            rSum += w * nr;
                            gSum += w * ng;
                            bSum += w * nb;
                            wSum += w;
                        }
                    }
                    const auto toU8 = [](float v) {
                        return static_cast<uint8_t>(std::clamp(std::round(v), 0.0f, 255.0f));
                    };
                    auto sr = out.setPixel(x, y, toU8(rSum/wSum), toU8(gSum/wSum), toU8(bSum/wSum), ca);
                    if (!sr) return makeErrorResult<std::unique_ptr<Image>>(
                        sr.error().code(), sr.error().message());
                }
            }
            return makeSuccessResult<std::unique_ptr<Image>>(std::move(outR.value()));
        }

        return makeErrorResult<std::unique_ptr<Image>>(
            ErrorCode::UnsupportedFormat,
            std::format("BilateralFilter: unsupported image type {}",
                        static_cast<int>(image.getType())));

    } catch (const std::exception& e) {
        return makeErrorResult<std::unique_ptr<Image>>(
            ErrorCode::ProcessingFailed,
            std::format("BilateralFilter failed: {}", e.what()));
    }
}

std::string_view BilateralFilter::getName() const { return "BilateralFilter"; }

std::unique_ptr<FilterStrategy> BilateralFilter::clone() const {
    return std::make_unique<BilateralFilter>(m_diameter, m_sigmaColor, m_sigmaSpace);
}

} // namespace DIPAL
