// include/DIPAL/Filters/MorphologicalGradientFilter.hpp
#ifndef DIPAL_MORPHOLOGICAL_GRADIENT_FILTER_HPP
#define DIPAL_MORPHOLOGICAL_GRADIENT_FILTER_HPP

#include "FilterStrategy.hpp"
#include "StructuringElement.hpp"

namespace DIPAL {

/**
 * @brief Morphological gradient  (dilation − erosion)
 *
 * Highlights object boundaries.
 */
class MorphologicalGradientFilter : public FilterStrategy {
public:
    explicit MorphologicalGradientFilter(StructuringElement se = StructuringElement::defaultElement());

    [[nodiscard]] Result<std::unique_ptr<Image>> apply(const Image& image) const override;
    [[nodiscard]] std::string_view getName() const override;
    [[nodiscard]] std::unique_ptr<FilterStrategy> clone() const override;

private:
    StructuringElement m_se;
};

} // namespace DIPAL
#endif // DIPAL_MORPHOLOGICAL_GRADIENT_FILTER_HPP
