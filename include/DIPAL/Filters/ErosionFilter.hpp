// include/DIPAL/Filters/ErosionFilter.hpp
#ifndef DIPAL_EROSION_FILTER_HPP
#define DIPAL_EROSION_FILTER_HPP

#include "FilterStrategy.hpp"
#include "StructuringElement.hpp"

namespace DIPAL {

/**
 * @brief Morphological erosion
 *
 * Each output pixel is the minimum of the input pixels covered by the
 * structuring element.  Works on grayscale and colour (per-channel).
 */
class ErosionFilter : public FilterStrategy {
public:
    explicit ErosionFilter(StructuringElement se = StructuringElement::defaultElement());

    [[nodiscard]] Result<std::unique_ptr<Image>> apply(const Image& image) const override;
    [[nodiscard]] std::string_view getName() const override;
    [[nodiscard]] std::unique_ptr<FilterStrategy> clone() const override;

    [[nodiscard]] const StructuringElement& getStructuringElement() const noexcept { return m_se; }

private:
    StructuringElement m_se;
};

} // namespace DIPAL
#endif // DIPAL_EROSION_FILTER_HPP
