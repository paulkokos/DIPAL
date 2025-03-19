// include/DIPAL/Filters/GaussianBlurFilter.hpp
#ifndef DIPAL_GAUSSIAN_BLUR_FILTER_HPP
#define DIPAL_GAUSSIAN_BLUR_FILTER_HPP

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
     * @return A new blurred image
     */
    std::unique_ptr<Image> apply(const Image& image) const override;

    /**
     * @brief Get the name of the filter
     * @return "Gaussian Blur"
     */
    std::string getName() const override;

private:
    float m_sigma;
    int m_kernelSize;
    std::vector<float> m_kernel;

    /**
     * @brief Generate the Gaussian kernel
     */
    void generateKernel();
};

}  // namespace DIPAL

#endif  // DIPAL_GAUSSIAN_BLUR_FILTER_HPP
