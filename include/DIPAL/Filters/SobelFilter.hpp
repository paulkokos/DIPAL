// include/DIPAL/Filters/SobelFilter.hpp
#ifndef DIPAL_SOBEL_FILTER_HPP
#define DIPAL_SOBEL_FILTER_HPP

#include "FilterStrategy.hpp"

namespace DIPAL {

/**
 * @brief Sobel edge detection filter
 * 
 * Detects edges using Sobel operators in horizontal and vertical directions
 */
class SobelFilter : public FilterStrategy {
public:
    /**
     * @brief Create a Sobel filter
     * @param normalize If true, normalize the output to 0-255 range
     */
    explicit SobelFilter(bool normalize = true);

    /**
     * @brief Apply Sobel filter to an image
     * @param image The image to process
     * @return Result containing the filtered image or error
     */
    [[nodiscard]] Result<std::unique_ptr<Image>> apply(const Image& image) const override;

    /**
     * @brief Get the name of the filter
     * @return "SobelFilter"
     */
    [[nodiscard]] std::string_view getName() const override;

    /**
     * @brief Clone the filter
     * @return A new Sobel filter with the same parameters
     */
    [[nodiscard]] std::unique_ptr<FilterStrategy> clone() const override;

    /**
     * @brief Check if normalization is enabled
     * @return true if normalization is enabled, false otherwise
     */
    [[nodiscard]] bool isNormalized() const noexcept;

private:
    bool m_normalize;
};

} // namespace DIPAL

#endif // DIPAL_SOBEL_FILTER_HPP
