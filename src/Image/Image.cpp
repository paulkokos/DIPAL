// src/Image/Image.cpp
#include "../../include/DIPAL/Image/Image.hpp"

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <memory>
#include <stdexcept>
#include <format>
#include <span>

namespace DIPAL {

Image::Image()
    : m_width(0), m_height(0), m_type(Type::Grayscale), m_channels(0), m_bytesPerPixel(0) {}

Image::Image(int width, int height, Type type) : m_width(width), m_height(height), m_type(type) {
    if (width <= 0 || height <= 0) {
        throw std::invalid_argument("Image dimensions must be positive");
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
    m_data.resize(static_cast<size_t>(width) * height * m_bytesPerPixel);
}

int Image::getWidth() const {
    return m_width;
}

int Image::getHeight()  const{
    return m_height;
}

Image::Type Image::getType() const  {
    return m_type;
}

bool Image::isEmpty() const  {
    return m_data.empty() || m_width <= 0 || m_height <= 0;
}

std::span<const std::uint8_t> Image::getDataSpan() const noexcept{
    return std::span<const std::uint8_t>(m_data);
}

std::span<std::uint8_t> Image::getDataSpan()  {
    return std::span<std::uint8_t>(m_data);
}

const std::uint8_t* Image::getData() const  {
    return m_data.data();
}

std::uint8_t* Image::getData() {
    return m_data.data();
}

int Image::getChannels() const  {
    return m_channels;
}

int Image::getBytesPerPixel() const  {
    return m_bytesPerPixel;
}

size_t Image::getDataSize() const  {
    return m_data.size();
}

std::string Image::toString() const {
    const char* typeStr;
    switch (m_type) {
        case Type::Binary:
            typeStr = "Binary";
            break;
        case Type::Grayscale:
            typeStr = "Grayscale";
            break;
        case Type::RGB:
            typeStr = "RGB";
            break;
        case Type::RGBA:
            typeStr = "RGBA";
            break;
        default:
            typeStr = "Unknown";
    }
    
    return std::format("Image({}x{}, type={}, channels={})",
                     m_width, m_height, typeStr, m_channels);
}

std::unique_ptr<Image> Image::clone() const {
    auto cloned = std::make_unique<Image>(m_width, m_height, m_type);
    std::copy(m_data.begin(), m_data.end(), cloned->m_data.begin());
    return cloned;
}

bool Image::isValidCoordinate(int x, int y) const  {
    return x >= 0 && x < m_width && y >= 0 && y < m_height;
}

std::size_t Image::getPixelIndex(int x, int y) const {
    if (!isValidCoordinate(x, y)) {
        throw std::out_of_range("Pixel coordinates out of range");
    }
    return static_cast<size_t>(y) * m_width * m_bytesPerPixel + 
           static_cast<size_t>(x) * m_bytesPerPixel;
}

} // namespace DIPAL
