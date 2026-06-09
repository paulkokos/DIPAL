// include/DIPAL/Filters/LowPassFilter.hpp
#ifndef DIPAL_LOW_PASS_FILTER_HPP
#define DIPAL_LOW_PASS_FILTER_HPP

#include "FilterStrategy.hpp"

namespace DIPAL {

/**
 * @brief Low-pass filter  (Gaussian blur)
 *
 * Attenuates high-frequency components by applying a Gaussian blur.
 * This is a named alias that makes intent explicit in processing pipelines.
 * Works on grayscale and colour images.
 */
class LowPassFilter : public FilterStrategy {
public:
    /**
     * @param sigma      Standard deviation of the Gaussian kernel
     * @param kernelSize Kernel size (0 = auto from sigma)
     */
    explicit LowPassFilter(float sigma = 1.0f, int kernelSize = 0);

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
#endif // DIPAL_LOW_PASS_FILTER_HPP
