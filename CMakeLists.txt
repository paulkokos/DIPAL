cmake_minimum_required(VERSION 3.20)
project(
  DIPAL
  VERSION 0.1.0
  LANGUAGES CXX)

# Set C++23 as the required standard
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Generate compile_commands.json for LSP support
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Set default build type to Release if not specified
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
endif()

# Option to build examples
option(BUILD_EXAMPLES "Build example programs" ON)

# Enable warnings
if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
elseif(MSVC)
  add_compile_options(/W4 /EHsc)
endif()

# Configure compiler flags based on build type
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")
endif()

# Main library sources - explicitly list core files
set(DIPAL_CORE_SOURCES 
    src/Core/Core.cpp 
    src/Core/Error.cpp
    src/Image/Image.cpp
    src/Image/ImageFactory.cpp
)

# Use glob for other source files (optional modules)
file(GLOB_RECURSE DIPAL_OTHER_SOURCES
    src/Color/*.cpp
    src/Filters/*.cpp
    src/IO/*.cpp
    src/Image/BinaryImage.cpp
    src/Image/ColorImage.cpp
    src/Image/GrayscaleImage.cpp
    src/Image/PixelIterator.cpp
    src/ImageProcessor/*.cpp
    src/Observer/*.cpp
    src/Transformation/*.cpp
    src/Utils/*.cpp
)

# Combine all sources
set(DIPAL_SOURCES ${DIPAL_CORE_SOURCES} ${DIPAL_OTHER_SOURCES})

# Filter out any non-existent files
set(DIPAL_FILTERED_SOURCES)
foreach(SOURCE_FILE ${DIPAL_SOURCES})
    if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/${SOURCE_FILE}")
        list(APPEND DIPAL_FILTERED_SOURCES ${SOURCE_FILE})
    else()
        message(STATUS "Skipping non-existent file: ${SOURCE_FILE}")
    endif()
endforeach()

# Debug: Print source files being built
message(STATUS "Building DIPAL with these source files:")
foreach(SOURCE_FILE ${DIPAL_FILTERED_SOURCES})
  message(STATUS "  ${SOURCE_FILE}")
endforeach()

# Create the library target
add_library(dipal ${DIPAL_FILTERED_SOURCES})

# Set up include directories
target_include_directories(dipal
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

# Add compiler feature flags
target_compile_features(dipal PUBLIC cxx_std_23)

# Enable Link Time Optimization for release builds
if(NOT CMAKE_BUILD_TYPE STREQUAL "Debug")
  set_property(TARGET dipal PROPERTY INTERPROCEDURAL_OPTIMIZATION TRUE)
endif()

# Add examples if enabled
if(BUILD_EXAMPLES)
  if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/examples/CMakeLists.txt")
    add_subdirectory(examples)
  else()
    message(STATUS "Examples directory not found, skipping examples")
  endif()
endif()

# Installation
include(GNUInstallDirs)

install(TARGETS dipal
    EXPORT dipal-targets
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    INCLUDES DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

install(EXPORT dipal-targets
    FILE dipal-targets.cmake
    NAMESPACE DIPAL::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/dipal
)

install(DIRECTORY include/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

# Generate and install package configuration files
include(CMakePackageConfigHelpers)

configure_package_config_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/cmake/dipal-config.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/dipal-config.cmake
    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/dipal
)

write_basic_package_version_file(
    ${CMAKE_CURRENT_BINARY_DIR}/dipal-config-version.cmake
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion
)

install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/dipal-config.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/dipal-config-version.cmake
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/dipal
)