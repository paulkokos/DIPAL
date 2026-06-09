// include/DIPAL/Filters/TopHatFilter.hpp
#ifndef DIPAL_TOP_HAT_FILTER_HPP
#define DIPAL_TOP_HAT_FILTER_HPP

#include "FilterStrategy.hpp"
#include "StructuringElement.hpp"

namespace DIPAL {

/**
 * @brief Top-hat transform
 *
 * White top-hat: original − opening  (extracts small bright features)
 * Black top-hat: closing − original  (extracts small dark features)
 */
class TopHatFilter : public FilterStrategy {
public:
    enum class Type { White, Black };

    explicit TopHatFilter(Type type = Type::White,
                          StructuringElement se = StructuringElement::defaultElement());

    [[nodiscard]] Result<std::unique_ptr<Image>> apply(const Image& image) const override;
    [[nodiscard]] std::string_view getName() const override;
    [[nodiscard]] std::unique_ptr<FilterStrategy> clone() const override;

    [[nodiscard]] Type getType() const noexcept { return m_type; }

private:
    Type               m_type;
    StructuringElement m_se;
};

} // namespace DIPAL
#endif // DIPAL_TOP_HAT_FILTER_HPP
