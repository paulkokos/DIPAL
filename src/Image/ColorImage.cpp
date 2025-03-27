// src/Image/ColorImage.cpp
#include "../../include/DIPAL/Image/ColorImage.hpp"
#include "../../include/DIPAL/Image/GrayscaleImage.hpp"

#include <stdexcept>

namespace DIPAL {

ColorImage::ColorImage(int width, int height, bool hasAlpha)
    : Image(width, height, hasAlpha ? Type::RGBA : Type::RGB) {}

VoidResult ColorImage::getPixel(int x, int y, uint8_t& r, uint8_t& g, uint8_t& b, uint8_t& a) const {
    if (!isValidCoordinate(x, y)) {
        return makeVoidErrorResult(ErrorCode::OutOfRange, "Pixel coordinates out of range");
    }

    size_t idx = static_cast<size_t>(y) * m_width * m_bytesPerPixel + 
                 static_cast<size_t>(x) * m_bytesPerPixel;
                 
    r = m_data[idx];
    g = m_data[idx + 1];
    b = m_data[idx + 2];

    if (hasAlpha() && idx + 3 < m_data.size()) {
        a = m_data[idx + 3];
    } else {
        a = 255;  // Default alpha is fully opaque
    }
    
    return makeVoidSuccessResult();
}

VoidResult ColorImage::setPixel(int x, int y, uint8_t r, uint8_t g, uint8_t b, uint8_t a) {
    if (!isValidCoordinate(x, y)) {
        return makeVoidErrorResult(ErrorCode::OutOfRange, "Pixel coordinates out of range");
    }

    size_t idx = static_cast<size_t>(y) * m_width * m_bytesPerPixel + 
                 static_cast<size_t>(x) * m_bytesPerPixel;
                 
    m_data[idx] = r;
    m_data[idx + 1] = g;
    m_data[idx + 2] = b;

    if (hasAlpha() && idx + 3 < m_data.size()) {
        m_data[idx + 3] = a;
    }
    
    return makeVoidSuccessResult();
}

bool ColorImage::hasAlpha() const noexcept {
    return m_type == Type::RGBA;
}

Result<std::unique_ptr<GrayscaleImage>> ColorImage::getChannel(int channel) const {
    if (channel < 0 || channel >= m_channels) {
        return makeErrorResult<std::unique_ptr<GrayscaleImage>>(
            ErrorCode::InvalidParameter, 
            std::format("Invalid channel index: {}", channel)
        );
    }
    
    auto channelImage = std::make_unique<GrayscaleImage>(m_width, m_height);
    
    for (int y = 0; y < m_height; ++y) {
        for (int x = 0; x < m_width; ++x) {
            size_t srcIdx = static_cast<size_t>(y) * m_width * m_bytesPerPixel + 
                           static_cast<size_t>(x) * m_bytesPerPixel + channel;
            
            auto result = channelImage->setPixel(x, y, m_data[srcIdx]);
            if (!result) {
                return makeErrorResult<std::unique_ptr<GrayscaleImage>>(
                    result.error().code(),
                    result.error().message()
                );
            }
        }
    }
    
    return makeSuccessResult(std::move(channelImage));
}

std::unique_ptr<Image> ColorImage::clone() const {
    auto cloned = std::make_unique<ColorImage>(m_width, m_height, hasAlpha());
    std::copy(m_data.begin(), m_data.end(), cloned->m_data.begin());
    return cloned;
}

} // namespace DIPAL
