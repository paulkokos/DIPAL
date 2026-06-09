// include/DIPAL/Filters/SkeletonFilter.hpp
#ifndef DIPAL_SKELETON_FILTER_HPP
#define DIPAL_SKELETON_FILTER_HPP

#include "FilterStrategy.hpp"

namespace DIPAL {

/**
 * @brief Morphological skeleton / thinning via Zhang-Suen algorithm
 *
 * Reduces binary foreground objects to 1-pixel-wide skeletons while
 * preserving topology.  Input is thresholded at 128 before processing.
 * Output: 255 = skeleton pixel, 0 = background.
 */
class SkeletonFilter : public FilterStrategy {
public:
    /**
     * @param threshold  Binarisation threshold applied to the input (default 128)
     */
    explicit SkeletonFilter(uint8_t threshold = 128);

    [[nodiscard]] Result<std::unique_ptr<Image>> apply(const Image& image) const override;
    [[nodiscard]] std::string_view getName() const override;
    [[nodiscard]] std::unique_ptr<FilterStrategy> clone() const override;

    [[nodiscard]] uint8_t getThreshold() const noexcept { return m_threshold; }

private:
    uint8_t m_threshold;
};

} // namespace DIPAL
#endif // DIPAL_SKELETON_FILTER_HPP
