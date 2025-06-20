// include/DIPAL/Core/Core.hpp
#ifndef DIPAL_CORE_HPP
#define DIPAL_CORE_HPP

#include "Error.hpp"
#include "Types.hpp"

#include <array>
#include <chrono>
#include <cstdint>
#include <memory>
#include <span>
#include <string>
#include <string_view>

// Version information
#define DIPAL_VERSION_MAJOR 0
#define DIPAL_VERSION_MINOR 1
#define DIPAL_VERSION_PATCH 0

namespace DIPAL {

/**
 * @brief DIPAL Library version information
 */
struct Version {
    int major;
    int minor;
    int patch;

    constexpr Version(int maj, int min, int pat) noexcept : major(maj), minor(min), patch(pat) {}

    [[nodiscard]] std::string toString() const;
    [[nodiscard]] constexpr bool operator==(const Version& other) const noexcept {
        return major == other.major && minor == other.minor && patch == other.patch;
    }
    [[nodiscard]] constexpr bool operator<(const Version& other) const noexcept {
        if (major != other.major)
            return major < other.major;
        if (minor != other.minor)
            return minor < other.minor;
        return patch < other.patch;
    }
};

/**
 * @brief DIPAL Library build information
 */
struct BuildInfo {
    std::string_view buildDate;
    std::string_view buildTime;
    std::string_view compiler;
    std::string_view platform;
    bool debugBuild;

    [[nodiscard]] std::string toString() const;
};

/**
 * @brief Core library functionality and utilities
 */
class Core {
public:
    // Library information
    [[nodiscard]] static constexpr Version getVersion() noexcept {
        return Version(DIPAL_VERSION_MAJOR, DIPAL_VERSION_MINOR, DIPAL_VERSION_PATCH);
    }

    [[nodiscard]] static std::string getVersionString();
    [[nodiscard]] static const BuildInfo& getBuildInfo();

    // Library initialization and cleanup
    [[nodiscard]] static VoidResult initialize();
    static void shutdown();
    [[nodiscard]] static bool isInitialized() noexcept;

    // Memory utilities
    [[nodiscard]] static std::size_t getMemoryUsage();
    [[nodiscard]] static std::size_t getPeakMemoryUsage();
    static void resetPeakMemoryUsage();

    // Performance utilities
    [[nodiscard]] static std::chrono::high_resolution_clock::time_point now() noexcept;
    [[nodiscard]] static double elapsedMs(
        std::chrono::high_resolution_clock::time_point start) noexcept;

    // Math utilities commonly used in image processing
    template <typename T>
    [[nodiscard]] static constexpr T clamp(T value, T min, T max) noexcept {
        return value < min ? min : (value > max ? max : value);
    }

    template <typename T>
    [[nodiscard]] static constexpr T lerp(T a, T b, float t) noexcept {
        return static_cast<T>(a + t * (b - a));
    }

    [[nodiscard]] static constexpr bool isPowerOfTwo(uint32_t x) noexcept {
        return x > 0 && (x & (x - 1)) == 0;
    }

    [[nodiscard]] static constexpr uint32_t nextPowerOfTwo(uint32_t x) noexcept {
        if (x <= 1)
            return 1;
        --x;
        x |= x >> 1;
        x |= x >> 2;
        x |= x >> 4;
        x |= x >> 8;
        x |= x >> 16;
        return ++x;
    }

    // Bit manipulation utilities
    [[nodiscard]] static constexpr uint8_t reverseBits(uint8_t value) noexcept {
        value = ((value & 0xF0) >> 4) | ((value & 0x0F) << 4);
        value = ((value & 0xCC) >> 2) | ((value & 0x33) << 2);
        value = ((value & 0xAA) >> 1) | ((value & 0x55) << 1);
        return value;
    }

    [[nodiscard]] static constexpr int countSetBits(uint32_t value) noexcept {
        int count = 0;
        while (value) {
            count += value & 1;
            value >>= 1;
        }
        return count;
    }

    // Validation utilities
    [[nodiscard]] static constexpr bool isValidDimension(int dimension) noexcept {
        return dimension > 0 && dimension <= 65536;  // Reasonable max size
    }

    [[nodiscard]] static constexpr bool isValidDimensions(int width, int height) noexcept {
        return isValidDimension(width) && isValidDimension(height);
    }

    [[nodiscard]] static constexpr bool isValidCoordinate(int x,
                                                          int y,
                                                          int width,
                                                          int height) noexcept {
        return x >= 0 && x < width && y >= 0 && y < height;
    }

    // String utilities
    [[nodiscard]] static std::string toLower(std::string_view str);
    [[nodiscard]] static std::string toUpper(std::string_view str);
    [[nodiscard]] static bool startsWith(std::string_view str, std::string_view prefix);
    [[nodiscard]] static bool endsWith(std::string_view str, std::string_view suffix);

    // File extension utilities
    [[nodiscard]] static std::string getFileExtension(std::string_view filepath);

    // Debug utilities
    [[nodiscard]] static std::string formatBytes(std::size_t bytes);
    [[nodiscard]] static std::string formatDuration(double milliseconds);

    // Platform-specific optimizations detection
    [[nodiscard]] static bool hasSSE2Support() noexcept;
    [[nodiscard]] static bool hasAVX2Support() noexcept;
    [[nodiscard]] static int getOptimalThreadCount() noexcept;

    // Memory alignment utilities for performance
    [[nodiscard]] static void* alignedAlloc(std::size_t size, std::size_t alignment);
    static void alignedFree(void* ptr);

    template <typename T>
    [[nodiscard]] static std::size_t getAlignedSize(std::size_t count, std::size_t alignment = 32) {
        std::size_t bytes = count * sizeof(T);
        return (bytes + alignment - 1) & ~(alignment - 1);
    }

private:
    Core() = delete;

    // Internal state
    static bool s_initialized;
    static std::size_t s_memoryUsage;
    static std::size_t s_peakMemoryUsage;
    static std::chrono::high_resolution_clock::time_point s_initTime;
};

/**
 * @brief RAII wrapper for library initialization
 */
class LibraryGuard {
public:
    LibraryGuard() {
        auto result = Core::initialize();
        if (!result) {
            throw std::runtime_error("Failed to initialize DIPAL library: " +
                                     result.error().toString());
        }
    }

    ~LibraryGuard() { Core::shutdown(); }

    // Non-copyable, non-movable
    LibraryGuard(const LibraryGuard&) = delete;
    LibraryGuard& operator=(const LibraryGuard&) = delete;
    LibraryGuard(LibraryGuard&&) = delete;
    LibraryGuard& operator=(LibraryGuard&&) = delete;
};

/**
 * @brief Simple scoped timer for performance measurement
 */
class ScopedTimer {
public:
    explicit ScopedTimer(std::string_view name = "Timer") : m_name(name), m_start(Core::now()) {}

    ~ScopedTimer() {
        auto elapsed = Core::elapsedMs(m_start);
        // Note: In a real implementation, this would use the logging system
        // For now, we'll just store the result
        m_lastDuration = elapsed;
    }

    [[nodiscard]] double getLastDuration() const noexcept { return m_lastDuration; }

private:
    std::string_view m_name;
    std::chrono::high_resolution_clock::time_point m_start;
    static inline double m_lastDuration = 0.0;
};

}  // namespace DIPAL

#endif  // DIPAL_CORE_HPP
