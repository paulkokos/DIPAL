// include/DIPAL/Filters/FilterStrategy.hpp
#ifndef DIPAL_FILTER_STRATEGY_HPP
#define DIPAL_FILTER_STRATEGY_HPP

#include <memory>
#include <string>

#include "../Image/Image.hpp"

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
     * @return A new image with the filter applied
     */
    virtual std::unique_ptr<Image> apply(const Image& image) const = 0;

    /**
     * @brief Get the name of the filter
     * @return Filter name
     */
    virtual std::string getName() const = 0;
};

}  // namespace DIPAL

#endif  // DIPAL_FILTER_STRATEGY_HPP
