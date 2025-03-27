// src/Utils/Concurrency.cpp
#include "../../include/DIPAL/Utils/Concurrency.hpp"

namespace DIPAL {

ThreadPool::ThreadPool(size_t numThreads)
    : m_stop(false), m_activeThreads(0) {
    // Use hardware concurrency if numThreads is 0
    if (numThreads == 0) {
        numThreads = std::thread::hardware_concurrency();
    }
    
    // At least one thread
    numThreads = std::max(numThreads, size_t(1));
    
    // Create worker threads
    for (size_t i = 0; i < numThreads; ++i) {
        m_workers.emplace_back([this]() {
            while (true) {
                std::function<void()> task;
                
                {
                    std::unique_lock<std::mutex> lock(m_queueMutex);
                    
                    // Wait for a task or stop signal
                    m_condition.wait(lock, [this]() {
                        return m_stop || !m_tasks.empty();
                    });
                    
                    // Exit if stopped and no more tasks
                    if (m_stop && m_tasks.empty()) {
                        return;
                    }
                    
                    // Get task from the queue
                    task = std::move(m_tasks.front());
                    m_tasks.pop();
                }
                
                // Execute task
                m_activeThreads++;
                try {
                    task();
                } catch (...) {
                    // Log error in a real implementation
                }
                m_activeThreads--;
                
                // Notify completion if queue is empty and no active threads
                if (m_tasks.empty() && m_activeThreads == 0) {
                    std::lock_guard<std::mutex> lock(m_completionMutex);
                    m_completionCondition.notify_all();
                }
            }
        });
    }
}

ThreadPool::~ThreadPool() {
    {
        std::lock_guard<std::mutex> lock(m_queueMutex);
        m_stop = true;
    }
    
    // Wake up all threads
    m_condition.notify_all();
    
    // Join all threads
    for (auto& worker : m_workers) {
        if (worker.joinable()) {
            worker.join();
        }
    }
}

size_t ThreadPool::getThreadCount() const {
    return m_workers.size();
}

size_t ThreadPool::getQueueSize() const {
    std::lock_guard<std::mutex> lock(m_queueMutex);
    return m_tasks.size();
}

void ThreadPool::waitForCompletion() {
    std::unique_lock<std::mutex> lock(m_completionMutex);
    
    // Wait until there are no tasks in the queue and no active threads
    m_completionCondition.wait(lock, [this]() {
        std::lock_guard<std::mutex> queueLock(m_queueMutex);
        return m_tasks.empty() && m_activeThreads == 0;
    });
}

} // namespace DIPAL
