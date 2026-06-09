// include/DIPAL/Filters/ClosingFilter.hpp
#ifndef DIPAL_CLOSING_FILTER_HPP
#define DIPAL_CLOSING_FILTER_HPP

#include "FilterStrategy.hpp"
#include "StructuringElement.hpp"

namespace DIPAL {

/**
 * @brief Morphological closing  (dilation → erosion)
 *
 * Fills small dark holes and smooths object boundaries.
 */
class ClosingFilter : public FilterStrategy {
public:
    explicit ClosingFilter(StructuringElement se = StructuringElement::defaultElement());

    [[nodiscard]] Result<std::unique_ptr<Image>> apply(const Image& image) const override;
    [[nodiscard]] std::string_view getName() const override;
    [[nodiscard]] std::unique_ptr<FilterStrategy> clone() const override;

private:
    StructuringElement m_se;
};

} // namespace DIPAL
#endif // DIPAL_CLOSING_FILTER_HPP
