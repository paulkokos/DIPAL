// include/DIPAL/Filters/HighPassFilter.hpp
#ifndef DIPAL_HIGH_PASS_FILTER_HPP
#define DIPAL_HIGH_PASS_FILTER_HPP

#include "FilterStrategy.hpp"

namespace DIPAL {

/**
 * @brief High-pass filter  (original − Gaussian blur + 128 bias)
 *
 * Subtracts a Gaussian-blurred version from the original to retain only
 * high-frequency detail. A 128-bias centres the result around mid-grey.
 * Works on grayscale and colour images.
 */
class HighPassFilter : public FilterStrategy {
public:
    /**
     * @param sigma      Standard deviation of the subtracted Gaussian blur
     * @param kernelSize Gaussian kernel size (0 = auto from sigma)
     */
    explicit HighPassFilter(float sigma = 1.0f, int kernelSize = 0);

    [[nodiscard]] Result<std::unique_ptr<Image>> apply(const Image& image) const override;
    [[nodiscard]] std::string_view getName() const override;
    [[nodiscard]] std::unique_ptr<FilterStrategy> clone() const override;

    [[nodiscard]] float getSigma()      const noexcept { return m_sigma; }
    [[nodiscard]] int   getKernelSize() const noexcept { return m_kernelSize; }

private:
    float m_sigma;
    int   m_kernelSize;
};

} // namespace DIPAL
#endif // DIPAL_HIGH_PASS_FILTER_HPP
