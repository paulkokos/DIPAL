// include/DIPAL/Color/ColorConversions.hpp
#ifndef DIPAL_COLOR_CONVERSIONS_HPP
#define DIPAL_COLOR_CONVERSIONS_HPP

#include <cstdint>
#include "../Core/Types.hpp"

namespace DIPAL {

/**
 * @brief Static class for color space conversions
 */
class ColorConversions {
public:
    /**
     * @brief Convert RGB to HSV
     * @param rgb RGB color
     * @return HSV color
     */
    [[nodiscard]] static HSV rgbToHsv(const RGB& rgb);

    /**
     * @brief Convert RGBA to HSV (alpha is ignored)
     * @param rgba RGBA color
     * @return HSV color
     */
    [[nodiscard]] static HSV rgbaToHsv(const RGBA& rgba);

    /**
     * @brief Convert HSV to RGB
     * @param hsv HSV color
     * @return RGB color
     */
    [[nodiscard]] static RGB hsvToRgb(const HSV& hsv);

    /**
     * @brief Convert HSV to RGBA
     * @param hsv HSV color
     * @param alpha Alpha value (0-255)
     * @return RGBA color
     */
    [[nodiscard]] static RGBA hsvToRgba(const HSV& hsv, uint8_t alpha = 255);

    /**
     * @brief Convert RGB to grayscale using luminance formula
     * @param rgb RGB color
     * @return Grayscale value (0-255)
     */
    [[nodiscard]] static uint8_t rgbToGrayscale(const RGB& rgb);

    /**
     * @brief Convert RGBA to grayscale using luminance formula
     * @param rgba RGBA color
     * @return Grayscale value (0-255)
     */
    [[nodiscard]] static uint8_t rgbaToGrayscale(const RGBA& rgba);

    /**
     * @brief Blend two RGBA colors
     * @param foreground Foreground color
     * @param background Background color
     * @return Blended color
     */
    [[nodiscard]] static RGBA blendColors(const RGBA& foreground, const RGBA& background);
};

} // namespace DIPAL

#endif // DIPAL_COLOR_CONVERSIONS_HPP
