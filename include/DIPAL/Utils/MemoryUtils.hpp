// include/DIPAL/Utils/MemoryUtils.hpp
#ifndef DIPAL_MEMORY_UTILS_HPP
#define DIPAL_MEMORY_UTILS_HPP

#include <memory>
#include <vector>
#include <span>
#include <algorithm>
#include <cstring>
#include <stdexcept>

namespace DIPAL {

/**
 * @brief Utility functions for memory management
 */
class MemoryUtils {
public:
    /**
     * @brief Safely copy data between spans with boundary checking
     * @tparam T Data type
     * @param dest Destination span
     * @param src Source span
     * @param max_elements Maximum number of elements to copy
     * @return Number of elements copied
     */
    template <typename T>
    static size_t safeCopy(std::span<T> dest, std::span<const T> src, size_t max_elements = SIZE_MAX) {
        size_t count = std::min({dest.size(), src.size(), max_elements});
        std::copy_n(src.begin(), count, dest.begin());
        return count;
    }
    
    /**
     * @brief Safely set memory with boundary checking
     * @tparam T Data type
     * @param dest Destination span
     * @param value Value to set
     * @return Number of elements set
     */
    template <typename T>
    static size_t safeSet(std::span<T> dest, const T& value) {
        std::fill(dest.begin(), dest.end(), value);
        return dest.size();
    }
    
    /**
     * @brief Allocate aligned memory
     * @param size Size in bytes
     * @param alignment Alignment in bytes (must be power of 2)
     * @return Aligned pointer or nullptr on failure
     */
    static void* alignedAlloc(size_t size, size_t alignment) {
#ifdef _WIN32
        return _aligned_malloc(size, alignment);
#else
        void* ptr = nullptr;
        if (posix_memalign(&ptr, alignment, size) != 0) {
            return nullptr;
        }
        return ptr;
#endif
    }
    
    /**
     * @brief Free aligned memory
     * @param ptr Pointer to aligned memory
     */
    static void alignedFree(void* ptr) {
#ifdef _WIN32
        _aligned_free(ptr);
#else
        free(ptr);
#endif
    }
    
    /**
     * @brief Create a vector with aligned memory
     * @tparam T Element type
     * @param size Number of elements
     * @param alignment Alignment in bytes (must be power of 2)
     * @return Vector with aligned storage
     */
    template <typename T>
    static std::vector<T> createAlignedVector(size_t size, size_t alignment = 32) {
        // Create aligned allocator
        auto alloc = std::allocator<T>();
        
        // Create vector with that allocator
        std::vector<T, std::allocator<T>> vec(size, alloc);
        
        // Ensure the vector's capacity is exactly what we requested
        vec.shrink_to_fit();
        
        // Check alignment (in debug builds)
#ifdef _DEBUG
        if (reinterpret_cast<uintptr_t>(vec.data()) % alignment != 0) {
            throw std::runtime_error("Failed to create properly aligned vector");
        }
#endif
        
        return vec;
    }
};

} // namespace DIPAL

#endif // DIPAL_MEMORY_UTILS_HPP
