// include/DIPAL/Filters/UnsharpMaskFilter.hpp
#ifndef DIPAL_UNSHARP_MASK_FILTER_HPP
#define DIPAL_UNSHARP_MASK_FILTER_HPP

#include "FilterStrategy.hpp"  // Fixed the typo here (was "FilterStategy.hpp")
#include "GaussianBlurFilter.hpp"

#include <cstdint>  // For uint8_t, uint16_t, etc.

namespace DIPAL {

/**
 * @brief Unsharp mask filter implementation
 *
 * Enhances image details by subtracting a blurred version from the original image
 */
class UnsharpMaskFilter : public FilterStrategy {
public:
    /**
     * @brief Create an unsharp mask filter
     * @param amount Strength of the effect (1.0 is normal, higher values increase sharpness)
     * @param radius Radius of the Gaussian blur (sigma value)
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
     * @return "UnsharpMask"
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
    float m_amount;       ///< Strength of the effect
    float m_radius;       ///< Radius of the Gaussian blur
    uint8_t m_threshold;  ///< Minimum brightness difference
};

}  // namespace DIPAL

#endif  // DIPAL_UNSHARP_MASK_FILTER_HPP
