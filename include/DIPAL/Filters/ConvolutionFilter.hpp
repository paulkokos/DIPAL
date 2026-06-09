// include/DIPAL/Filters/ConvolutionFilter.hpp
#ifndef DIPAL_CONVOLUTION_FILTER_HPP
#define DIPAL_CONVOLUTION_FILTER_HPP

#include "FilterStrategy.hpp"
#include "Kernel.hpp"

#include <string>

namespace DIPAL {

/**
 * @brief Generic 2D convolution filter
 *
 * Applies an arbitrary Kernel to grayscale or color images.
 * Color images are processed per-channel.
 */
class ConvolutionFilter : public FilterStrategy {
public:
    /**
     * @brief Construct a convolution filter
     * @param kernel     The kernel to apply
     * @param borderMode How to handle pixels near the image boundary
     * @param name       Optional name (defaults to "ConvolutionFilter")
     */
    explicit ConvolutionFilter(Kernel kernel,
                               BorderMode borderMode = BorderMode::Clamp,
                               std::string name = "ConvolutionFilter");

    [[nodiscard]] Result<std::unique_ptr<Image>> apply(const Image& image) const override;
    [[nodiscard]] std::string_view getName() const override;
    [[nodiscard]] std::unique_ptr<FilterStrategy> clone() const override;

    [[nodiscard]] const Kernel&    getKernel()     const noexcept { return m_kernel; }
    [[nodiscard]] BorderMode       getBorderMode() const noexcept { return m_borderMode; }

private:
    Kernel     m_kernel;
    BorderMode m_borderMode;
    std::string m_name;

    // Resolve out-of-bounds coordinate according to BorderMode.
    // Returns the resolved index, or -1 when BorderMode::Zero applies.
    [[nodiscard]] int resolveCoord(int coord, int size) const noexcept;
};

} // namespace DIPAL

#endif // DIPAL_CONVOLUTION_FILTER_HPP
