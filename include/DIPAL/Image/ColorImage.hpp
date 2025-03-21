// include/DIPAL/Image/ColorImage.hpp
#ifndef DIPAL_COLOR_IMAGE_HPP
#define DIPAL_COLOR_IMAGE_HPP

#include "Image.hpp"

namespace DIPAL {

/**
 * @brief Specialized class for color images (RGB or RGBA)
 */
class ColorImage : public Image {
public:
    /**
     * @brief Create a new empty RGB color image
     * @param width Image width in pixels
     * @param height Image height in pixels
     * @param hasAlpha Whether the image has an alpha channel
     */
    ColorImage(int width, int height, bool hasAlpha = false);

    /**
     * @brief Get RGB pixel value at specific coordinates
     * @param x X coordinate
     * @param y Y coordinate
     * @param r Reference to store red value (0-255)
     * @param g Reference to store green value (0-255)
     * @param b Reference to store blue value (0-255)
     * @param a Reference to store alpha value (0-255), if applicable
     */
    void getPixel(int x, int y, uint8_t& r, uint8_t& g, uint8_t& b, uint8_t& a) const;

    /**
     * @brief Set RGB pixel value at specific coordinates
     * @param x X coordinate
     * @param y Y coordinate
     * @param r Red value (0-255)
     * @param g Green value (0-255)
     * @param b Blue value (0-255)
     * @param a Alpha value (0-255), ignored if image doesn't have alpha
     */
    void setPixel(int x, int y, uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255);

    /**
     * @brief Check if the image has an alpha channel
     * @return true if the image has an alpha channel, false otherwise
     */
    bool hasAlpha() const;

    /**
     * @brief Clone the color image
     * @return A new color image that is a deep copy of this image
     */
    std::unique_ptr<Image> clone() const override;
};

}  // namespace DIPAL

#endif  // DIPAL_COLOR_IMAGE_HPP
