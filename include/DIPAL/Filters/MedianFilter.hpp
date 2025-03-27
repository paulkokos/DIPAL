// include/DIPAL/Filters/MedianFilter.hpp
#ifndef DIPAL_MEDIAN_FILTER_HPP
#define DIPAL_MEDIAN_FILTER_HPP

#include "FilterStrategy.hpp"

namespace DIPAL {

/**
 * @brief Median filter implementation
 * 
 * Applies a median filter to reduce noise while preserving edges
 */
class MedianFilter : public FilterStrategy {
public:
    /**
     * @brief Create a median filter
     * @param kernelSize Size of the kernel (must be odd)
     */
    explicit MedianFilter(int kernelSize = 3);

    /**
     * @brief Apply median filter to an image
     * @param image The image to process
     * @return Result containing the filtered image or error
     */
    [[nodiscard]] Result<std::unique_ptr<Image>> apply(const Image& image) const override;

    /**
     * @brief Get the name of the filter
     * @return "MedianFilter"
     */
    [[nodiscard]] std::string_view getName() const override;

    /**
     * @brief Clone the filter
     * @return A new median filter with the same parameters
     */
    [[nodiscard]] std::unique_ptr<FilterStrategy> clone() const override;

    /**
     * @brief Get the kernel size
     * @return Kernel size
     */
    [[nodiscard]] int getKernelSize() const noexcept;

private:
    int m_kernelSize;
};

} // namespace DIPAL

#endif // DIPAL_MEDIAN_FILTER_HPP
