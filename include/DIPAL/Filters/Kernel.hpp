// include/DIPAL/Filters/Kernel.hpp
#ifndef DIPAL_KERNEL_HPP
#define DIPAL_KERNEL_HPP

#include <vector>
#include <span>
#include <stdexcept>
#include <format>
#include <cmath>
#include <numbers>

namespace DIPAL {

/**
 * @brief Border handling mode for convolution
 */
enum class BorderMode {
    Clamp,    // Clamp coordinates to image boundary (replicate border pixels)
    Reflect,  // Reflect kernel indices at the boundary
    Wrap,     // Wrap around (tile)
    Zero,     // Treat out-of-bounds pixels as zero
};

/**
 * @brief A 2D convolution kernel
 *
 * Stores floating-point weights in row-major order.
 * Width and height must both be odd and positive.
 */
class Kernel {
public:
    /**
     * @brief Construct a kernel from a flat weight vector
     * @param width  Kernel width (must be odd and > 0)
     * @param height Kernel height (must be odd and > 0)
     * @param weights Row-major weights (size must equal width * height)
     */
    Kernel(int width, int height, std::vector<float> weights);

    [[nodiscard]] int getWidth()  const noexcept { return m_width; }
    [[nodiscard]] int getHeight() const noexcept { return m_height; }
    [[nodiscard]] int getHalfWidth()  const noexcept { return m_width  / 2; }
    [[nodiscard]] int getHalfHeight() const noexcept { return m_height / 2; }

    [[nodiscard]] float at(int row, int col) const noexcept {
        return m_weights[static_cast<size_t>(row) * m_width + col];
    }

    [[nodiscard]] std::span<const float> weights() const noexcept { return m_weights; }

    // -------------------------------------------------------------------------
    // Factory methods for common kernels
    // -------------------------------------------------------------------------

    /** 3x3 box (mean) blur */
    [[nodiscard]] static Kernel box(int size = 3);

    /** Gaussian blur kernel */
    [[nodiscard]] static Kernel gaussian(float sigma, int size = 0);

    /** 3x3 sharpening kernel */
    [[nodiscard]] static Kernel sharpen();

    /** 3x3 Laplacian (4-neighbour) */
    [[nodiscard]] static Kernel laplacian4();

    /** 3x3 Laplacian (8-neighbour) */
    [[nodiscard]] static Kernel laplacian8();

    /** 3x3 Prewitt X gradient */
    [[nodiscard]] static Kernel prewittX();

    /** 3x3 Prewitt Y gradient */
    [[nodiscard]] static Kernel prewittY();

    /** 2x2 Roberts Cross X */
    [[nodiscard]] static Kernel robertsCrossX();

    /** 2x2 Roberts Cross Y */
    [[nodiscard]] static Kernel robertsCrossY();

    /** 3x3 emboss */
    [[nodiscard]] static Kernel emboss();

    /** 3x3 edge-enhance */
    [[nodiscard]] static Kernel edgeEnhance();

private:
    int m_width;
    int m_height;
    std::vector<float> m_weights;
};

} // namespace DIPAL

#endif // DIPAL_KERNEL_HPP
