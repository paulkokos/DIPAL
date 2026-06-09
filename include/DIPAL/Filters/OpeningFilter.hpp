// include/DIPAL/Filters/OpeningFilter.hpp
#ifndef DIPAL_OPENING_FILTER_HPP
#define DIPAL_OPENING_FILTER_HPP

#include "FilterStrategy.hpp"
#include "StructuringElement.hpp"

namespace DIPAL {

/**
 * @brief Morphological opening  (erosion → dilation)
 *
 * Removes small bright objects and smooths object boundaries.
 */
class OpeningFilter : public FilterStrategy {
public:
    explicit OpeningFilter(StructuringElement se = StructuringElement::defaultElement());

    [[nodiscard]] Result<std::unique_ptr<Image>> apply(const Image& image) const override;
    [[nodiscard]] std::string_view getName() const override;
    [[nodiscard]] std::unique_ptr<FilterStrategy> clone() const override;

private:
    StructuringElement m_se;
};

} // namespace DIPAL
#endif // DIPAL_OPENING_FILTER_HPP
