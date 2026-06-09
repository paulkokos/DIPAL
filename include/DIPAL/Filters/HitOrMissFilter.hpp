// include/DIPAL/Filters/HitOrMissFilter.hpp
#ifndef DIPAL_HIT_OR_MISS_FILTER_HPP
#define DIPAL_HIT_OR_MISS_FILTER_HPP

#include "FilterStrategy.hpp"
#include "StructuringElement.hpp"

namespace DIPAL {

/**
 * @brief Hit-or-Miss transform
 *
 * Detects pixels that match a foreground structuring element AND whose
 * complement matches a background structuring element simultaneously.
 * Input is treated as binary (threshold 128).  Output: 255 = match, 0 = no match.
 */
class HitOrMissFilter : public FilterStrategy {
public:
    /**
     * @param foreground  SE applied to the image (pixels that must be ON)
     * @param background  SE applied to the complement (pixels that must be OFF)
     */
    HitOrMissFilter(StructuringElement foreground, StructuringElement background);

    [[nodiscard]] Result<std::unique_ptr<Image>> apply(const Image& image) const override;
    [[nodiscard]] std::string_view getName() const override;
    [[nodiscard]] std::unique_ptr<FilterStrategy> clone() const override;

private:
    StructuringElement m_foreground;
    StructuringElement m_background;
};

} // namespace DIPAL
#endif // DIPAL_HIT_OR_MISS_FILTER_HPP
