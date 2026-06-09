// src/Filters/Kernel.cpp
#include "../../include/DIPAL/Filters/Kernel.hpp"

#include <stdexcept>
#include <format>
#include <cmath>
#include <numbers>

namespace DIPAL {

Kernel::Kernel(int width, int height, std::vector<float> weights)
    : m_width(width), m_height(height), m_weights(std::move(weights))
{
    if (width <= 0 || width % 2 == 0)
        throw std::invalid_argument(
            std::format("Kernel width must be positive and odd, got {}", width));
    if (height <= 0 || height % 2 == 0)
        throw std::invalid_argument(
            std::format("Kernel height must be positive and odd, got {}", height));
    if (static_cast<int>(m_weights.size()) != width * height)
        throw std::invalid_argument(
            std::format("Kernel weight count {} != width*height {}",
                        m_weights.size(), width * height));
}

Kernel Kernel::box(int size) {
    if (size <= 0 || size % 2 == 0)
        throw std::invalid_argument(
            std::format("Box kernel size must be positive and odd, got {}", size));
    const float w = 1.0f / static_cast<float>(size * size);
    return Kernel(size, size, std::vector<float>(static_cast<size_t>(size * size), w));
}

Kernel Kernel::gaussian(float sigma, int size) {
    if (sigma <= 0.0f)
        throw std::invalid_argument(
            std::format("Gaussian sigma must be positive, got {}", sigma));

    if (size <= 0)
        size = static_cast<int>(std::ceil(6.0f * sigma)) | 1; // round up to odd
    if (size % 2 == 0)
        ++size;

    const int half = size / 2;
    const float twoSigSq = 2.0f * sigma * sigma;
    std::vector<float> w(static_cast<size_t>(size * size));
    float sum = 0.0f;

    for (int r = -half; r <= half; ++r) {
        for (int c = -half; c <= half; ++c) {
            float val = std::exp(-(r * r + c * c) / twoSigSq);
            w[static_cast<size_t>((r + half) * size + (c + half))] = val;
            sum += val;
        }
    }
    for (auto& v : w)
        v /= sum;

    return Kernel(size, size, std::move(w));
}

Kernel Kernel::sharpen() {
    return Kernel(3, 3, {
         0.0f, -1.0f,  0.0f,
        -1.0f,  5.0f, -1.0f,
         0.0f, -1.0f,  0.0f
    });
}

Kernel Kernel::laplacian4() {
    return Kernel(3, 3, {
         0.0f,  1.0f,  0.0f,
         1.0f, -4.0f,  1.0f,
         0.0f,  1.0f,  0.0f
    });
}

Kernel Kernel::laplacian8() {
    return Kernel(3, 3, {
         1.0f,  1.0f,  1.0f,
         1.0f, -8.0f,  1.0f,
         1.0f,  1.0f,  1.0f
    });
}

Kernel Kernel::prewittX() {
    return Kernel(3, 3, {
        -1.0f, 0.0f, 1.0f,
        -1.0f, 0.0f, 1.0f,
        -1.0f, 0.0f, 1.0f
    });
}

Kernel Kernel::prewittY() {
    return Kernel(3, 3, {
        -1.0f, -1.0f, -1.0f,
         0.0f,  0.0f,  0.0f,
         1.0f,  1.0f,  1.0f
    });
}

// Roberts Cross kernels are 2x2, which violates the odd-size rule.
// We embed them in a 3x3 with zero-padding so the framework stays uniform.
Kernel Kernel::robertsCrossX() {
    return Kernel(3, 3, {
         1.0f,  0.0f, 0.0f,
         0.0f, -1.0f, 0.0f,
         0.0f,  0.0f, 0.0f
    });
}

Kernel Kernel::robertsCrossY() {
    return Kernel(3, 3, {
         0.0f,  1.0f, 0.0f,
        -1.0f,  0.0f, 0.0f,
         0.0f,  0.0f, 0.0f
    });
}

Kernel Kernel::emboss() {
    return Kernel(3, 3, {
        -2.0f, -1.0f, 0.0f,
        -1.0f,  1.0f, 1.0f,
         0.0f,  1.0f, 2.0f
    });
}

Kernel Kernel::edgeEnhance() {
    return Kernel(3, 3, {
         0.0f,  0.0f,  0.0f,
        -1.0f,  1.0f,  0.0f,
         0.0f,  0.0f,  0.0f
    });
}

} // namespace DIPAL
