// include/DIPAL/Filters/GaussianBlurFilter.hpp
#ifndef DIPAL_GAUSSIAN_BLUR_FILTER_HPP
#define DIPAL_GAUSSIAN_BLUR_FILTER_HPP

#include <vector>
#include "FilterStrategy.hpp"

namespace DIPAL {

/**
 * @brief Gaussian blur filter implementation
 */
class GaussianBlurFilter : public FilterStrategy {
public:
    /**
     * @brief Create a Gaussian blur filter
     * @param sigma Standard deviation of the Gaussian kernel
     * @param kernelSize Size of the kernel (must be odd)
     */
    GaussianBlurFilter(float sigma = 1.0f, int kernelSize = 3);

    /**
     * @brief Apply Gaussian blur to an image
     * @param image The image to process
     * @return Result containing the blurred image or error
     */
    [[nodiscard]] Result<std::unique_ptr<Image>> apply(const Image& image) const override;

    /**
     * @brief Get the name of the filter
     * @return "GaussianBlur"
     */
    [[nodiscard]] std::string_view getName() const override;
    
    /**
     * @brief Clone the filter
     * @return A new Gaussian blur filter with the same parameters
     */
    [[nodiscard]] std::unique_ptr<FilterStrategy> clone() const override;
    
    /**
     * @brief Get the sigma parameter
     * @return Sigma value
     */
    [[nodiscard]] float getSigma() const noexcept;
    
    /**
     * @brief Get the kernel size
     * @return Kernel size
     */
    [[nodiscard]] int getKernelSize() const noexcept;
    
    /**
     * @brief Get the kernel values
     * @return Span containing the kernel values
     */
    [[nodiscard]] std::span<const float> getKernel() const noexcept;

private:
    float m_sigma;
    int m_kernelSize;
    std::vector<float> m_kernel;

    /**
     * @brief Generate the Gaussian kernel
     */
    void generateKernel();
};

} // namespace DIPAL

#endif // DIPAL_GAUSSIAN_BLUR_FILTER_HPP
