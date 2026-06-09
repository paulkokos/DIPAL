// include/DIPAL/Core/Types.hpp
#ifndef DIPAL_TYPES_HPP
#define DIPAL_TYPES_HPP
#include <algorithm>
#include <cmath>
#include <cstdint>
#include <array>

namespace DIPAL {

// Common color types

/**
 * @brief RGB color structure (3 channels)
 */
struct RGB {
    uint8_t r;
    uint8_t g;
    uint8_t b;
    
    // Constructors
    constexpr RGB() noexcept : r(0), g(0), b(0) {}
    constexpr RGB(uint8_t r, uint8_t g, uint8_t b) noexcept : r(r), g(g), b(b) {}
    
    // Comparison operators
    constexpr bool operator==(const RGB& other) const noexcept {
        return r == other.r && g == other.g && b == other.b;
    }
    
    constexpr bool operator!=(const RGB& other) const noexcept {
        return !(*this == other);
    }
};

/**
 * @brief RGBA color structure (4 channels)
 */
struct RGBA {
    uint8_t r;
    uint8_t g;
    uint8_t b;
    uint8_t a;
    
    // Constructors
    constexpr RGBA() noexcept : r(0), g(0), b(0), a(255) {}
    constexpr RGBA(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255) noexcept : r(r), g(g), b(b), a(a) {}
    
    // Convert from RGB
    constexpr RGBA(const RGB& rgb, uint8_t a = 255) noexcept : r(rgb.r), g(rgb.g), b(rgb.b), a(a) {}
    
    // Comparison operators
    constexpr bool operator==(const RGBA& other) const noexcept {
        return r == other.r && g == other.g && b == other.b && a == other.a;
    }
    
    constexpr bool operator!=(const RGBA& other) const noexcept {
        return !(*this == other);
    }
};

/**
 * @brief HSV color structure
 */
struct HSV {
    float h;  // Hue [0, 360)
    float s;  // Saturation [0, 1]
    float v;  // Value [0, 1]
    
    // Constructors
    constexpr HSV() noexcept : h(0.0f), s(0.0f), v(0.0f) {}
    constexpr HSV(float h, float s, float v) noexcept : 
        h(h < 0.0f ? 0.0f : (h >= 360.0f ? h - 360.0f * static_cast<int>(h / 360.0f) : h)),
        s(s < 0.0f ? 0.0f : (s > 1.0f ? 1.0f : s)),
        v(v < 0.0f ? 0.0f : (v > 1.0f ? 1.0f : v)) {}
    
    // Comparison operators
    bool operator==(const HSV& other) const noexcept {
        constexpr float epsilon = 0.0001f;
        return std::abs(h - other.h) < epsilon && 
               std::abs(s - other.s) < epsilon && 
               std::abs(v - other.v) < epsilon;
    }
    
    bool operator!=(const HSV& other) const noexcept {
        return !(*this == other);
    }
};

/**
 * @brief HSI color structure (Hue-Saturation-Intensity)
 */
struct HSI {
    float h;  // Hue [0, 360)
    float s;  // Saturation [0, 1]
    float i;  // Intensity [0, 1]

    constexpr HSI() noexcept : h(0.0f), s(0.0f), i(0.0f) {}
    constexpr HSI(float h, float s, float i) noexcept : h(h), s(s), i(i) {}

    bool operator==(const HSI& o) const noexcept {
        constexpr float eps = 0.0001f;
        return std::abs(h-o.h)<eps && std::abs(s-o.s)<eps && std::abs(i-o.i)<eps;
    }
    bool operator!=(const HSI& o) const noexcept { return !(*this == o); }
};

/**
 * @brief HLS color structure (Hue-Lightness-Saturation)
 */
struct HLS {
    float h;  // Hue [0, 360)
    float l;  // Lightness [0, 1]
    float s;  // Saturation [0, 1]

