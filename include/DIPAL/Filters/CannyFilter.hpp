// include/DIPAL/Filters/CannyFilter.hpp
#ifndef DIPAL_CANNY_FILTER_HPP
#define DIPAL_CANNY_FILTER_HPP

#include "FilterStrategy.hpp"

namespace DIPAL {

/**
 * @brief Canny edge detector
 *
 * Full four-stage pipeline:
 *   1. Gaussian smoothing
 *   2. Sobel gradient magnitude and direction
 *   3. Non-maximum suppression (thin edges to 1 pixel)
 *   4. Double-threshold hysteresis (strong / weak / discard)
 *
 * Output is a binary-style grayscale image: 255 = edge, 0 = background.
 * Color images are converted to grayscale before processing.
 */
class CannyFilter : public FilterStrategy {
public:
    /**
     * @param lowThreshold   Lower hysteresis threshold  (0–255)
     * @param highThreshold  Upper hysteresis threshold  (0–255)
     * @param sigma          Gaussian pre-blur sigma (default 1.0)
     * @param kernelSize     Gaussian kernel size; 0 = auto from sigma
     */
    CannyFilter(float lowThreshold  = 50.0f,
                float highThreshold = 150.0f,
                float sigma         = 1.0f,
                int   kernelSize    = 0);

    [[nodiscard]] Result<std::unique_ptr<Image>> apply(const Image& image) const override;
    [[nodiscard]] std::string_view getName() const override;
    [[nodiscard]] std::unique_ptr<FilterStrategy> clone() const override;

    [[nodiscard]] float getLowThreshold()  const noexcept { return m_lowThreshold; }
    [[nodiscard]] float getHighThreshold() const noexcept { return m_highThreshold; }
    [[nodiscard]] float getSigma()         const noexcept { return m_sigma; }
    [[nodiscard]] int   getKernelSize()    const noexcept { return m_kernelSize; }

private:
    float m_lowThreshold;
    float m_highThreshold;
    float m_sigma;
    int   m_kernelSize;
};

} // namespace DIPAL

#endif // DIPAL_CANNY_FILTER_HPP
