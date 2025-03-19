// src/Image/GrayscaleImage.cpp
#include "../../include/DIPAL/Image/GrayscaleImage.hpp"

namespace DIPAL {

GrayscaleImage::GrayscaleImage(int width, int height) : Image(width, height, Type::Grayscale) {}

uint8_t GrayscaleImage::getPixel(int x, int y) const {
    if (x < 0 || x >= m_width || y < 0 || y >= m_height) {
        throw std::out_of_range("Pixel coordinates out of range");
    }

    return m_data[y * m_width + x];
}

void GrayscaleImage::setPixel(int x, int y, uint8_t value) {
    if (x < 0 || x >= m_width || y < 0 || y >= m_height) {
        throw std::out_of_range("Pixel coordinates out of range");
    }

    m_data[y * m_width + x] = value;
}

std::unique_ptr<Image> GrayscaleImage::clone() const {
    auto cloned = std::make_unique<GrayscaleImage>(m_width, m_height);
    std::copy(m_data.begin(), m_data.end(), cloned->m_data.begin());
    return cloned;
}

}  // namespace DIPAL