    constexpr HLS() noexcept : h(0.0f), l(0.0f), s(0.0f) {}
    constexpr HLS(float h, float l, float s) noexcept : h(h), l(l), s(s) {}

    bool operator==(const HLS& o) const noexcept {
        constexpr float eps = 0.0001f;
        return std::abs(h-o.h)<eps && std::abs(l-o.l)<eps && std::abs(s-o.s)<eps;
    }
    bool operator!=(const HLS& o) const noexcept { return !(*this == o); }
};

/**
 * @brief CIELAB color structure (perceptually uniform)
 * L* in [0, 100], a* and b* typically in [-128, 127]
 */
struct Lab {
    float L;
    float a;
    float b;

    constexpr Lab() noexcept : L(0.0f), a(0.0f), b(0.0f) {}
    constexpr Lab(float L, float a, float b) noexcept : L(L), a(a), b(b) {}

    bool operator==(const Lab& o) const noexcept {
        constexpr float eps = 0.001f;
        return std::abs(L-o.L)<eps && std::abs(a-o.a)<eps && std::abs(b-o.b)<eps;
    }
    bool operator!=(const Lab& o) const noexcept { return !(*this == o); }
};

/**
 * @brief CMY color structure (subtractive, printing)
 * Each channel in [0, 1]
 */
struct CMY {
    float c;  // Cyan
    float m;  // Magenta
    float y;  // Yellow

    constexpr CMY() noexcept : c(0.0f), m(0.0f), y(0.0f) {}
    constexpr CMY(float c, float m, float y) noexcept : c(c), m(m), y(y) {}

    bool operator==(const CMY& o) const noexcept {
        constexpr float eps = 0.0001f;
        return std::abs(c-o.c)<eps && std::abs(m-o.m)<eps && std::abs(y-o.y)<eps;
    }
    bool operator!=(const CMY& o) const noexcept { return !(*this == o); }
};

/**
 * @brief YCbCr color structure (luma + chroma, BT.601)
 * Y in [16, 235], Cb/Cr in [16, 240] (studio swing)
 */
struct YCbCr {
    uint8_t y;
    uint8_t cb;
    uint8_t cr;

    constexpr YCbCr() noexcept : y(16), cb(128), cr(128) {}
    constexpr YCbCr(uint8_t y, uint8_t cb, uint8_t cr) noexcept : y(y), cb(cb), cr(cr) {}

    bool operator==(const YCbCr& o) const noexcept {
        return y == o.y && cb == o.cb && cr == o.cr;
    }
    bool operator!=(const YCbCr& o) const noexcept { return !(*this == o); }
};

/**
 * @brief YIQ color structure (NTSC broadcast)
 * Y in [0, 1], I in [-0.596, 0.596], Q in [-0.523, 0.523]
 */
struct YIQ {
    float y;
    float i;
    float q;

    constexpr YIQ() noexcept : y(0.0f), i(0.0f), q(0.0f) {}
    constexpr YIQ(float y, float i, float q) noexcept : y(y), i(i), q(q) {}

    bool operator==(const YIQ& o) const noexcept {
        constexpr float eps = 0.0001f;
        return std::abs(y-o.y)<eps && std::abs(i-o.i)<eps && std::abs(q-o.q)<eps;
    }
    bool operator!=(const YIQ& o) const noexcept { return !(*this == o); }
};

/**
 * @brief 2D size structure
 */
struct Size {
    int width;
    int height;
    
    // Constructors
    constexpr Size() noexcept : width(0), height(0) {}
    constexpr Size(int w, int h) noexcept : width(w), height(h) {}
    
    // Comparison operators
    constexpr bool operator==(const Size& other) const noexcept {
        return width == other.width && height == other.height;
    }
    
    constexpr bool operator!=(const Size& other) const noexcept {
        return !(*this == other);
    }
    
    // Area calculation
    [[nodiscard]] constexpr int area() const noexcept {
        return width * height;
    }
    
