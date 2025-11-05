cmake_minimum_required(VERSION 3.24)

project(DIPAL
        VERSION 0.1.0
        LANGUAGES CXX
        DESCRIPTION "Digital Image Processing and Analysis Library"
)

# ============================================================================
# CMake Policy Settings
# ============================================================================
# Enable newer CMake policies for better diagnostics and features
cmake_policy(SET CMP0074 NEW)  # find_package() uses <PackageName>_ROOT
cmake_policy(SET CMP0115 NEW)  # Source file properties
if(POLICY CMP0135)
    cmake_policy(SET CMP0135 NEW)  # DOWNLOAD_EXTRACT_TIMESTAMP for FetchContent
endif()

# ============================================================================
# C++23 Standard Configuration
# ============================================================================
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_COLOR_DIAGNOSTICS ON)  # Colored compiler output

# ============================================================================
# Build Configuration Options
# ============================================================================
option(BUILD_TESTS "Build tests" ON)
option(BUILD_EXAMPLES "Build examples" ON)
option(BUILD_SHARED_LIBS "Build shared library" ON)
option(ENABLE_SANITIZERS "Enable sanitizers in Debug" OFF)
option(ENABLE_LTO "Enable Link Time Optimization" ON)
option(ENABLE_IPO "Enable Interprocedural Optimization" ON)

# ============================================================================
# Compiler-specific Configuration
# ============================================================================
if(MSVC)
    # MSVC-specific options
    add_compile_options(/W4 /WX /permissive- /Zc:__cplusplus /EHsc)
    add_compile_definitions(_SILENCE_ALL_CXX20_DEPRECATION_WARNINGS NOMINMAX WIN32_LEAN_AND_MEAN)
else()
    # GCC and Clang options
    add_compile_options(-Wall -Wextra -Wpedantic -Werror -fexceptions)

    # Clang-specific optimizations
    if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
        add_compile_options(-fcolor-diagnostics)
    endif()

    # Additional optimizations in Release mode (native only, not for cross-compilation)
    if(CMAKE_BUILD_TYPE MATCHES Release AND NOT CMAKE_CROSSCOMPILING)
        add_compile_options(-march=native)
    endif()
endif()

# Enable Link Time Optimization if requested
if(ENABLE_LTO)
    include(CheckIPOSupported)
    check_ipo_supported(RESULT IPO_SUPPORTED OUTPUT IPO_OUTPUT)
    if(IPO_SUPPORTED)
        set(CMAKE_INTERPROCEDURAL_OPTIMIZATION ON)
    endif()
endif()

# ============================================================================
# Fetch External Dependencies
# ============================================================================
include(FetchContent)

# Google Test - Latest version with modern CMake support
if(BUILD_TESTS)
    message(STATUS "Fetching Google Test v1.15.2...")
    FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG v1.15.2
        FIND_PACKAGE_ARGS QUIET
    )
    FetchContent_MakeAvailable(googletest)
endif()

# Expected library for std::expected (TartanLlama for compatibility)
# Using TartanLlama for all compilers for consistent behavior
message(STATUS "Fetching TartanLlama/expected library for std::expected...")
set(EXPECTED_BUILD_TESTS OFF CACHE BOOL "" FORCE)
FetchContent_Declare(
    expected
    GIT_REPOSITORY https://github.com/TartanLlama/expected.git
    GIT_TAG v1.1.0
)
FetchContent_MakeAvailable(expected)

# ============================================================================
# Source Files Collection
# ============================================================================
file(GLOB_RECURSE DIPAL_SOURCES
        "src/*.cpp"
        "src/**/*.cpp"
)

file(GLOB_RECURSE DIPAL_HEADERS
        "include/*.hpp"
        "include/**/*.hpp"
)

# ============================================================================
# Library Target Definition
# ============================================================================
add_library(dipal ${DIPAL_SOURCES} ${DIPAL_HEADERS})
add_library(DIPAL::dipal ALIAS dipal)

# Position Independent Code for shared libraries
set_target_properties(dipal PROPERTIES
    POSITION_INDEPENDENT_CODE ON
    CXX_STANDARD 23
    CXX_STANDARD_REQUIRED ON
)

# Include directories
target_include_directories(dipal
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<BUILD_INTERFACE:${expected_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

# ============================================================================
# Compiler Features and Options
# ============================================================================
target_compile_features(dipal PUBLIC cxx_std_23)

# Target-specific compiler options
if(MSVC)
    target_compile_options(dipal PRIVATE /wd4996)  # Disable specific deprecated warnings
else()
    # Additional flags for GCC/Clang
    target_compile_options(dipal PRIVATE
        -Wno-unused-parameter
        -Wno-comment
    )
endif()

# ============================================================================
# Sanitizers Configuration
# ============================================================================
if(ENABLE_SANITIZERS AND CMAKE_BUILD_TYPE MATCHES Debug)
    if(NOT MSVC)
        message(STATUS "Enabling AddressSanitizer and UndefinedBehaviorSanitizer...")
        target_compile_options(dipal PRIVATE -fsanitize=address,undefined)
        target_link_options(dipal PRIVATE -fsanitize=address,undefined)
    endif()
endif()

# ============================================================================
# Subdirectories
# ============================================================================
if(BUILD_TESTS)
    message(STATUS "Building tests...")
    enable_testing()
    add_subdirectory(tests)
endif()

if(BUILD_EXAMPLES)
    message(STATUS "Building examples...")
    add_subdirectory(examples)
endif()

# ============================================================================
# Install Configuration
# ============================================================================
include(GNUInstallDirs)

install(TARGETS dipal
    EXPORT dipal-targets
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(DIRECTORY include/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
    FILES_MATCHING PATTERN "*.hpp"
)

# Install CMake config files for find_package() support
install(EXPORT dipal-targets
    FILE dipal-targets.cmake
    NAMESPACE DIPAL::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/dipal
)

# ============================================================================
# Summary
# ============================================================================
message(STATUS "")
message(STATUS "DIPAL Configuration Summary")
message(STATUS "========================================")
message(STATUS "C++ Standard:           C++${CMAKE_CXX_STANDARD}")
message(STATUS "Build Type:             ${CMAKE_BUILD_TYPE}")
message(STATUS "Compiler:               ${CMAKE_CXX_COMPILER_ID} ${CMAKE_CXX_COMPILER_VERSION}")
message(STATUS "Build Tests:            ${BUILD_TESTS}")
message(STATUS "Build Examples:         ${BUILD_EXAMPLES}")
message(STATUS "Build Shared Libs:      ${BUILD_SHARED_LIBS}")
message(STATUS "Sanitizers:             ${ENABLE_SANITIZERS}")
message(STATUS "Link Time Optimization: ${ENABLE_LTO}")
message(STATUS "========================================")
message(STATUS "")










