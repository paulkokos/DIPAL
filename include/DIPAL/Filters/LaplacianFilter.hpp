// include/DIPAL/Filters/LaplacianFilter.hpp
#ifndef DIPAL_LAPLACIAN_FILTER_HPP
#define DIPAL_LAPLACIAN_FILTER_HPP

#include "FilterStrategy.hpp"

namespace DIPAL {

/**
 * @brief Laplacian edge detection filter
 *
 * Applies the discrete Laplacian operator to detect edges and regions of
 * rapid intensity change.  Color images are converted to grayscale first.
 */
class LaplacianFilter : public FilterStrategy {
public:
    /**
     * @param use8Neighbor  false = 4-neighbour kernel, true = 8-neighbour kernel
     * @param normalize     Normalize output to [0, 255] range
     */
    explicit LaplacianFilter(bool use8Neighbor = false, bool normalize = true);

    [[nodiscard]] Result<std::unique_ptr<Image>> apply(const Image& image) const override;
    [[nodiscard]] std::string_view getName() const override;
    [[nodiscard]] std::unique_ptr<FilterStrategy> clone() const override;

    [[nodiscard]] bool isUsing8Neighbor() const noexcept { return m_use8Neighbor; }
    [[nodiscard]] bool isNormalized()     const noexcept { return m_normalize; }

private:
    bool m_use8Neighbor;
    bool m_normalize;
};

} // namespace DIPAL

#endif // DIPAL_LAPLACIAN_FILTER_HPP
