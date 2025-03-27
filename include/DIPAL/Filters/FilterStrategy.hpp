// include/DIPAL/Filters/FilterStrategy.hpp
#ifndef DIPAL_FILTER_STRATEGY_HPP
#define DIPAL_FILTER_STRATEGY_HPP

#include <memory>
#include <string>
#include <string_view>

#include "../Image/Image.hpp"
#include "../Core/Error.hpp"

namespace DIPAL {

/**
 * @brief Abstract strategy for image filters
 *
 * Defines the interface for all image filters in the DIPAL library.
 */
class FilterStrategy {
public:
    virtual ~FilterStrategy() = default;

    /**
     * @brief Apply the filter to an image
     * @param image The image to process
     * @return Result containing the filtered image or error
     */
    [[nodiscard]] virtual Result<std::unique_ptr<Image>> apply(const Image& image) const = 0;

    /**
     * @brief Get the name of the filter
     * @return Filter name
     */
    [[nodiscard]] virtual std::string_view getName() const = 0;
    
    /**
     * @brief Clone the filter
     * @return A new filter that is a copy of this one
     */
    [[nodiscard]] virtual std::unique_ptr<FilterStrategy> clone() const = 0;
};

} // namespace DIPAL

#endif // DIPAL_FILTER_STRATEGY_HPP
