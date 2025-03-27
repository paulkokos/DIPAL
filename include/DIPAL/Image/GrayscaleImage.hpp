// include/DIPAL/Image/GrayscaleImage.hpp
#ifndef DIPAL_GRAYSCALE_IMAGE_HPP
#define DIPAL_GRAYSCALE_IMAGE_HPP

#include "Image.hpp"
#include "../Core/Error.hpp"

namespace DIPAL {

/**
 * @brief Specialized class for grayscale images
 */
class GrayscaleImage : public Image {
public:
    /**
     * @brief Create a new empty grayscale image
     * @param width Image width in pixels
     * @param height Image height in pixels
     */
    GrayscaleImage(int width, int height);

    /**
     * @brief Get pixel value at specific coordinates
     * @param x X coordinate
     * @param y Y coordinate
     * @return Result containing pixel value or error
     */
    [[nodiscard]] Result<uint8_t> getPixel(int x, int y) const;

    /**
     * @brief Set pixel value at specific coordinates
     * @param x X coordinate
     * @param y Y coordinate
     * @param value Pixel value (0-255)
     * @return VoidResult indicating success or error
     */
    [[maybe_unused]] VoidResult setPixel(int x, int y, uint8_t value);

    /**
     * @brief Clone the grayscale image
     * @return A new grayscale image that is a deep copy of this image
     */
    [[nodiscard]] std::unique_ptr<Image> clone() const override;

    /**
     * @brief Get a span of a specific row
     * @param y Row index
     * @return Span containing the row data or empty span if invalid
     */
    [[nodiscard]] std::span<const uint8_t> getRow(int y) const;
    
    /**
     * @brief Get a modifiable span of a specific row
     * @param y Row index
     * @return Span containing the row data or empty span if invalid
     */
    [[nodiscard]] std::span<uint8_t> getRow(int y);
};

} // namespace DIPAL

#endif // DIPAL_GRAYSCALE_IMAGE_HPP
