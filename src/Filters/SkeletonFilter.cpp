// src/Filters/SkeletonFilter.cpp
// Zhang-Suen thinning algorithm
#include "../../include/DIPAL/Filters/SkeletonFilter.hpp"
#include "../../include/DIPAL/Image/GrayscaleImage.hpp"
#include "../../include/DIPAL/Image/ColorImage.hpp"
#include "../../include/DIPAL/Image/ImageFactory.hpp"

#include <algorithm>
#include <format>
#include <vector>

namespace DIPAL {

SkeletonFilter::SkeletonFilter(uint8_t threshold) : m_threshold(threshold) {}

Result<std::unique_ptr<Image>> SkeletonFilter::apply(const Image& image) const {
    const int width  = image.getWidth();
    const int height = image.getHeight();

    if (width == 0 || height == 0)
        return makeErrorResult<std::unique_ptr<Image>>(
            ErrorCode::InvalidParameter, "Cannot skeletonise an empty image");

    try {
        // Obtain grayscale
        std::unique_ptr<GrayscaleImage> gray;
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
                std::format("SkeletonFilter: unsupported image type {}",
                            static_cast<int>(image.getType())));
        }

        // Binarise into a flat bool grid: true = foreground
        std::vector<uint8_t> grid(static_cast<size_t>(width * height), 0);
        for (int y = 0; y < height; ++y) {
            for (int x = 0; x < width; ++x) {
                auto pr = gray->getPixel(x, y);
                if (!pr) return makeErrorResult<std::unique_ptr<Image>>(
                    pr.error().code(), pr.error().message());
                grid[static_cast<size_t>(y * width + x)] = (pr.value() >= m_threshold) ? 1 : 0;
            }
        }

        // Zhang-Suen iterative thinning
        // Neighbours in order: P2(top), P3(top-right), P4(right), P5(bottom-right),
        //                      P6(bottom), P7(bottom-left), P8(left), P9(top-left)
        const int dx[8] = { 0,  1,  1,  1,  0, -1, -1, -1};
        const int dy[8] = {-1, -1,  0,  1,  1,  1,  0, -1};

        auto get = [&](int x, int y) -> uint8_t {
            if (x < 0 || x >= width || y < 0 || y >= height) return 0;
            return grid[static_cast<size_t>(y * width + x)];
        };

        bool changed = true;
        while (changed) {
            changed = false;
            for (int iter = 0; iter < 2; ++iter) {
                std::vector<size_t> toDelete;
                for (int y = 1; y < height - 1; ++y) {
                    for (int x = 1; x < width - 1; ++x) {
                        if (!get(x, y)) continue;

                        // Collect 8-neighbours in order P2..P9
                        const uint8_t n[8] = {
                            get(x + dx[0], y + dy[0]),
                            get(x + dx[1], y + dy[1]),
                            get(x + dx[2], y + dy[2]),
                            get(x + dx[3], y + dy[3]),
                            get(x + dx[4], y + dy[4]),
                            get(x + dx[5], y + dy[5]),
                            get(x + dx[6], y + dy[6]),
                            get(x + dx[7], y + dy[7]),
                        };

                        // B(P) = number of non-zero neighbours
                        int B = 0;
                        for (auto v : n) B += v;
                        if (B < 2 || B > 6) continue;

                        // A(P) = number of 0→1 transitions in cyclic order P2..P9
                        int A = 0;
                        for (int k = 0; k < 8; ++k)
                            if (!n[k] && n[(k + 1) % 8]) ++A;
                        if (A != 1) continue;

                        if (iter == 0) {
                            // Sub-iteration 1: P2·P4·P6 = 0 AND P4·P6·P8 = 0
                            if (n[0] * n[2] * n[4] != 0) continue;
                            if (n[2] * n[4] * n[6] != 0) continue;
                        } else {
                            // Sub-iteration 2: P2·P4·P8 = 0 AND P2·P6·P8 = 0
                            if (n[0] * n[2] * n[6] != 0) continue;
                            if (n[0] * n[4] * n[6] != 0) continue;
                        }

                        toDelete.push_back(static_cast<size_t>(y * width + x));
                    }
                }
                for (auto idx : toDelete) {
                    grid[idx] = 0;
                    changed = true;
                }
            }
        }

        // Write result
        auto outR = ImageFactory::createGrayscale(width, height);
        if (!outR) return makeErrorResult<std::unique_ptr<Image>>(
            outR.error().code(), outR.error().message());
        auto& out = *outR.value();

        for (int y = 0; y < height; ++y) {
            for (int x = 0; x < width; ++x) {
                uint8_t val = grid[static_cast<size_t>(y * width + x)] ? 255 : 0;
                auto sr = out.setPixel(x, y, val);
                if (!sr) return makeErrorResult<std::unique_ptr<Image>>(
                    sr.error().code(), sr.error().message());
            }
        }

        return makeSuccessResult<std::unique_ptr<Image>>(std::move(outR.value()));

    } catch (const std::exception& e) {
        return makeErrorResult<std::unique_ptr<Image>>(
            ErrorCode::ProcessingFailed,
            std::format("SkeletonFilter failed: {}", e.what()));
    }
}

std::string_view SkeletonFilter::getName() const { return "SkeletonFilter"; }

std::unique_ptr<FilterStrategy> SkeletonFilter::clone() const {
    return std::make_unique<SkeletonFilter>(m_threshold);
}

} // namespace DIPAL
