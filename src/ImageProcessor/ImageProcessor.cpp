// src/ImageProcessor/ImageProcessor.cpp
#include "../../include/DIPAL/ImageProcessor/ImageProcessor.hpp"
#include "../../include/DIPAL/ImageProcessor/FilterCommand.hpp"

#include <algorithm>
#include <format>
#include <stdexcept>

namespace DIPAL {

ImageProcessor::ImageProcessor() = default;

void ImageProcessor::addObserver(std::shared_ptr<ProcessingObserver> observer) {
    if (observer) {
        m_observers.push_back(std::move(observer));
    }
}

void ImageProcessor::removeObserver(std::shared_ptr<ProcessingObserver> observer) {
    if (!observer) {
        return;
    }
    
    m_observers.erase(
        std::remove_if(m_observers.begin(), m_observers.end(),
                      [&observer](const std::shared_ptr<ProcessingObserver>& element) {
                          return element.get() == observer.get();
                      }),
        m_observers.end());
}

Result<std::unique_ptr<Image>> ImageProcessor::process(
    const Image& image, 
    std::unique_ptr<ProcessingCommand> command
) {
    if (!command) {
        notifyError("Invalid command (null)");
        return makeErrorResult<std::unique_ptr<Image>>(
            ErrorCode::InvalidParameter,
            "Command cannot be null"
        );
    }

    notifyProcessingStarted(command->getName());
    notifyProgressUpdated(0.0f);

    try {
        auto result = command->execute(image);
        notifyProgressUpdated(1.0f);

        if (!result) {
            notifyError(std::format("Command '{}' failed: {}", 
                                  command->getName(), result.error().message()));
            notifyProcessingCompleted(command->getName(), false);
            return makeErrorResult<std::unique_ptr<Image>>(
                result.error().code(),
                result.error().message()
            );
        }

        if (command->isUndoable()) {
            m_undoStack.push_back(std::move(command));
        }

        notifyProcessingCompleted(command->getName(), true);
        return makeSuccessResult(std::move(result.value()));
    } catch (const std::exception& e) {
        std::string errorMsg = std::format("Exception during command execution: {}", e.what());
        notifyError(errorMsg);
        notifyProcessingCompleted(command->getName(), false);
        return makeErrorResult<std::unique_ptr<Image>>(
            ErrorCode::ProcessingFailed,
            errorMsg
        );
    }
}

Result<std::unique_ptr<Image>> ImageProcessor::processAll(
    const Image& image,
    std::span<std::unique_ptr<ProcessingCommand>> commands
) {
    if (commands.empty()) {
        return makeSuccessResult(image.clone());
    }

    std::unique_ptr<Image> currentImage = image.clone();
    float stepSize = 1.0f / commands.size();
    float progress = 0.0f;

    notifyProcessingStarted("Processing Pipeline");
    notifyProgressUpdated(progress);

    try {
        for (auto& command : commands) {
            if (!command) {
                notifyError("Null command in pipeline");
                notifyProcessingCompleted("Processing Pipeline", false);
                return makeErrorResult<std::unique_ptr<Image>>(
                    ErrorCode::InvalidParameter,
                    "Null command in pipeline"
                );
            }

            auto result = process(*currentImage, std::move(command));
            if (!result) {
                notifyProcessingCompleted("Processing Pipeline", false);
                return result;
            }

            currentImage = std::move(result.value());
            progress += stepSize;
            notifyProgressUpdated(progress);
        }

        notifyProcessingCompleted("Processing Pipeline", true);
        return makeSuccessResult(std::move(currentImage));
    } catch (const std::exception& e) {
        std::string errorMsg = std::format("Exception during pipeline execution: {}", e.what());
        notifyError(errorMsg);
        notifyProcessingCompleted("Processing Pipeline", false);
        return makeErrorResult<std::unique_ptr<Image>>(
            ErrorCode::ProcessingFailed,
            errorMsg
        );
    }
}

Result<std::unique_ptr<Image>> ImageProcessor::applyFilter(
    const Image& image, 
    const FilterStrategy& filter
) {
    notifyProcessingStarted(filter.getName());
    notifyProgressUpdated(0.0f);

    try {
        auto result = filter.apply(image);
        notifyProgressUpdated(1.0f);

        if (!result) {
            notifyError(std::format("Filter '{}' failed: {}", 
                                  filter.getName(), result.error().message()));
            notifyProcessingCompleted(filter.getName(), false);
            return result;
        }

        notifyProcessingCompleted(filter.getName(), true);
        return result;
    } catch (const std::exception& e) {
        std::string errorMsg = std::format("Exception during filter application: {}", e.what());
        notifyError(errorMsg);
        notifyProcessingCompleted(filter.getName(), false);
        return makeErrorResult<std::unique_ptr<Image>>(
            ErrorCode::ProcessingFailed,
            errorMsg
        );
    }
}
/*  TODO: there is a bug in the whole Undo functionality
Result<std::unique_ptr<Image>> ImageProcessor::undo(const Image& image) {
    if (!canUndo()) {
        notifyError("No operations to undo");
        return makeErrorResult<std::unique_ptr<Image>>(
            ErrorCode::InvalidParameter,
            "No operations to undo"
        );
    }

    auto command = std::move(m_undoStack.back());
    m_undoStack.pop_back();

    std::string operationName = std::format("Undo {}", command->getName());
    notifyProcessingStarted(operationName);

    try {
        auto result = command->undo(image);
        
        if (!result) {
            notifyError(std::format("Undo failed: {}", result.error().message()));
            notifyProcessingCompleted(operationName, false);
            return result;
        }
        
        notifyProcessingCompleted(operationName, true);
        return result;
    } catch (const std::exception& e) {
        std::string errorMsg = std::format("Exception during undo: {}", e.what());
        notifyError(errorMsg);
        notifyProcessingCompleted(operationName, false);
        return makeErrorResult<std::unique_ptr<Image>>(
            ErrorCode::ProcessingFailed,
            errorMsg
        );
    }
}
*/
bool ImageProcessor::canUndo() const noexcept {
    return !m_undoStack.empty();
}

size_t ImageProcessor::getUndoCount() const noexcept {
    return m_undoStack.size();
}

void ImageProcessor::notifyProcessingStarted(std::string_view operationName) {
    for (auto& observer : m_observers) {
        observer->onProcessingStarted(operationName);
    }
}

void ImageProcessor::notifyProgressUpdated(float progress) {
    // Clamp progress to [0.0, 1.0]
    progress = std::clamp(progress, 0.0f, 1.0f);
    
    for (auto& observer : m_observers) {
        observer->onProgressUpdated(progress);
    }
}

void ImageProcessor::notifyProcessingCompleted(std::string_view operationName, bool success) {
    for (auto& observer : m_observers) {
        observer->onProcessingCompleted(operationName, success);
    }
}

void ImageProcessor::notifyError(std::string_view errorMessage) {
    for (auto& observer : m_observers) {
        observer->onError(errorMessage);
    }
}

} // namespace DIPAL
