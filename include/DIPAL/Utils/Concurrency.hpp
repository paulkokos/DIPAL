// include/DIPAL/Utils/Concurrency.hpp
#ifndef DIPAL_CONCURRENCY_HPP
#define DIPAL_CONCURRENCY_HPP

#include <thread>
#include <mutex>
#include <condition_variable>
#include <future>
#include <queue>
#include <functional>
#include <vector>
#include <memory>
#include <atomic>
#include <optional>
#include <type_traits>

namespace DIPAL {

/**
 * @brief Thread pool for parallel image processing
 * 
 * Allows efficient execution of tasks across multiple threads.
 */
class ThreadPool {
public:
    /**
     * @brief Create a thread pool
     * @param numThreads Number of worker threads (default: hardware concurrency)
     */
    explicit ThreadPool(size_t numThreads = 0);
    
    /**
     * @brief Destructor - stops all threads
     */
    ~ThreadPool();
    
    // Disable copying
    ThreadPool(const ThreadPool&) = delete;
    ThreadPool& operator=(const ThreadPool&) = delete;
    
    // Allow moving
    ThreadPool(ThreadPool&&) noexcept = delete;
    ThreadPool& operator=(ThreadPool&&) noexcept = delete;
    
    /**
     * @brief Submit a task to be executed by the pool
     * @tparam F Function type
     * @tparam Args Argument types
     * @param f Function to execute
     * @param args Arguments to pass to the function
     * @return Future for the result
     */
    template<typename F, typename... Args>
    auto submit(F&& f, Args&&... args) -> std::future<std::invoke_result_t<F, Args...>> {
        using ReturnType = std::invoke_result_t<F, Args...>;
        
        auto task = std::make_shared<std::packaged_task<ReturnType()>>(
            std::bind(std::forward<F>(f), std::forward<Args>(args)...)
        );
        
        auto future = task->get_future();
        
        {
            std::lock_guard<std::mutex> lock(m_queueMutex);
            
            // Don't allow enqueueing after stopping the pool
            if (m_stop) {
                throw std::runtime_error("Cannot enqueue on stopped ThreadPool");
            }
            
            m_tasks.emplace([task]() { (*task)(); });
        }
        
        m_condition.notify_one();
        return future;
    }
    
    /**
     * @brief Get the number of active threads
     * @return Number of threads
     */
    size_t getThreadCount() const;
    
    /**
     * @brief Get the number of tasks waiting to be processed
     * @return Queue size
     */
    size_t getQueueSize() const;
    
    /**
     * @brief Wait for all tasks to complete
     */
    void waitForCompletion();

private:
    // Worker threads
    std::vector<std::thread> m_workers;
    
    // Task queue
    std::queue<std::function<void()>> m_tasks;
    
    // Synchronization
    mutable std::mutex m_queueMutex;
    std::condition_variable m_condition;
    std::atomic<bool> m_stop;
    std::atomic<size_t> m_activeThreads;
    
    // Completion tracking
    std::mutex m_completionMutex;
    std::condition_variable m_completionCondition;
};

/**
 * @brief Parallelizes execution of a function over a range
 * @tparam IndexType Type of the index (usually int)
 * @tparam Func Function type
 * @param start Start index (inclusive)
 * @param end End index (exclusive)
 * @param func Function to execute for each index
 * @param numThreads Number of threads to use (0 for auto)
 */
template<typename IndexType, typename Func>
void parallelFor(IndexType start, IndexType end, Func func, size_t numThreads = 0) {
    if (numThreads == 0) {
        numThreads = std::thread::hardware_concurrency();
    }
    
    // For small ranges, just do it sequentially
    if (end - start <= static_cast<IndexType>(numThreads) || numThreads == 1) {
        for (IndexType i = start; i < end; ++i) {
            func(i);
        }
        return;
    }
    
    // Create thread pool
    ThreadPool pool(numThreads);
    
    // Calculate chunk size
    IndexType chunkSize = (end - start) / static_cast<IndexType>(numThreads);
    
    // Submit tasks
    std::vector<std::future<void>> futures;
    for (size_t t = 0; t < numThreads; ++t) {
        IndexType chunkStart = start + static_cast<IndexType>(t) * chunkSize;
        IndexType chunkEnd = (t == numThreads - 1) ? end : chunkStart + chunkSize;
        
        futures.push_back(pool.submit([func, chunkStart, chunkEnd]() {
            for (IndexType i = chunkStart; i < chunkEnd; ++i) {
                func(i);
            }
        }));
    }
    
    // Wait for all tasks to complete
    for (auto& future : futures) {
        future.get();
    }
}

} // namespace DIPAL

#endif // DIPAL_CONCURRENCY_HPP
