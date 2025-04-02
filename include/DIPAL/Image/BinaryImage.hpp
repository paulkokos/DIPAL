// include/DIPAL/Image/BinaryImage.hpp
#ifndef DIPAL_BINARY_IMAGE_HPP
#define DIPAL_BINARY_IMAGE_HPP

#include "Image.hpp"
#include "../Core/Error.hpp"

namespace DIPAL {

class GrayscaleImage;  // Forward declaration

/**
 * @brief Specialized class for binary (black and white) images
 * 
 * Binary images store each pixel as a single bit (0 for black, 1 for white).
 * They are stored efficiently with 8 pixels per byte.
 */
class BinaryImage : public Image {
public:
    /**
     * @brief Create a new empty binary image
     * @param width Image width in pixels
     * @param height Image height in pixels
     */
    BinaryImage(int width, int height);

    /**
     * @brief Get pixel value at specific coordinates
     * @param x X coordinate
     * @param y Y coordinate
     * @return Result containing pixel value (true for white, false for black) or error
     */
    [[nodiscard]] Result<bool> getPixel(int x, int y) const;

    /**
     * @brief Set pixel value at specific coordinates
     * @param x X coordinate
     * @param y Y coordinate
     * @param value Pixel value (true for white, false for black)
     * @return VoidResult indicating success or error
     */
    [[nodiscard]] VoidResult setPixel(int x, int y, bool value);

    /**
     * @brief Clone the binary image
     * @return A new binary image that is a deep copy of this image
     */
    [[nodiscard]] std::unique_ptr<Image> clone() const override;

    /**
     * @brief Get a span of a specific row in bit-packed format
     * @param y Row index
     * @return Span containing the row data (8 pixels per byte) or empty span if invalid
     */
    [[nodiscard]] std::span<const uint8_t> getRow(int y) const;
    
    /**
     * @brief Get a modifiable span of a specific row in bit-packed format
     * @param y Row index
     * @return Span containing the row data (8 pixels per byte) or empty span if invalid
     */
    [[nodiscard]] std::span<uint8_t> getRow(int y);

    /**
     * @brief Get the number of bytes per row
     * @return Bytes per row
     */
    [[nodiscard]] int getBytesPerRow() const;

    /**
     * @brief Invert the image (flip black and white)
     * @return VoidResult indicating success or error
     */
    [[nodiscard]] VoidResult invert();

    /**
     * @brief Set all pixels to the same value
     * @param value Pixel value (true for white, false for black)
     * @return VoidResult indicating success or error
     */
    [[nodiscard]] VoidResult fill(bool value);

    /**
     * @brief Count the number of white pixels (bits set to 1)
     * @return Number of white pixels
     */
    [[nodiscard]] size_t countWhitePixels() const;

    /**
     * @brief Apply a binary threshold to a grayscale image
     * @param image Source grayscale image
     * @param threshold Threshold value (0-255)
     * @param invert If true, pixels above threshold become black (false) instead of white (true)
     * @return Result containing a binary image or error
     */
    [[nodiscard]] static Result<std::unique_ptr<BinaryImage>> fromGrayscale(
        const GrayscaleImage& image, 
        uint8_t threshold = 128, 
        bool invert = false
    );

private:
    // Helper methods to work with bit-packed data
    [[nodiscard]] int getBitIndex(int x, int y) const;
    [[nodiscard]] int getByteIndex(int x, int y) const;
    [[nodiscard]] uint8_t getBitMask(int x) const;
};

} // namespace DIPAL

#endif // DIPAL_BINARY_IMAGE_HPP
