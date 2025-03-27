// include/DIPAL/ImageProcessor/FilterCommand.hpp
#ifndef DIPAL_FILTER_COMMAND_HPP
#define DIPAL_FILTER_COMMAND_HPP

#include <memory>

#include "../Filters/FilterStrategy.hpp"
#include "ProcessingCommand.hpp"

namespace DIPAL {

/**
 * @brief Command for applying filters to images
 */
class FilterCommand : public ProcessingCommand {
public:
    /**
     * @brief Create a filter command
     * @param filter The filter strategy to use
     */
    explicit FilterCommand(std::unique_ptr<FilterStrategy> filter);

    /**
     * @brief Apply the filter to an image
     * @param image The image to process
     * @return Result containing the filtered image or error
     */
    Result<std::unique_ptr<Image>> execute(const Image& image) override;

    /**
     * @brief Get the name of the command
     * @return Filter name
     */
    std::string_view getName() const override;

    /**
     * @brief Whether the filter can be undone
     * @return Always returns false for filters
     */
    bool isUndoable() const override;

private:
    std::unique_ptr<FilterStrategy> m_filter;
};

} // namespace DIPAL

#endif // DIPAL_FILTER_COMMAND_HPP
