// include/DIPAL/Filters/BilateralFilter.hpp
#ifndef DIPAL_BILATERAL_FILTER_HPP
#define DIPAL_BILATERAL_FILTER_HPP

#include "FilterStrategy.hpp"

namespace DIPAL {

/**
 * @brief Bilateral filter (edge-preserving smoothing)
 *
 * Combines a spatial Gaussian weight (distance in pixels) with a range
 * Gaussian weight (difference in intensity) so that edges are preserved
 * while flat regions are smoothed.
 *
 * Works on grayscale and colour images (per-channel range weight).
 */
class BilateralFilter : public FilterStrategy {
public:
    /**
     * @param diameter    Diameter of the pixel neighbourhood (must be odd; 0 = auto from sigmaSpace)
     * @param sigmaColor  Gaussian standard deviation in the intensity/colour domain
     * @param sigmaSpace  Gaussian standard deviation in the spatial domain (pixels)
     */
    BilateralFilter(int diameter = 0, float sigmaColor = 75.0f, float sigmaSpace = 75.0f);

    [[nodiscard]] Result<std::unique_ptr<Image>> apply(const Image& image) const override;
    [[nodiscard]] std::string_view getName() const override;
    [[nodiscard]] std::unique_ptr<FilterStrategy> clone() const override;

    [[nodiscard]] int   getDiameter()   const noexcept { return m_diameter; }
    [[nodiscard]] float getSigmaColor() const noexcept { return m_sigmaColor; }
    [[nodiscard]] float getSigmaSpace() const noexcept { return m_sigmaSpace; }

private:
    int   m_diameter;
    float m_sigmaColor;
    float m_sigmaSpace;
};

} // namespace DIPAL
#endif // DIPAL_BILATERAL_FILTER_HPP
