// include/DIPAL/Filters/UnsharpMaskFilter.hpp
#ifndef DIPAL_UNSHARP_MASK_FILTER_HPP
#define DIPAL_UNSHARP_MASK_FILTER_HPP

#include "FilterStrategy.hpp"

namespace DIPAL {

/**
 * @brief Unsharp Mask filter implementation
 *
 * Enhances edges by subtracting a blurred version of the image from the original.
 * The formula is: result = original + amount * (original - blurred)
 */
class UnsharpMaskFilter : public FilterStrategy {
public:
    /**
     * @brief Create an unsharp mask filter
     * @param amount Strength of the sharpening effect (typically 0.5-2.0)
     * @param radius Blur radius for the mask
     * @param threshold Minimum brightness difference to apply sharpening
     */
    UnsharpMaskFilter(float amount = 1.0f, float radius = 1.0f, uint8_t threshold = 0);

    /**
     * @brief Apply unsharp mask to an image
     * @param image The image to process
     * @return Result containing the sharpened image or error
     */
    [[nodiscard]] Result<std::unique_ptr<Image>> apply(const Image& image) const override;

    /**
     * @brief Get the name of the filter
     * @return "UnsharpMaskFilter"
     */
    [[nodiscard]] std::string_view getName() const override;

    /**
     * @brief Clone the filter
     * @return A new unsharp mask filter with the same parameters
     */
    [[nodiscard]] std::unique_ptr<FilterStrategy> clone() const override;

    /**
     * @brief Get the amount parameter
     * @return Amount value
     */
    [[nodiscard]] float getAmount() const noexcept;

    /**
     * @brief Get the radius parameter
     * @return Radius value
     */
    [[nodiscard]] float getRadius() const noexcept;

    /**
     * @brief Get the threshold parameter
     * @return Threshold value
     */
    [[nodiscard]] uint8_t getThreshold() const noexcept;

private:
    float m_amount;       ///< Strength of the sharpening effect
    float m_radius;       ///< Blur radius for the mask
    uint8_t m_threshold;  ///< Minimum brightness difference to apply sharpening
};

}  // namespace DIPAL

#endif  // DIPAL_UNSHARP_MASK_FILTER_HPP
