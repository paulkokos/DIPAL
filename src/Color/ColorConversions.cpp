// src/Color/ColorConversions.cpp
#include "../../include/DIPAL/Color/ColorConversions.hpp"

#include <algorithm>
#include <cmath>

namespace DIPAL {

HSV ColorConversions::rgbToHsv(const RGB& rgb) {
    // Normalize RGB to [0, 1]
    float r = rgb.r / 255.0f;
    float g = rgb.g / 255.0f;
    float b = rgb.b / 255.0f;

    float cmax = std::max({r, g, b});
    float cmin = std::min({r, g, b});
    float delta = cmax - cmin;

    // Calculate HSV
    HSV hsv;

    // Calculate hue
    if (delta < 0.00001f) {
        hsv.h = 0.0f;  // Undefined, default to 0
    } else if (cmax == r) {
        hsv.h = 60.0f * std::fmod(((g - b) / delta), 6.0f);
    } else if (cmax == g) {
        hsv.h = 60.0f * (((b - r) / delta) + 2.0f);
    } else { // cmax == b
        hsv.h = 60.0f * (((r - g) / delta) + 4.0f);
    }

    // Ensure hue is in [0, 360)
    if (hsv.h < 0.0f) {
        hsv.h += 360.0f;
    }

    // Calculate saturation
    hsv.s = (cmax < 0.00001f) ? 0.0f : (delta / cmax);

    // Value is maximum component
    hsv.v = cmax;

    return hsv;
}

HSV ColorConversions::rgbaToHsv(const RGBA& rgba) {
    // Just use RGB part
    return rgbToHsv(RGB(rgba.r, rgba.g, rgba.b));
}

RGB ColorConversions::hsvToRgb(const HSV& hsv) {
    // Constrain values
    float h = std::fmod(hsv.h, 360.0f);
    if (h < 0.0f) h += 360.0f;
    float s = std::clamp(hsv.s, 0.0f, 1.0f);
    float v = std::clamp(hsv.v, 0.0f, 1.0f);

    // HSV to RGB conversion
    float c = v * s;
    float x = c * (1.0f - std::abs(std::fmod(h / 60.0f, 2.0f) - 1.0f));
    float m = v - c;

    float r = 0.0f, g = 0.0f, b = 0.0f;

    if (h >= 0.0f && h < 60.0f) {
        r = c; g = x; b = 0.0f;
    } else if (h >= 60.0f && h < 120.0f) {
        r = x; g = c; b = 0.0f;
    } else if (h >= 120.0f && h < 180.0f) {
        r = 0.0f; g = c; b = x;
    } else if (h >= 180.0f && h < 240.0f) {
        r = 0.0f; g = x; b = c;
    } else if (h >= 240.0f && h < 300.0f) {
        r = x; g = 0.0f; b = c;
    } else { // h >= 300.0f && h < 360.0f
        r = c; g = 0.0f; b = x;
    }

    // Scale to [0, 255] and return
    return RGB(
        static_cast<uint8_t>(std::round((r + m) * 255.0f)),
        static_cast<uint8_t>(std::round((g + m) * 255.0f)),
        static_cast<uint8_t>(std::round((b + m) * 255.0f))
    );
}

RGBA ColorConversions::hsvToRgba(const HSV& hsv, uint8_t alpha) {
    RGB rgb = hsvToRgb(hsv);
    return RGBA(rgb.r, rgb.g, rgb.b, alpha);
}

uint8_t ColorConversions::rgbToGrayscale(const RGB& rgb) {
    // Standard luminance formula (ITU-R BT.709)
    return static_cast<uint8_t>(std::round(
        0.2126f * rgb.r + 0.7152f * rgb.g + 0.0722f * rgb.b
    ));
}

uint8_t ColorConversions::rgbaToGrayscale(const RGBA& rgba) {
    // Just use RGB part
    return rgbToGrayscale(RGB(rgba.r, rgba.g, rgba.b));
}

RGBA ColorConversions::blendColors(const RGBA& foreground, const RGBA& background) {
    // Alpha blending formula
    float alpha = foreground.a / 255.0f;
    float invAlpha = 1.0f - alpha;

    RGBA result;
    result.r = static_cast<uint8_t>(std::round(foreground.r * alpha + background.r * invAlpha));
    result.g = static_cast<uint8_t>(std::round(foreground.g * alpha + background.g * invAlpha));
    result.b = static_cast<uint8_t>(std::round(foreground.b * alpha + background.b * invAlpha));
    
    // Combined alpha
    result.a = static_cast<uint8_t>(std::round(foreground.a + background.a * invAlpha));

    return result;
}

} // namespace DIPAL
