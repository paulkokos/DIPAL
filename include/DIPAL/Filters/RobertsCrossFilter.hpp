// include/DIPAL/Filters/RobertsCrossFilter.hpp
#ifndef DIPAL_ROBERTS_CROSS_FILTER_HPP
#define DIPAL_ROBERTS_CROSS_FILTER_HPP

#include "FilterStrategy.hpp"

namespace DIPAL {

/**
 * @brief Roberts Cross edge detection filter
 *
 * Computes gradient magnitude using the 2x2 Roberts Cross diagonal operators
 * (embedded in 3x3 kernels).  Color images are converted to grayscale first.
 */
class RobertsCrossFilter : public FilterStrategy {
public:
    /**
     * @param normalize  Normalize output to [0, 255] range
     */
    explicit RobertsCrossFilter(bool normalize = true);

    [[nodiscard]] Result<std::unique_ptr<Image>> apply(const Image& image) const override;
    [[nodiscard]] std::string_view getName() const override;
    [[nodiscard]] std::unique_ptr<FilterStrategy> clone() const override;

    [[nodiscard]] bool isNormalized() const noexcept { return m_normalize; }

private:
    bool m_normalize;
};

} // namespace DIPAL

#endif // DIPAL_ROBERTS_CROSS_FILTER_HPP
