// include/DIPAL/Filters/PrewittFilter.hpp
#ifndef DIPAL_PREWITT_FILTER_HPP
#define DIPAL_PREWITT_FILTER_HPP

#include "FilterStrategy.hpp"

namespace DIPAL {

/**
 * @brief Prewitt edge detection filter
 *
 * Computes gradient magnitude from horizontal and vertical Prewitt operators.
 * Color images are converted to grayscale first.
 */
class PrewittFilter : public FilterStrategy {
public:
    /**
     * @param normalize  Normalize output to [0, 255] range
     */
    explicit PrewittFilter(bool normalize = true);

    [[nodiscard]] Result<std::unique_ptr<Image>> apply(const Image& image) const override;
    [[nodiscard]] std::string_view getName() const override;
    [[nodiscard]] std::unique_ptr<FilterStrategy> clone() const override;

    [[nodiscard]] bool isNormalized() const noexcept { return m_normalize; }

private:
    bool m_normalize;
};

} // namespace DIPAL

#endif // DIPAL_PREWITT_FILTER_HPP
