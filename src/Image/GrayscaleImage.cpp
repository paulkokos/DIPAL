// src/Image/GrayscaleImage.cpp
#include "../../include/DIPAL/Image/GrayscaleImage.hpp"

#include <algorithm>
#include <stdexcept>

namespace DIPAL {

GrayscaleImage::GrayscaleImage(int width, int height) 
    : Image(width, height, Type::Grayscale) {}

Result<uint8_t> GrayscaleImage::getPixel(int x, int y) const {
    if (!isValidCoordinate(x, y)) {
        return makeErrorResult<uint8_t>(ErrorCode::OutOfRange, "Pixel coordinates out of range");
    }

    size_t idx = static_cast<size_t>(y) * m_width + static_cast<size_t>(x);
    return makeSuccessResult(m_data[idx]);
}

VoidResult GrayscaleImage::setPixel(int x, int y, uint8_t value) {
    if (!isValidCoordinate(x, y)) {
        return makeVoidErrorResult(ErrorCode::OutOfRange, "Pixel coordinates out of range");
    }

    size_t idx = static_cast<size_t>(y) * m_width + static_cast<size_t>(x);
    m_data[idx] = value;
    
    return makeVoidSuccessResult();
}

std::unique_ptr<Image> GrayscaleImage::clone() const {
    auto cloned = std::make_unique<GrayscaleImage>(m_width, m_height);
    std::copy(m_data.begin(), m_data.end(), cloned->m_data.begin());
    return cloned;
}

std::span<const uint8_t> GrayscaleImage::getRow(int y) const {
    if (y < 0 || y >= m_height) {
        return std::span<const uint8_t>();  // Return empty span for invalid row
    }
    
    const uint8_t* rowStart = m_data.data() + static_cast<size_t>(y) * m_width;
    return std::span<const uint8_t>(rowStart, m_width);
}

std::span<uint8_t> GrayscaleImage::getRow(int y) {
    if (y < 0 || y >= m_height) {
        return std::span<uint8_t>();  // Return empty span for invalid row
    }
    
    uint8_t* rowStart = m_data.data() + static_cast<size_t>(y) * m_width;
    return std::span<uint8_t>(rowStart, m_width);
}

} // namespace DIPAL
