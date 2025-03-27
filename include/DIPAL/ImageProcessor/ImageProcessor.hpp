// include/DIPAL/ImageProcessor/ImageProcessor.hpp
#ifndef DIPAL_IMAGE_PROCESSOR_HPP
#define DIPAL_IMAGE_PROCESSOR_HPP

#include <deque>
#include <memory>
#include <vector>
#include <span>

#include "../Image/Image.hpp"
#include "../Observer/ProcessingObserver.hpp"
#include "ProcessingCommand.hpp"
#include "../Core/Error.hpp"

namespace DIPAL {

/**
 * @brief Main class for processing images
 *
 * This class orchestrates the image processing operations using
 * commands and notifies observers of progress.
 */
class ImageProcessor {
public:
    /**
     * @brief Create a new image processor
     */
    ImageProcessor();

    /**
     * @brief Add an observer to monitor processing
     * @param observer The observer to add
     */
    void addObserver(std::shared_ptr<ProcessingObserver> observer);

    /**
     * @brief Remove an observer
     * @param observer The observer to remove
     */
    void removeObserver(std::shared_ptr<ProcessingObserver> observer);

    /**
     * @brief Process an image with a command
     * @param image The image to process
     * @param command The command to apply
     * @return Result containing the processed image or error
     */
    [[nodiscard]] Result<std::unique_ptr<Image>> process(
        const Image& image, 
        std::unique_ptr<ProcessingCommand> command
    );

    /**
     * @brief Process an image with multiple commands in sequence
     * @param image The image to process
     * @param commands Span of commands to apply in order
     * @return Result containing the processed image or error
     */
    [[nodiscard]] Result<std::unique_ptr<Image>> processAll(
        const Image& image,
        std::span<std::unique_ptr<ProcessingCommand>> commands
    );

    /**
     * @brief Process an image with a filter
     * @param image The image to process
     * @param filter The filter to apply
     * @return Result containing the processed image or error
     */
    [[nodiscard]] Result<std::unique_ptr<Image>> applyFilter(
        const Image& image, 
        const FilterStrategy& filter
    );

    /**
     * @brief Undo the last command if possible
     * @param image The current image
     * @return Result containing the image before the last command, or error
     */
    [[nodiscard]] Result<std::unique_ptr<Image>> undo(const Image& image);

    /**
     * @brief Check if undo is available
     * @return true if undo is available, false otherwise
     */
    [[nodiscard]] bool canUndo() const noexcept;
    
    /**
     * @brief Get the number of commands in the undo stack
     * @return Number of undoable commands
     */
    [[nodiscard]] size_t getUndoCount() const noexcept;

private:
    std::vector<std::shared_ptr<ProcessingObserver>> m_observers;
    std::deque<std::unique_ptr<ProcessingCommand>> m_undoStack;

    /**
     * @brief Notify observers that processing has started
     * @param operationName Name of the operation
     */
    void notifyProcessingStarted(std::string_view operationName);

    /**
     * @brief Notify observers of progress update
     * @param progress Progress value between 0.0 and 1.0
     */
    void notifyProgressUpdated(float progress);

    /**
     * @brief Notify observers that processing has completed
     * @param operationName Name of the operation
     * @param success Whether the operation was successful
     */
    void notifyProcessingCompleted(std::string_view operationName, bool success);

    /**
     * @brief Notify observers of an error
     * @param errorMessage Description of the error
     */
    void notifyError(std::string_view errorMessage);
};

} // namespace DIPAL

#endif // DIPAL_IMAGE_PROCESSOR_HPP
