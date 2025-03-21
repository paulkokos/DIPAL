// include/DIPAL/ImageProcessor/ProcessingCommand.hpp
#ifndef DIPAL_PROCESSING_COMMAND_HPP
#define DIPAL_PROCESSING_COMMAND_HPP

#include <memory>
#include <string>

#include "../Image/Image.hpp"

namespace DIPAL {

/**
 * @brief Abstract command interface for image processing operations
 */
class ProcessingCommand {
public:
    virtual ~ProcessingCommand() = default;

    /**
     * @brief Execute the command on an image
     * @param image The image to process
     * @return A new processed image
     */
    virtual std::unique_ptr<Image> execute(const Image& image) = 0;

    /**
     * @brief Get the name of the command
     * @return Command name
     */
    virtual std::string getName() const = 0;

    /**
     * @brief Whether the command can be undone
     * @return true if the command can be undone, false otherwise
     */
    virtual bool isUndoable() const = 0;

    /**
     * @brief Undo the command (if supported)
     * @param image The processed image
     * @return The original image before processing
     */
    virtual std::unique_ptr<Image> undo(const Image& image);
};

}  // namespace DIPAL

#endif  // DIPAL_PROCESSING_COMMAND_HPP
