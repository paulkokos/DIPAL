// include/DIPAL/Color/ColorConversions.hpp
#ifndef DIPAL_COLOR_CONVERSIONS_HPP
#define DIPAL_COLOR_CONVERSIONS_HPP

#include <cstdint>
#include "../Core/Types.hpp"
#include <array>

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

    // -------------------------------------------------------------------------
    // HSI
    // -------------------------------------------------------------------------
    [[nodiscard]] static HSI  rgbToHsi(const RGB& rgb);
    [[nodiscard]] static RGB  hsiToRgb(const HSI& hsi);

    // -------------------------------------------------------------------------
    // HLS
    // -------------------------------------------------------------------------
    [[nodiscard]] static HLS  rgbToHls(const RGB& rgb);
    [[nodiscard]] static RGB  hlsToRgb(const HLS& hls);

    // -------------------------------------------------------------------------
    // CIELAB  (D65 illuminant, sRGB primaries)
    // -------------------------------------------------------------------------
    [[nodiscard]] static Lab  rgbToLab(const RGB& rgb);
    [[nodiscard]] static RGB  labToRgb(const Lab& lab);

    // -------------------------------------------------------------------------
    // CMY
    // -------------------------------------------------------------------------
    [[nodiscard]] static CMY  rgbToCmy(const RGB& rgb);
    [[nodiscard]] static RGB  cmyToRgb(const CMY& cmy);

    // -------------------------------------------------------------------------
    // sRGB  (linear ↔ gamma-corrected)
    // -------------------------------------------------------------------------
    [[nodiscard]] static std::array<float,3> rgbToLinear(const RGB& rgb);
    [[nodiscard]] static RGB  linearToRgb(const std::array<float,3>& linear);

    // -------------------------------------------------------------------------
    // YCbCr  (BT.601 studio swing)
    // -------------------------------------------------------------------------
    [[nodiscard]] static YCbCr rgbToYCbCr(const RGB& rgb);
    [[nodiscard]] static RGB   yCbCrToRgb(const YCbCr& ycbcr);

    // -------------------------------------------------------------------------
    // YIQ  (NTSC)
    // -------------------------------------------------------------------------
    [[nodiscard]] static YIQ  rgbToYiq(const RGB& rgb);
    [[nodiscard]] static RGB  yiqToRgb(const YIQ& yiq);
};

} // namespace DIPAL

#endif // DIPAL_COLOR_CONVERSIONS_HPP