    // Empty check
    [[nodiscard]] constexpr bool isEmpty() const noexcept {
        return width <= 0 || height <= 0;
    }
};

/**
 * @brief 2D point structure
 */
struct Point {
    int x;
    int y;
    
    // Constructors
    constexpr Point() noexcept : x(0), y(0) {}
    constexpr Point(int x, int y) noexcept : x(x), y(y) {}
    
    // Comparison operators
    constexpr bool operator==(const Point& other) const noexcept {
        return x == other.x && y == other.y;
    }
    
    constexpr bool operator!=(const Point& other) const noexcept {
        return !(*this == other);
    }
    
    // Vector addition
    constexpr Point operator+(const Point& other) const noexcept {
        return Point(x + other.x, y + other.y);
    }
    
    // Vector subtraction
    constexpr Point operator-(const Point& other) const noexcept {
        return Point(x - other.x, y - other.y);
    }
};

/**
 * @brief 2D rectangle structure
 */
struct Rect {
    int x;
    int y;
    int width;
    int height;
    
    // Constructors
    constexpr Rect() noexcept : x(0), y(0), width(0), height(0) {}
    constexpr Rect(int x, int y, int w, int h) noexcept : x(x), y(y), width(w), height(h) {}
    constexpr Rect(const Point& tl, const Size& size) noexcept : 
        x(tl.x), y(tl.y), width(size.width), height(size.height) {}
    constexpr Rect(const Point& tl, const Point& br) noexcept :
        x(tl.x), y(tl.y), width(br.x - tl.x), height(br.y - tl.y) {}
    
    // Comparison operators
    constexpr bool operator==(const Rect& other) const noexcept {
        return x == other.x && y == other.y && 
               width == other.width && height == other.height;
    }
    
    constexpr bool operator!=(const Rect& other) const noexcept {
        return !(*this == other);
    }
    
    // Derived properties
    [[nodiscard]] constexpr Point topLeft() const noexcept {
        return Point(x, y);
    }
    
    [[nodiscard]] constexpr Point topRight() const noexcept {
        return Point(x + width, y);
    }
    
    [[nodiscard]] constexpr Point bottomLeft() const noexcept {
        return Point(x, y + height);
    }
    
    [[nodiscard]] constexpr Point bottomRight() const noexcept {
        return Point(x + width, y + height);
    }
    
    [[nodiscard]] constexpr Size size() const noexcept {
        return Size(width, height);
    }
    
    [[nodiscard]] constexpr int area() const noexcept {
        return width * height;
    }
    
    [[nodiscard]] constexpr bool isEmpty() const noexcept {
        return width <= 0 || height <= 0;
    }
    
    [[nodiscard]] constexpr bool contains(const Point& pt) const noexcept {
        return pt.x >= x && pt.x < x + width &&
               pt.y >= y && pt.y < y + height;
    }
    
    [[nodiscard]] constexpr bool contains(const Rect& rect) const noexcept {
        return rect.x >= x && rect.x + rect.width <= x + width &&
               rect.y >= y && rect.y + rect.height <= y + height;
    }
    
    [[nodiscard]] constexpr bool intersects(const Rect& rect) const noexcept {
        return !(rect.x >= x + width || rect.x + rect.width <= x ||
                 rect.y >= y + height || rect.y + rect.height <= y);
    }
    
    // Calculate intersection rectangle
    [[nodiscard]] constexpr Rect intersection(const Rect& rect) const noexcept {
        int nx = std::max(x, rect.x);
        int ny = std::max(y, rect.y);
        int nw = std::min(x + width, rect.x + rect.width) - nx;
        int nh = std::min(y + height, rect.y + rect.height) - ny;
        
        if (nw <= 0 || nh <= 0) {
            return Rect();  // Empty rect
        }
        
        return Rect(nx, ny, nw, nh);
    }
};

} // namespace DIPAL

#endif // DIPAL_TYPES_HPP
