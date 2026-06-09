// include/DIPAL/Filters/DilationFilter.hpp
#ifndef DIPAL_DILATION_FILTER_HPP
#define DIPAL_DILATION_FILTER_HPP

#include "FilterStrategy.hpp"
#include "StructuringElement.hpp"

namespace DIPAL {

/**
 * @brief Morphological dilation
 *
 * Each output pixel is the maximum of the input pixels covered by the
 * structuring element.  Works on grayscale and colour (per-channel).
 */
class DilationFilter : public FilterStrategy {
public:
    explicit DilationFilter(StructuringElement se = StructuringElement::defaultElement());

    [[nodiscard]] Result<std::unique_ptr<Image>> apply(const Image& image) const override;
    [[nodiscard]] std::string_view getName() const override;
    [[nodiscard]] std::unique_ptr<FilterStrategy> clone() const override;

    [[nodiscard]] const StructuringElement& getStructuringElement() const noexcept { return m_se; }

private:
    StructuringElement m_se;
};

} // namespace DIPAL
#endif // DIPAL_DILATION_FILTER_HPP
