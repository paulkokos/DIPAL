// src/Filters/StructuringElement.cpp
#include "../../include/DIPAL/Filters/StructuringElement.hpp"

#include <stdexcept>
#include <format>
#include <cmath>

namespace DIPAL {

StructuringElement::StructuringElement(int width, int height, std::vector<bool> mask)
    : m_width(width), m_height(height), m_mask(std::move(mask))
{
    if (width <= 0 || width % 2 == 0)
        throw std::invalid_argument(
            std::format("StructuringElement width must be positive and odd, got {}", width));
    if (height <= 0 || height % 2 == 0)
        throw std::invalid_argument(
            std::format("StructuringElement height must be positive and odd, got {}", height));
    if (static_cast<int>(m_mask.size()) != width * height)
        throw std::invalid_argument(
            std::format("StructuringElement mask size {} != width*height {}",
                        m_mask.size(), width * height));
}

StructuringElement StructuringElement::rect(int width, int height) {
    return StructuringElement(width, height,
                              std::vector<bool>(static_cast<size_t>(width * height), true));
}

StructuringElement StructuringElement::ellipse(int width, int height) {
    if (width <= 0 || width % 2 == 0)
        throw std::invalid_argument(
            std::format("Ellipse SE width must be positive and odd, got {}", width));
    if (height <= 0 || height % 2 == 0)
        throw std::invalid_argument(
            std::format("Ellipse SE height must be positive and odd, got {}", height));

    const float cx = (width  - 1) / 2.0f;
    const float cy = (height - 1) / 2.0f;
    std::vector<bool> mask(static_cast<size_t>(width * height), false);

    for (int r = 0; r < height; ++r) {
        for (int c = 0; c < width; ++c) {
            const float dx = (c - cx) / cx;
            const float dy = (r - cy) / cy;
            if (dx * dx + dy * dy <= 1.0f)
                mask[static_cast<size_t>(r * width + c)] = true;
        }
    }
    return StructuringElement(width, height, std::move(mask));
}

StructuringElement StructuringElement::cross(int size) {
    if (size <= 0 || size % 2 == 0)
        throw std::invalid_argument(
            std::format("Cross SE size must be positive and odd, got {}", size));

    std::vector<bool> mask(static_cast<size_t>(size * size), false);
    const int half = size / 2;
    for (int i = 0; i < size; ++i) {
        mask[static_cast<size_t>(half * size + i)] = true; // middle row
        mask[static_cast<size_t>(i    * size + half)] = true; // middle col
    }
    return StructuringElement(size, size, std::move(mask));
}

StructuringElement StructuringElement::diamond(int size) {
    if (size <= 0 || size % 2 == 0)
        throw std::invalid_argument(
            std::format("Diamond SE size must be positive and odd, got {}", size));

    const int half = size / 2;
    std::vector<bool> mask(static_cast<size_t>(size * size), false);
    for (int r = 0; r < size; ++r) {
        for (int c = 0; c < size; ++c) {
            if (std::abs(r - half) + std::abs(c - half) <= half)
                mask[static_cast<size_t>(r * size + c)] = true;
        }
    }
    return StructuringElement(size, size, std::move(mask));
}

StructuringElement StructuringElement::defaultElement() {
    return rect(3, 3);
}

} // namespace DIPAL
