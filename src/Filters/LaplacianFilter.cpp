// src/Filters/LaplacianFilter.cpp
#include "../../include/DIPAL/Filters/LaplacianFilter.hpp"
#include "../../include/DIPAL/Filters/ConvolutionFilter.hpp"
#include "../../include/DIPAL/Filters/Kernel.hpp"
#include "../../include/DIPAL/Image/GrayscaleImage.hpp"
#include "../../include/DIPAL/Image/ColorImage.hpp"
#include "../../include/DIPAL/Image/ImageFactory.hpp"

#include <algorithm>
#include <cmath>
#include <format>

namespace DIPAL {

LaplacianFilter::LaplacianFilter(bool use8Neighbor, bool normalize)
    : m_use8Neighbor(use8Neighbor), m_normalize(normalize)
{}

Result<std::unique_ptr<Image>> LaplacianFilter::apply(const Image& image) const {
    const int width  = image.getWidth();
    const int height = image.getHeight();

    if (width == 0 || height == 0) {
        return makeErrorResult<std::unique_ptr<Image>>(
            ErrorCode::InvalidParameter,
            "Cannot apply Laplacian filter to an empty image");
    }

    try {
        // Convert colour → grayscale if necessary
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
                std::format("LaplacianFilter: unsupported image type {}",
                            static_cast<int>(image.getType())));
        }

        // Apply Laplacian kernel via ConvolutionFilter
        const Kernel k = m_use8Neighbor ? Kernel::laplacian8() : Kernel::laplacian4();
        ConvolutionFilter conv(k, BorderMode::Clamp, "LaplacianFilter");
        auto convResult = conv.apply(*gray);
        if (!convResult)
            return convResult;

        if (!m_normalize)
            return convResult;

        // Normalize to [0, 255]
        auto& out = static_cast<GrayscaleImage&>(*convResult.value());
        int maxVal = 0;
        for (int y = 0; y < height; ++y)
            for (int x = 0; x < width; ++x) {
                auto pr = out.getPixel(x, y);
                if (pr) maxVal = std::max(maxVal, static_cast<int>(pr.value()));
            }

        if (maxVal > 0 && maxVal < 255) {
            for (int y = 0; y < height; ++y)
                for (int x = 0; x < width; ++x) {
                    auto pr = out.getPixel(x, y);
                    if (!pr) continue;
                    auto sr = out.setPixel(x, y,
                        static_cast<uint8_t>((pr.value() * 255) / maxVal));
                    if (!sr)
                        return makeErrorResult<std::unique_ptr<Image>>(
                            sr.error().code(), sr.error().message());
                }
        }

        return convResult;

    } catch (const std::exception& e) {
        return makeErrorResult<std::unique_ptr<Image>>(
            ErrorCode::ProcessingFailed,
            std::format("LaplacianFilter failed: {}", e.what()));
    }
}

std::string_view LaplacianFilter::getName() const { return "LaplacianFilter"; }

std::unique_ptr<FilterStrategy> LaplacianFilter::clone() const {
    return std::make_unique<LaplacianFilter>(m_use8Neighbor, m_normalize);
}

} // namespace DIPAL
