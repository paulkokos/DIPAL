// src/ImageProcessor/ImageProcessor.cpp
#include "../../include/DIPAL/ImageProcessor/ImageProcessor.hpp"

#include <algorithm>
namespace DIPAL {

ImageProcessor::ImageProcessor() {
    // Initialize any needed members
}

void ImageProcessor::addObserver(std::shared_ptr<ProcessingObserver> observer) {
    m_observers.push_back(observer);
}

void ImageProcessor::removeObserver(std::shared_ptr<ProcessingObserver> observer) {
    // Use std::remove_if instead of std::find with a lambda to compare the objects
    m_observers.erase(
        std::remove_if(m_observers.begin(),
                       m_observers.end(),
                       [&observer](const std::shared_ptr<ProcessingObserver>& element) {
                           return element.get() == observer.get();
                       }),
        m_observers.end());
}
std::unique_ptr<Image> ImageProcessor::process(const Image& image,
                                               std::unique_ptr<ProcessingCommand> command) {
    if (!command) {
        notifyError("Invalid command");
        return nullptr;
    }

    notifyProcessingStarted(command->getName());

    try {
        notifyProgressUpdated(0.0f);
        auto result = command->execute(image);
        notifyProgressUpdated(1.0f);

        if (command->isUndoable()) {
            m_undoStack.push_back(std::move(command));
        }

        notifyProcessingCompleted(command->getName(), true);
        return result;
    } catch (const std::exception& e) {
        notifyError(e.what());
        notifyProcessingCompleted(command->getName(), false);
        return nullptr;
    }
}

std::unique_ptr<Image> ImageProcessor::processAll(
    const Image& image,
    std::vector<std::unique_ptr<ProcessingCommand>> commands) {
    if (commands.empty()) {
        return image.clone();
    }

    std::unique_ptr<Image> currentImage = image.clone();
    float stepSize = 1.0f / commands.size();
    float progress = 0.0f;

    notifyProcessingStarted("Processing Pipeline");
    notifyProgressUpdated(progress);

    try {
        for (auto& command : commands) {
            auto tempImage = process(*currentImage, std::move(command));
            if (!tempImage) {
                notifyProcessingCompleted("Processing Pipeline", false);
                return nullptr;
            }

            currentImage = std::move(tempImage);
            progress += stepSize;
            notifyProgressUpdated(progress);
        }

        notifyProcessingCompleted("Processing Pipeline", true);
        return currentImage;
    } catch (const std::exception& e) {
        notifyError(e.what());
        notifyProcessingCompleted("Processing Pipeline", false);
        return nullptr;
    }
}

std::unique_ptr<Image> ImageProcessor::undo(const Image& image) {
    if (!canUndo()) {
        notifyError("No operations to undo");
        return nullptr;
    }

    auto command = std::move(m_undoStack.back());
    m_undoStack.pop_back();

    notifyProcessingStarted("Undo " + command->getName());

    try {
        auto result = command->undo(image);
        notifyProcessingCompleted("Undo " + command->getName(), true);
        return result;
    } catch (const std::exception& e) {
        notifyError(e.what());
        notifyProcessingCompleted("Undo " + command->getName(), false);
        return nullptr;
    }
}

bool ImageProcessor::canUndo() const {
    return !m_undoStack.empty();
}

void ImageProcessor::notifyProcessingStarted(const std::string& operationName) {
    for (auto& observer : m_observers) {
        observer->onProcessingStarted(operationName);
    }
}

void ImageProcessor::notifyProgressUpdated(float progress) {
    for (auto& observer : m_observers) {
        observer->onProgressUpdated(progress);
    }
}

void ImageProcessor::notifyProcessingCompleted(const std::string& operationName, bool success) {
    for (auto& observer : m_observers) {
        observer->onProcessingCompleted(operationName, success);
    }
}

void ImageProcessor::notifyError(const std::string& errorMessage) {
    for (auto& observer : m_observers) {
        observer->onError(errorMessage);
    }
}

}  // namespace DIPAL
