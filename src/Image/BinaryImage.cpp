// src/Image/BinaryImage.cpp
#include "../../include/DIPAL/Image/BinaryImage.hpp"
#include "../../include/DIPAL/Image/GrayscaleImage.hpp"

#include <algorithm>
#include <bit>
#include <cmath>
#include <format>

namespace DIPAL {

BinaryImage::BinaryImage(int width, int height) 
    : Image(width, height, Type::Binary) {
    // Binary images are packed with 8 pixels per byte, so we adjust the storage size
    // We need to ceil(width / 8) bytes per row
    int bytesPerRow = getBytesPerRow();
    
    // Reallocate data storage with the correct size
    m_data.resize(static_cast<size_t>(bytesPerRow) * height);
    
    // Override the bytesPerPixel value set by base constructor since we're bit-packed
    m_bytesPerPixel = 0; // Not applicable for bit-packed images
    m_channels = 1;      // Still one channel (binary)
}

Result<bool> BinaryImage::getPixel(int x, int y) const {
    if (!isValidCoordinate(x, y)) {
        return makeErrorResult<bool>(ErrorCode::OutOfRange, "Pixel coordinates out of range");
    }

    int byteIndex = getByteIndex(x, y);
    uint8_t bitMask = getBitMask(x);
    
    // Check if the bit is set (1 = white, 0 = black)
    return makeSuccessResult((m_data[byteIndex] & bitMask) != 0);
}

VoidResult BinaryImage::setPixel(int x, int y, bool value) {
    if (!isValidCoordinate(x, y)) {
        return makeVoidErrorResult(ErrorCode::OutOfRange, "Pixel coordinates out of range");
    }

    int byteIndex = getByteIndex(x, y);
    uint8_t bitMask = getBitMask(x);
    
    if (value) {
        // Set the bit (white pixel)
        m_data[byteIndex] |= bitMask;
    } else {
        // Clear the bit (black pixel)
        m_data[byteIndex] &= ~bitMask;
    }
    
    return makeVoidSuccessResult();
}

std::unique_ptr<Image> BinaryImage::clone() const {
    auto cloned = std::make_unique<BinaryImage>(m_width, m_height);
    std::copy(m_data.begin(), m_data.end(), cloned->m_data.begin());
    return cloned;
}

std::span<const uint8_t> BinaryImage::getRow(int y) const {
    if (y < 0 || y >= m_height) {
        return std::span<const uint8_t>(); // Return empty span for invalid row
    }
    
    int bytesPerRow = getBytesPerRow();
    const uint8_t* rowStart = m_data.data() + static_cast<size_t>(y) * bytesPerRow;
    return std::span<const uint8_t>(rowStart, bytesPerRow);
}

std::span<uint8_t> BinaryImage::getRow(int y) {
    if (y < 0 || y >= m_height) {
        return std::span<uint8_t>(); // Return empty span for invalid row
    }
    
    int bytesPerRow = getBytesPerRow();
    uint8_t* rowStart = m_data.data() + static_cast<size_t>(y) * bytesPerRow;
    return std::span<uint8_t>(rowStart, bytesPerRow);
}

int BinaryImage::getBytesPerRow() const {
    return (m_width + 7) / 8; // Ceiling division for bit packing (8 pixels per byte)
}

VoidResult BinaryImage::invert() {
    // Invert all bits in the image
    for (auto& byte : m_data) {
        byte = ~byte;
    }
    
    // If the image width is not a multiple of 8, we need to clean up the unused bits
    // in the last byte of each row to avoid counting them in operations like countWhitePixels
    int extraBits = m_width % 8;
    if (extraBits != 0) {
        int bytesPerRow = getBytesPerRow();
        uint8_t mask = static_cast<uint8_t>((1 << extraBits) - 1); // Mask for used bits
        
        for (int y = 0; y < m_height; ++y) {
            size_t lastByteIndex = static_cast<size_t>(y) * bytesPerRow + bytesPerRow - 1;
            m_data[lastByteIndex] &= mask; // Zero out unused bits
        }
    }
    
    return makeVoidSuccessResult();
}

VoidResult BinaryImage::fill(bool value) {
    // Set all bytes to 0x00 (all black) or 0xFF (all white)
    std::fill(m_data.begin(), m_data.end(), value ? 0xFF : 0x00);
    
    // If the image width is not a multiple of 8, we need to clean up the unused bits
    // in the last byte of each row
    int extraBits = m_width % 8;
    if (extraBits != 0) {
        int bytesPerRow = getBytesPerRow();
        uint8_t mask = static_cast<uint8_t>((1 << extraBits) - 1); // Mask for used bits
        
        for (int y = 0; y < m_height; ++y) {
            size_t lastByteIndex = static_cast<size_t>(y) * bytesPerRow + bytesPerRow - 1;
            if (value) {
                m_data[lastByteIndex] &= mask; // Keep the used bits, zero out unused bits
            } else {
                // Already zeroed
            }
        }
    }
    
    return makeVoidSuccessResult();
}

size_t BinaryImage::countWhitePixels() const {
    size_t count = 0;
    
    // For each byte, count the number of bits set to 1 (white pixels)
    for (size_t i = 0; i < m_data.size(); ++i) {
        count += std::popcount(m_data[i]);
    }
    
    // If the image width is not a multiple of 8, we need to subtract the count for unused bits
    // in the last byte of each row
    int extraBits = m_width % 8;
    if (extraBits != 0) {
        [[maybe_unused]] int unusedBits = 8 - extraBits;
        int bytesPerRow = getBytesPerRow();
        
        for (int y = 0; y < m_height; ++y) {
            size_t lastByteIndex = static_cast<size_t>(y) * bytesPerRow + bytesPerRow - 1;
            uint8_t lastByte = m_data[lastByteIndex];
            
            // Count only the bits corresponding to actual pixels
            uint8_t maskedByte = lastByte & ((1 << extraBits) - 1);
            uint8_t unusedBitCount = std::popcount(lastByte) - std::popcount(maskedByte);
            count -= unusedBitCount;
        }
    }
    
    return count;
}

Result<std::unique_ptr<BinaryImage>> BinaryImage::fromGrayscale(
    const GrayscaleImage& image, 
    uint8_t threshold, 
    bool invert
) {
    try {
        auto result = std::make_unique<BinaryImage>(image.getWidth(), image.getHeight());
        
        for (int y = 0; y < image.getHeight(); ++y) {
            for (int x = 0; x < image.getWidth(); ++x) {
                auto pixelResult = image.getPixel(x, y);
                if (!pixelResult) {
                    return makeErrorResult<std::unique_ptr<BinaryImage>>(
                        pixelResult.error().code(),
                        pixelResult.error().message()
                    );
                }
                
                uint8_t pixelValue = pixelResult.value();
                bool isWhite = (pixelValue >= threshold);
                
                // Apply inversion if requested
                if (invert) {
                    isWhite = !isWhite;
                }
                
                auto setResult = result->setPixel(x, y, isWhite);
                if (!setResult) {
                    return makeErrorResult<std::unique_ptr<BinaryImage>>(
                        setResult.error().code(),
                        setResult.error().message()
                    );
                }
            }
        }
        
        return makeSuccessResult(std::move(result));
    } catch (const std::exception& e) {
        return makeErrorResult<std::unique_ptr<BinaryImage>>(
            ErrorCode::ProcessingFailed,
            std::format("Failed to create binary image from grayscale: {}", e.what())
        );
    }
}

int BinaryImage::getBitIndex(int x, int y) const {
    return (y * m_width) + x;
}

int BinaryImage::getByteIndex(int x, int y) const {
    int bytesPerRow = getBytesPerRow();
    return (y * bytesPerRow) + (x / 8);
}

uint8_t BinaryImage::getBitMask(int x) const {
    // Create a mask for the specific bit position (0-7) within the byte
    // The position is calculated from the least significant bit (rightmost)
    // For little-endian bit ordering: Bit 0 = 0x01, Bit 1 = 0x02, ..., Bit 7 = 0x80
    return static_cast<uint8_t>(1 << (x % 8));
}

} // namespace DIPAL
