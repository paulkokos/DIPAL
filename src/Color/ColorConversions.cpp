// src/Color/ColorConversions.cpp
#include "../../include/DIPAL/Color/ColorConversions.hpp"

#include <algorithm>
#include <array>
#include <cmath>
#include <numbers>

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
    constexpr float epsilon = 0.00001f;
    if (delta < epsilon) {
        hsv.h = 0.0f;  // Undefined, default to 0
    } else if (std::abs(cmax - r) < epsilon) {
        hsv.h = 60.0f * std::fmod(((g - b) / delta), 6.0f);
    } else if (std::abs(cmax - g) < epsilon) {
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

// =============================================================================
// HSI
// =============================================================================

HSI ColorConversions::rgbToHsi(const RGB& rgb) {
    const float r = rgb.r / 255.0f;
    const float g = rgb.g / 255.0f;
    const float b = rgb.b / 255.0f;

    const float intensity = (r + g + b) / 3.0f;

    constexpr float eps = 1e-6f;
    if (intensity < eps) return HSI(0.0f, 0.0f, 0.0f);

    const float minVal = std::min({r, g, b});
    const float saturation = 1.0f - (3.0f * minVal / (r + g + b));

    float hue = 0.0f;
    if (saturation > eps) {
        const float num = 0.5f * ((r - g) + (r - b));
        const float den = std::sqrt((r-g)*(r-g) + (r-b)*(g-b));
        const float theta = (den < eps) ? 0.0f : std::acos(std::clamp(num / den, -1.0f, 1.0f));
        hue = (b <= g) ? theta : (2.0f * std::numbers::pi_v<float> - theta);
        hue *= (180.0f / std::numbers::pi_v<float>);
    }

    return HSI(hue, saturation, intensity);
}

RGB ColorConversions::hsiToRgb(const HSI& hsi) {
    const float h = hsi.h;
    const float s = std::clamp(hsi.s, 0.0f, 1.0f);
    const float i = std::clamp(hsi.i, 0.0f, 1.0f);

    float r, g, b;
    const float hRad = h * (std::numbers::pi_v<float> / 180.0f);

    if (h < 120.0f) {
        b = i * (1.0f - s);
        r = i * (1.0f + (s * std::cos(hRad)) / std::cos(std::numbers::pi_v<float>/3.0f - hRad));
        g = 3.0f * i - (r + b);
    } else if (h < 240.0f) {
        const float h2 = hRad - 2.0f * std::numbers::pi_v<float> / 3.0f;
        r = i * (1.0f - s);
        g = i * (1.0f + (s * std::cos(h2)) / std::cos(std::numbers::pi_v<float>/3.0f - h2));
        b = 3.0f * i - (r + g);
    } else {
        const float h3 = hRad - 4.0f * std::numbers::pi_v<float> / 3.0f;
        g = i * (1.0f - s);
        b = i * (1.0f + (s * std::cos(h3)) / std::cos(std::numbers::pi_v<float>/3.0f - h3));
        r = 3.0f * i - (g + b);
    }

    return RGB(
        static_cast<uint8_t>(std::clamp(std::round(r * 255.0f), 0.0f, 255.0f)),
        static_cast<uint8_t>(std::clamp(std::round(g * 255.0f), 0.0f, 255.0f)),
        static_cast<uint8_t>(std::clamp(std::round(b * 255.0f), 0.0f, 255.0f))
    );
}

// =============================================================================
// HLS
// =============================================================================

HLS ColorConversions::rgbToHls(const RGB& rgb) {
    const float r = rgb.r / 255.0f;
    const float g = rgb.g / 255.0f;
    const float b = rgb.b / 255.0f;

    const float cmax = std::max({r, g, b});
    const float cmin = std::min({r, g, b});
    const float delta = cmax - cmin;
    const float l = (cmax + cmin) / 2.0f;

    constexpr float eps = 1e-6f;
    if (delta < eps) return HLS(0.0f, l, 0.0f);

    const float s = delta / (1.0f - std::abs(2.0f * l - 1.0f));

    float h;
    if (std::abs(cmax - r) < eps)
        h = 60.0f * std::fmod((g - b) / delta, 6.0f);
    else if (std::abs(cmax - g) < eps)
        h = 60.0f * ((b - r) / delta + 2.0f);
    else
        h = 60.0f * ((r - g) / delta + 4.0f);
    if (h < 0.0f) h += 360.0f;

    return HLS(h, l, s);
}

RGB ColorConversions::hlsToRgb(const HLS& hls) {
    const float h = hls.h;
    const float l = std::clamp(hls.l, 0.0f, 1.0f);
    const float s = std::clamp(hls.s, 0.0f, 1.0f);

    const float c = (1.0f - std::abs(2.0f * l - 1.0f)) * s;
    const float x = c * (1.0f - std::abs(std::fmod(h / 60.0f, 2.0f) - 1.0f));
    const float m = l - c / 2.0f;

    float r = 0.0f, g = 0.0f, b = 0.0f;
    if      (h <  60.0f) { r = c; g = x; }
    else if (h < 120.0f) { r = x; g = c; }
    else if (h < 180.0f) { g = c; b = x; }
    else if (h < 240.0f) { g = x; b = c; }
    else if (h < 300.0f) { r = x; b = c; }
    else                 { r = c; b = x; }

    return RGB(
        static_cast<uint8_t>(std::clamp(std::round((r + m) * 255.0f), 0.0f, 255.0f)),
        static_cast<uint8_t>(std::clamp(std::round((g + m) * 255.0f), 0.0f, 255.0f)),
        static_cast<uint8_t>(std::clamp(std::round((b + m) * 255.0f), 0.0f, 255.0f))
    );
}

// =============================================================================
// CIELAB  (D65 illuminant)
// =============================================================================

// sRGB gamma expand
static float srgbToLinearChannel(float c) {
    return (c <= 0.04045f) ? c / 12.92f : std::pow((c + 0.055f) / 1.055f, 2.4f);
}
// CIE f function
static float labF(float t) {
    constexpr float delta = 6.0f / 29.0f;
    return (t > delta * delta * delta)
        ? std::cbrt(t)
        : t / (3.0f * delta * delta) + 4.0f / 29.0f;
}
// Inverse CIE f
static float labFInv(float t) {
    constexpr float delta = 6.0f / 29.0f;
    return (t > delta) ? t * t * t : 3.0f * delta * delta * (t - 4.0f / 29.0f);
}

Lab ColorConversions::rgbToLab(const RGB& rgb) {
    // sRGB → linear
    const float rl = srgbToLinearChannel(rgb.r / 255.0f);
    const float gl = srgbToLinearChannel(rgb.g / 255.0f);
    const float bl = srgbToLinearChannel(rgb.b / 255.0f);

    // Linear RGB → XYZ (D65)
    const float x = 0.4124564f * rl + 0.3575761f * gl + 0.1804375f * bl;
    const float y = 0.2126729f * rl + 0.7151522f * gl + 0.0721750f * bl;
    const float z = 0.0193339f * rl + 0.1191920f * gl + 0.9503041f * bl;

    // D65 white point
    constexpr float xn = 0.95047f, yn = 1.00000f, zn = 1.08883f;

    const float fx = labF(x / xn);
    const float fy = labF(y / yn);
    const float fz = labF(z / zn);

    return Lab(116.0f * fy - 16.0f,
               500.0f * (fx - fy),
               200.0f * (fy - fz));
}

RGB ColorConversions::labToRgb(const Lab& lab) {
    const float fy = (lab.L + 16.0f) / 116.0f;
    const float fx = lab.a / 500.0f + fy;
    const float fz = fy - lab.b / 200.0f;

    constexpr float xn = 0.95047f, yn = 1.00000f, zn = 1.08883f;
    const float x = xn * labFInv(fx);
    const float y = yn * labFInv(fy);
    const float z = zn * labFInv(fz);

    // XYZ → linear RGB (D65)
    float r =  3.2404542f * x - 1.5371385f * y - 0.4985314f * z;
    float g = -0.9692660f * x + 1.8760108f * y + 0.0415560f * z;
    float b =  0.0556434f * x - 0.2040259f * y + 1.0572252f * z;

    // Gamma compress (sRGB)
    auto compress = [](float c) -> float {
        c = std::clamp(c, 0.0f, 1.0f);
        return (c <= 0.0031308f) ? 12.92f * c : 1.055f * std::pow(c, 1.0f/2.4f) - 0.055f;
    };

    return RGB(
        static_cast<uint8_t>(std::clamp(std::round(compress(r) * 255.0f), 0.0f, 255.0f)),
        static_cast<uint8_t>(std::clamp(std::round(compress(g) * 255.0f), 0.0f, 255.0f)),
        static_cast<uint8_t>(std::clamp(std::round(compress(b) * 255.0f), 0.0f, 255.0f))
    );
}

// =============================================================================
// CMY
// =============================================================================

CMY ColorConversions::rgbToCmy(const RGB& rgb) {
    return CMY(1.0f - rgb.r / 255.0f,
               1.0f - rgb.g / 255.0f,
               1.0f - rgb.b / 255.0f);
}

RGB ColorConversions::cmyToRgb(const CMY& cmy) {
    return RGB(
        static_cast<uint8_t>(std::clamp(std::round((1.0f - cmy.c) * 255.0f), 0.0f, 255.0f)),
        static_cast<uint8_t>(std::clamp(std::round((1.0f - cmy.m) * 255.0f), 0.0f, 255.0f)),
        static_cast<uint8_t>(std::clamp(std::round((1.0f - cmy.y) * 255.0f), 0.0f, 255.0f))
    );
}

// =============================================================================
// sRGB  (gamma correction)
// =============================================================================

std::array<float,3> ColorConversions::rgbToLinear(const RGB& rgb) {
    return {
        srgbToLinearChannel(rgb.r / 255.0f),
        srgbToLinearChannel(rgb.g / 255.0f),
        srgbToLinearChannel(rgb.b / 255.0f)
    };
}

RGB ColorConversions::linearToRgb(const std::array<float,3>& linear) {
    auto compress = [](float c) -> float {
        c = std::clamp(c, 0.0f, 1.0f);
        return (c <= 0.0031308f) ? 12.92f * c : 1.055f * std::pow(c, 1.0f/2.4f) - 0.055f;
    };
    return RGB(
        static_cast<uint8_t>(std::clamp(std::round(compress(linear[0]) * 255.0f), 0.0f, 255.0f)),
        static_cast<uint8_t>(std::clamp(std::round(compress(linear[1]) * 255.0f), 0.0f, 255.0f)),
        static_cast<uint8_t>(std::clamp(std::round(compress(linear[2]) * 255.0f), 0.0f, 255.0f))
    );
}

// =============================================================================
// YCbCr  (BT.601 studio swing)
// =============================================================================

YCbCr ColorConversions::rgbToYCbCr(const RGB& rgb) {
    const float r = rgb.r, g = rgb.g, b = rgb.b;
    const float y  =  16.0f + 0.257f*r + 0.504f*g + 0.098f*b;
    const float cb = 128.0f - 0.148f*r - 0.291f*g + 0.439f*b;
    const float cr = 128.0f + 0.439f*r - 0.368f*g - 0.071f*b;
    return YCbCr(
        static_cast<uint8_t>(std::clamp(std::round(y),  0.0f, 255.0f)),
        static_cast<uint8_t>(std::clamp(std::round(cb), 0.0f, 255.0f)),
        static_cast<uint8_t>(std::clamp(std::round(cr), 0.0f, 255.0f))
    );
}

RGB ColorConversions::yCbCrToRgb(const YCbCr& ycbcr) {
    const float y  = ycbcr.y  - 16.0f;
    const float cb = ycbcr.cb - 128.0f;
    const float cr = ycbcr.cr - 128.0f;
    const float r = 1.164f*y              + 1.596f*cr;
    const float g = 1.164f*y - 0.392f*cb - 0.813f*cr;
    const float b = 1.164f*y + 2.017f*cb;
    return RGB(
        static_cast<uint8_t>(std::clamp(std::round(r), 0.0f, 255.0f)),
        static_cast<uint8_t>(std::clamp(std::round(g), 0.0f, 255.0f)),
        static_cast<uint8_t>(std::clamp(std::round(b), 0.0f, 255.0f))
    );
}

// =============================================================================
// YIQ  (NTSC)
// =============================================================================

YIQ ColorConversions::rgbToYiq(const RGB& rgb) {
    const float r = rgb.r / 255.0f;
    const float g = rgb.g / 255.0f;
    const float b = rgb.b / 255.0f;
    return YIQ(
        0.299f*r  + 0.587f*g  + 0.114f*b,
        0.5959f*r - 0.2746f*g - 0.3213f*b,
        0.2115f*r - 0.5227f*g + 0.3112f*b
    );
}

RGB ColorConversions::yiqToRgb(const YIQ& yiq) {
    const float r = yiq.y + 0.9563f*yiq.i + 0.6210f*yiq.q;
    const float g = yiq.y - 0.2721f*yiq.i - 0.6474f*yiq.q;
    const float b = yiq.y - 1.1070f*yiq.i + 1.7046f*yiq.q;
    return RGB(
        static_cast<uint8_t>(std::clamp(std::round(r * 255.0f), 0.0f, 255.0f)),
        static_cast<uint8_t>(std::clamp(std::round(g * 255.0f), 0.0f, 255.0f)),
        static_cast<uint8_t>(std::clamp(std::round(b * 255.0f), 0.0f, 255.0f))
    );
}

} // namespace DIPAL
