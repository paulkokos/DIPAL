// include/DIPAL/ImageProcessor/ParallelProcessor.hpp
#ifndef DIPAL_PARALLEL_PROCESSOR_HPP
#define DIPAL_PARALLEL_PROCESSOR_HPP

#include "ImageProcessor.hpp"
#include "../Utils/Concurrency.hpp"

namespace DIPAL {

/**
 * @brief Image processor that uses parallel processing for operations
 */
class ParallelProcessor : public ImageProcessor {
public:
    /**
     * @brief Create a parallel image processor
     * @param numThreads Number of threads to use (0 for auto)
     */
      explicit ParallelProcessor(size_t numThreads = 0)
        : ImageProcessor(), m_numThreads(numThreads) {
        // Create the thread pool
        m_threadPool = std::make_unique<ThreadPool>(numThreads);
    }
     ParallelProcessor(ParallelProcessor&& other) noexcept
        : ImageProcessor(std::move(other)),
          m_threadPool(std::move(other.m_threadPool)),
          m_numThreads(other.m_numThreads) {
    }

     ParallelProcessor& operator=(ParallelProcessor&& other) noexcept {
        if (this != &other) {
            ImageProcessor::operator=(std::move(other));
            m_threadPool = std::move(other.m_threadPool);
            m_numThreads = other.m_numThreads;
        }
        return *this;
    }

    /**
     * @brief Apply a filter to an image using parallel processing
     * @param image The image to process
     * @param filter The filter to apply
     * @return Result containing the processed image or error
     */
    [[nodiscard]] Result<std::unique_ptr<Image>> applyFilter(
        const Image& image,
        const FilterStrategy& filter
    ) override;
    
    /**
     * @brief Set the number of threads to use
     * @param numThreads Number of threads (0 for auto)
     */
    void setThreadCount(size_t numThreads);
    
    /**
     * @brief Get the number of threads being used
     * @return Number of threads
     */
    [[nodiscard]] size_t getThreadCount() const;

private:
    // Thread pool for parallel processing
    std::unique_ptr<ThreadPool> m_threadPool; 
    // Lock for thread pool access
    mutable std::mutex m_poolMutex;
    
    // Custom thread count (0 means auto)
    size_t m_numThreads;
};
} // namespace DIPAL

#endif // DIPAL_PARALLEL_PROCESSOR_HPP
