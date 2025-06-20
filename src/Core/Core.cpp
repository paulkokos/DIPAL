// src/Core/Core.cpp
#include <DIPAL/Core/Core.hpp>

#include <algorithm>
#include <cctype>
#include <chrono>
#include <cstdlib>
#include <format>
#include <iomanip>
#include <sstream>
#include <thread>

#ifdef _WIN32
#include <intrin.h>
#include <malloc.h>
#include <windows.h>
#elif defined(__unix__) || defined(__APPLE__)
#include <unistd.h>

#include <sys/utsname.h>
#ifdef __APPLE__
#include <sys/sysctl.h>
#endif
#endif

namespace DIPAL {

// Static member definitions
bool Core::s_initialized = false;
std::size_t Core::s_memoryUsage = 0;
std::size_t Core::s_peakMemoryUsage = 0;
std::chrono::high_resolution_clock::time_point Core::s_initTime;

// Version implementation
std::string Version::toString() const {
    return std::format("{}.{}.{}", major, minor, patch);
}

// BuildInfo implementation
std::string BuildInfo::toString() const {
    return std::format("Built on {} at {} with {} for {} ({})",
                       buildDate,
                       buildTime,
                       compiler,
                       platform,
                       debugBuild ? "Debug" : "Release");
}

// Core implementation
std::string Core::getVersionString() {
    return getVersion().toString();
}

const BuildInfo& Core::getBuildInfo() {
    static const BuildInfo info = {.buildDate = __DATE__,
                                   .buildTime = __TIME__,
#ifdef __clang__
                                   .compiler = "Clang " __clang_version__,
#elif defined(__GNUC__)
                                   .compiler = "GCC " __VERSION__,
#elif defined(_MSC_VER)
        .compiler = "MSVC " + std::to_string(_MSC_VER),
#else
        .compiler = "Unknown",
#endif
#ifdef _WIN32
                                   .platform = "Windows",
#elif defined(__APPLE__)
                                   .platform = "macOS",
#elif defined(__linux__)
        .platform = "Linux",
#elif defined(__unix__)
        .platform = "Unix",
#else
        .platform = "Unknown",
#endif
#ifdef _DEBUG
                                   .debugBuild = true
#else
                                   .debugBuild = false
#endif
    };
    return info;
}

VoidResult Core::initialize() {
    if (s_initialized) {
        return makeVoidSuccessResult();
    }

    try {
        s_initTime = now();
        s_memoryUsage = 0;
        s_peakMemoryUsage = 0;

        // Initialize any global state here
        // For example: thread pools, memory allocators, etc.

        s_initialized = true;
        return makeVoidSuccessResult();
    } catch (const std::exception& e) {
        return makeVoidErrorResult(ErrorCode::InternalError,
                                   std::format("Library initialization failed: {}", e.what()));
    }
}

void Core::shutdown() {
    if (!s_initialized) {
        return;
    }

    // Cleanup any global state here

    s_initialized = false;
}

bool Core::isInitialized() noexcept {
    return s_initialized;
}

std::size_t Core::getMemoryUsage() {
    return s_memoryUsage;
}

std::size_t Core::getPeakMemoryUsage() {
    return s_peakMemoryUsage;
}

void Core::resetPeakMemoryUsage() {
    s_peakMemoryUsage = s_memoryUsage;
}

std::chrono::high_resolution_clock::time_point Core::now() noexcept {
    return std::chrono::high_resolution_clock::now();
}

double Core::elapsedMs(std::chrono::high_resolution_clock::time_point start) noexcept {
    auto end = now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    return duration.count() / 1000.0;  // Convert to milliseconds
}

std::string Core::toLower(std::string_view str) {
    std::string result;
    result.reserve(str.size());
    std::transform(str.begin(), str.end(), std::back_inserter(result), [](unsigned char c) {
        return std::tolower(c);
    });
    return result;
}

std::string Core::toUpper(std::string_view str) {
    std::string result;
    result.reserve(str.size());
    std::transform(str.begin(), str.end(), std::back_inserter(result), [](unsigned char c) {
        return std::toupper(c);
    });
    return result;
}

bool Core::startsWith(std::string_view str, std::string_view prefix) {
    return str.size() >= prefix.size() && str.substr(0, prefix.size()) == prefix;
}

bool Core::endsWith(std::string_view str, std::string_view suffix) {
    return str.size() >= suffix.size() && str.substr(str.size() - suffix.size()) == suffix;
}

std::string Core::getFileExtension(std::string_view filepath) {
    auto pos = filepath.find_last_of('.');
    if (pos == std::string_view::npos || pos == filepath.size() - 1) {
        return "";
    }
    return toLower(filepath.substr(pos + 1));
}

// Note: guessImageTypeFromExtension is implemented in the header since it uses Image::Type

std::string Core::formatBytes(std::size_t bytes) {
    constexpr std::array<std::string_view, 5> units = {"B", "KB", "MB", "GB", "TB"};

    if (bytes == 0) {
        return "0 B";
    }

    double size = static_cast<double>(bytes);
    std::size_t unitIndex = 0;

    while (size >= 1024.0 && unitIndex < units.size() - 1) {
        size /= 1024.0;
        ++unitIndex;
    }

    std::ostringstream oss;
    oss << std::fixed << std::setprecision(1) << size << " " << units[unitIndex];
    return oss.str();
}

std::string Core::formatDuration(double milliseconds) {
    if (milliseconds < 1.0) {
        return std::format("{:.2f} μs", milliseconds * 1000.0);
    } else if (milliseconds < 1000.0) {
        return std::format("{:.2f} ms", milliseconds);
    } else if (milliseconds < 60000.0) {
        return std::format("{:.2f} s", milliseconds / 1000.0);
    } else {
        double minutes = milliseconds / 60000.0;
        return std::format("{:.2f} min", minutes);
    }
}

bool Core::hasSSE2Support() noexcept {
#if defined(__SSE2__) || (defined(_M_IX86_FP) && _M_IX86_FP >= 2) || defined(_M_X64)
    return true;
#elif defined(_WIN32)
    int cpuInfo[4];
    __cpuid(cpuInfo, 1);
    return (cpuInfo[3] & (1 << 26)) != 0;  // Check SSE2 bit
#elif defined(__GNUC__) || defined(__clang__)
// Check if compiler supports SSE2 intrinsics
#ifdef __SSE2__
    return true;
#else
    return false;
#endif
#else
    return false;
#endif
}

bool Core::hasAVX2Support() noexcept {
#if defined(__AVX2__)
    return true;
#elif defined(_WIN32)
    int cpuInfo[4];
    __cpuid(cpuInfo, 7);
    return (cpuInfo[1] & (1 << 5)) != 0;  // Check AVX2 bit
#elif defined(__GNUC__) || defined(__clang__)
#ifdef __AVX2__
    return true;
#else
    return false;
#endif
#else
    return false;
#endif
}

int Core::getOptimalThreadCount() noexcept {
    unsigned int threadCount = std::thread::hardware_concurrency();

    // Fallback if hardware_concurrency() returns 0
    if (threadCount == 0) {
        threadCount = 4;  // Reasonable default
    }

    // For image processing, using all cores might not be optimal
    // due to memory bandwidth limitations
    if (threadCount > 8) {
        return static_cast<int>(threadCount * 3 / 4);  // Use 75% of cores
    }

    return static_cast<int>(threadCount);
}

void* Core::alignedAlloc(std::size_t size, std::size_t alignment) {
    s_memoryUsage += size;
    s_peakMemoryUsage = std::max(s_peakMemoryUsage, s_memoryUsage);

#ifdef _WIN32
    return _aligned_malloc(size, alignment);
#elif defined(__ANDROID__) || defined(ANDROID)
    // Android doesn't have aligned_alloc before API 28
    void* ptr = nullptr;
    if (posix_memalign(&ptr, alignment, size) != 0) {
        return nullptr;
    }
    return ptr;
#elif defined(__APPLE__) || defined(__unix__)
    void* ptr = nullptr;
    if (posix_memalign(&ptr, alignment, size) != 0) {
        return nullptr;
    }
    return ptr;
#else
    // Fallback to standard malloc with manual alignment
    void* raw = std::malloc(size + alignment + sizeof(void*));
    if (!raw)
        return nullptr;

    // Align the pointer
    void* aligned = reinterpret_cast<void*>(
        (reinterpret_cast<uintptr_t>(raw) + sizeof(void*) + alignment - 1) & ~(alignment - 1));

    // Store the original pointer just before the aligned pointer
    reinterpret_cast<void**>(aligned)[-1] = raw;
    return aligned;
#endif
}

void Core::alignedFree(void* ptr) {
    if (!ptr)
        return;

#ifdef _WIN32
    _aligned_free(ptr);
#elif defined(__ANDROID__) || defined(ANDROID) || defined(__APPLE__) || defined(__unix__)
    std::free(ptr);
#else
    // Fallback: retrieve the original pointer and free it
    void* raw = reinterpret_cast<void**>(ptr)[-1];
    std::free(raw);
#endif
}

}  // namespace DIPAL
