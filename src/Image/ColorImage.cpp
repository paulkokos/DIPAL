// src/Image/ColorImage.cpp
#include "../../include/DIPAL/Image/ColorImage.hpp"

#include <iostream>
#include <stdexcept>
namespace DIPAL {

ColorImage::ColorImage(int width, int height, bool hasAlpha)
    : Image(width, height, hasAlpha ? Type::RGBA : Type::RGB) {}

void ColorImage::getPixel(int x, int y, uint8_t& r, uint8_t& g, uint8_t& b, uint8_t& a) const {
    if (x < 0 || x >= m_width || y < 0 || y >= m_height) {
        throw std::out_of_range("Pixel coordinates out of range");
    }

    size_t idx = (y * m_width + x) * m_bytesPerPixel;
    r = m_data[idx];
    g = m_data[idx + 1];
    b = m_data[idx + 2];

    if (hasAlpha() && idx + 3 < m_data.size()) {
        a = m_data[idx + 3];
    } else {
        a = 255;  // Default alpha is fully opaque
    }
}

void ColorImage::setPixel(int x, int y, uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
    if (x < 0 || x >= m_width || y < 0 || y >= m_height) {
        throw std::out_of_range("Pixel coordinates out of range");
    }

    size_t idx = (y * m_width + x) * m_bytesPerPixel;
    m_data[idx] = r;
    m_data[idx + 1] = g;
    m_data[idx + 2] = b;

    if (hasAlpha() && idx + 3 < m_data.size()) {
        m_data[idx + 3] = a;
    }
}

bool ColorImage::hasAlpha() const {
    return m_type == Type::RGBA;
}

std::unique_ptr<Image> ColorImage::clone() const {
    auto cloned = std::make_unique<ColorImage>(m_width, m_height, hasAlpha());
    std::copy(m_data.begin(), m_data.end(), cloned->m_data.begin());
    return cloned;
}

}  // namespace DIPAL
