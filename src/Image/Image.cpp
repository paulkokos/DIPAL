#include "../../include/DIPAL/Image/Image.hpp"

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <memory>
#include <stdexcept>

namespace DIPAL {
// Image class implementation
Image::Image()
    : m_width(0), m_height(0), m_type(Type::Grayscale), m_channels(0), m_bytesPerPixel(0) {}

Image::Image(int width, int height, Type type) : m_width(width), m_height(height), m_type(type) {
    if (width < 0 || height < 0) {
        throw std::invalid_argument("Image Dimentions must be positive");
    }

    // Set channel and bytes per pixel based on type

    switch (type) {
        case Type::Binary:
            m_channels = 1;
            m_bytesPerPixel = 1;
            break;
        case Type::Grayscale:
            m_channels = 1;
            m_bytesPerPixel = 1;
            break;
        case Type::RGB:
            m_channels = 3;
            m_bytesPerPixel = 3;
            break;
        case Type::RGBA:
            m_channels = 4;
            m_bytesPerPixel = 4;
            break;
        default:
            throw std::invalid_argument("Invalid image type");
    }

    // Allocate memory for the image data
    m_data.resize(width * height * m_bytesPerPixel);
}
int Image::getWidth() const {
    return m_width;
}

int Image::getHeight() const {
    return m_height;
}

Image::Type Image::getType() const {
    return m_type;
}

bool Image::isEmpty() const {
    return m_data.empty() || m_width == 0 || m_height == 0;
}

const uint8_t* Image::getData() const {
    return m_data.data();
}

uint8_t* Image::getData() {
    return m_data.data();
}

int Image::getChannels() const {
    return m_channels;
}
int Image::getBytesPerPixel() const {
    return m_bytesPerPixel;
}

size_t Image::getDataSize() const {
    return m_data.size();
}

std::unique_ptr<Image> Image::clone() const {
    auto cloned = std::make_unique<Image>(m_width, m_height, m_type);
    std::copy(m_data.begin(), m_data.end(), cloned->m_data.begin());
    return cloned;
}
}  // namespace DIPAL
