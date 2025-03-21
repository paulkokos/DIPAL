// include/DIPAL/Observer/ProcessingObserver.hpp
#ifndef DIPAL_PROCESSING_OBSERVER_HPP
#define DIPAL_PROCESSING_OBSERVER_HPP

#include <string>

namespace DIPAL {

/**
 * @brief Observer interface for monitoring image processing operations
 */
class ProcessingObserver {
public:
    virtual ~ProcessingObserver() = default;

    /**
     * @brief Called when processing starts
     * @param operationName Name of the operation
     */
    virtual void onProcessingStarted(const std::string& operationName) = 0;

    /**
     * @brief Called to update progress
     * @param progress Progress value between 0.0 and 1.0
     */
    virtual void onProgressUpdated(float progress) = 0;

    /**
     * @brief Called when processing is completed
     * @param operationName Name of the operation
     * @param success Whether the operation was successful
     */
    virtual void onProcessingCompleted(const std::string& operationName, bool success) = 0;

    /**
     * @brief Called when an error occurs
     * @param errorMessage Description of the error
     */
    virtual void onError(const std::string& errorMessage) = 0;
};

/**
 * @brief Concrete console observer implementation
 */
class ConsoleObserver : public ProcessingObserver {
public:
    void onProcessingStarted(const std::string& operationName) override;
    void onProgressUpdated(float progress) override;
    void onProcessingCompleted(const std::string& operationName, bool success) override;
    void onError(const std::string& errorMessage) override;
};

}  // namespace DIPAL

#endif  // DIPAL_PROCESSING_OBSERVER_HPP
