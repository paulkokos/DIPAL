// include/DIPAL/Image/GrayscaleImage.hpp
#ifndef DIPAL_GRAYSCALE_IMAGE_HPP
#define DIPAL_GRAYSCALE_IMAGE_HPP

#include "Image.hpp"

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
     * @return Pixel value (0-255)
     */
    uint8_t getPixel(int x, int y) const;

    /**
     * @brief Set pixel value at specific coordinates
     * @param x X coordinate
     * @param y Y coordinate
     * @param value Pixel value (0-255)
     */
    void setPixel(int x, int y, uint8_t value);

    /**
     * @brief Clone the grayscale image
     * @return A new grayscale image that is a deep copy of this image
     */
    std::unique_ptr<Image> clone() const override;
};

}  // namespace DIPAL

#endif  // DIPAL_GRAYSCALE_IMAGE_HPP
