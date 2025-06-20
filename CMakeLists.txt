cmake_minimum_required(VERSION 3.20)
project(
  DIPAL
  VERSION 0.1.0
  LANGUAGES CXX)

# Set C++20 as the required standard set(CMAKE_CXX_STANDARD 20) Set C++23 as the
# required standard
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Generate compile_commands.json for LSP support
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Define build types
set(CMAKE_CONFIGURATION_TYPES "Debug;Release;RelWithDebInfo;MinSizeRel")

# Add fmt library for format support
include(FetchContent)
FetchContent_Declare(
  fmt
  GIT_REPOSITORY https://github.com/fmtlib/fmt.git
  GIT_TAG 9.1.0 # Use a specific version
)
FetchContent_MakeAvailable(fmt)

# Option to build tests
option(BUILD_TESTS "Build test programs" ON)

# Option to build shared or static library
option(BUILD_SHARED_LIBS "Build shared libraries" ON)

# Option to build examples
option(BUILD_EXAMPLES "Build example programs" ON)

# Option to build documentation
option(BUILD_DOCUMENTATION "Build documentation" OFF)

# Option to enable sanitizers in debug mode
option(ENABLE_SANITIZERS "Enable Address and UB sanitizers in Debug mode" OFF)

# Options for image format support
option(DIPAL_ENABLE_JPEG "Enable JPEG image format support" ON)
option(DIPAL_ENABLE_PNG "Enable PNG image format support" OFF)
option(DIPAL_ENABLE_TIFF "Enable TIFF image format support" OFF)

# Handle JPEG support
if(DIPAL_ENABLE_JPEG)
  find_package(JPEG)

  if(JPEG_FOUND)
    message(STATUS "Found system libjpeg: ${JPEG_LIBRARIES}")
    add_definitions(-DDIPAL_HAS_JPEG)
  else()
    message(STATUS "System libjpeg not found, downloading via FetchContent")
    FetchContent_Declare(
      libjpeg
      URL https://ijg.org/files/jpegsrc.v9e.tar.gz
      URL_HASH
        SHA256=4077d6a6a75aeb01884f708919d25934c93305e49f7e3f36db9129320e6f4f3d)
    FetchContent_MakeAvailable(libjpeg)
    add_definitions(-DDIPAL_HAS_JPEG)
  endif()
endif()

# Handle PNG support
if(DIPAL_ENABLE_PNG)
  find_package(PNG)

  if(PNG_FOUND)
    message(STATUS "Found system libpng: ${PNG_LIBRARIES}")
    add_definitions(-DDIPAL_HAS_PNG)
  else()
    message(STATUS "PNG support requested but libpng not found")
    message(STATUS "PNG support will be disabled")
    set(DIPAL_ENABLE_PNG OFF)
  endif()
endif()

# Handle TIFF support
if(DIPAL_ENABLE_TIFF)
  find_package(TIFF)

  if(TIFF_FOUND)
    message(STATUS "Found system libtiff: ${TIFF_LIBRARIES}")
    add_definitions(-DDIPAL_HAS_TIFF)
  else()
    message(STATUS "TIFF support requested but libtiff not found")
    message(STATUS "TIFF support will be disabled")
    set(DIPAL_ENABLE_TIFF OFF)
  endif()
endif()

# Main library sources - explicitly list core files to ensure they're included
set(DIPAL_CORE_SOURCES src/Image/Image.cpp src/Core/Core.cpp src/Core/Error.cpp)

# Use glob for the rest of the files
file(
  GLOB_RECURSE
  DIPAL_OTHER_SOURCES
  src/Color/*.cpp
  src/Filters/*.cpp
  src/IO/*.cpp
  src/Image/BinaryImage.cpp
  src/Image/ColorImage.cpp
  src/Image/GrayscaleImage.cpp
  src/Image/ImageFactory.cpp
  src/Image/PixelIterator.cpp
  src/ImageProcessor/*.cpp
  src/Observer/*.cpp
  src/Transformation/*.cpp
  src/Utils/*.cpp)

# Combine the core sources with the other sources
set(DIPAL_SOURCES ${DIPAL_CORE_SOURCES} ${DIPAL_OTHER_SOURCES})

# For debugging - print out all source files
message(STATUS "Building DIPAL with these source files:")
foreach(SOURCE_FILE ${DIPAL_SOURCES})
  message(STATUS "  ${SOURCE_FILE}")
endforeach()

# Create the library target
add_library(dipal ${DIPAL_SOURCES})

# Link libraries to the dipal target
target_link_libraries(dipal PRIVATE fmt::fmt)

# Add image format libraries if enabled
if(DIPAL_ENABLE_JPEG)
  if(JPEG_FOUND)
    target_link_libraries(dipal PRIVATE ${JPEG_LIBRARIES})
    target_include_directories(dipal PRIVATE ${JPEG_INCLUDE_DIR})
  else()
    target_link_libraries(dipal PRIVATE jpeg)
  endif()
endif()

if(DIPAL_ENABLE_PNG AND PNG_FOUND)
  target_link_libraries(dipal PRIVATE ${PNG_LIBRARIES})
  target_include_directories(dipal PRIVATE ${PNG_INCLUDE_DIRS})
endif()

if(DIPAL_ENABLE_TIFF AND TIFF_FOUND)
  target_link_libraries(dipal PRIVATE ${TIFF_LIBRARIES})
  target_include_directories(dipal PRIVATE ${TIFF_INCLUDE_DIR})
endif()

# Add Google Test if tests are enabled
if(BUILD_TESTS)
  # First check if the googletest directory exists as a submodule
  if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/third_party/googletest/CMakeLists.txt")
    # Use the existing submodule
    message(STATUS "Using googletest submodule in third_party/googletest")
    set(BUILD_GMOCK
        ON
        CACHE BOOL "Build gmock")
    set(INSTALL_GTEST
        OFF
        CACHE BOOL "Disable gtest installation")
    set(gtest_force_shared_crt
        ON
        CACHE BOOL "Use shared CRT" FORCE)
    add_subdirectory(third_party/googletest)
  else()
    # Try to find the installed GTest
    find_package(GTest QUIET)

    if(NOT GTEST_FOUND)
      message(
        STATUS
          "Googletest submodule not found and GTest not installed. Downloading from source..."
      )

      # Include the FetchContent module
      include(FetchContent)

      # Declare the GTest dependency
      FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG v1.14.0 # Use a specific version
      )

      # Configure GTest build options
      set(BUILD_GMOCK
          ON
          CACHE BOOL "Build gmock" FORCE)
      set(INSTALL_GTEST
          OFF
          CACHE BOOL "Disable gtest installation" FORCE)

      # Prevent overriding the parent project's compiler/linker settings on
      # Windows
      set(gtest_force_shared_crt
          ON
          CACHE BOOL "Use shared CRT" FORCE)

      # Make the dependency available
      FetchContent_MakeAvailable(googletest)

      # Set variables to mimic what find_package would set
      set(GTEST_INCLUDE_DIRS "${googletest_SOURCE_DIR}/googletest/include")
      set(GTEST_LIBRARIES gtest)
      set(GTEST_MAIN_LIBRARIES gtest_main)
    endif()
  endif()

  # Now proceed with tests
  enable_testing()
  add_subdirectory(tests)
endif()

# Set default build type to Release if not specified
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE
      Release
      CACHE STRING "Build type" FORCE)
endif()

# Configure compiler options based on buildtype
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3")
elseif(CMAKE_BUILD_TYPE STREQUAL "RelWithDebInfo")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2 -g")
elseif(CMAKE_BUILD_TYPE STREQUAL "MinSizeRel")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Os")
endif()

# Enable warnings
if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID STREQUAL
                                           "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic -Werror)
elseif(MSVC)
  add_compile_options(/W4 /WX /EHsc)
  # Disable specific MSVC warnings
  add_compile_options(/wd4251) # needs to have dll-interface to be used by
  # clients of class
endif()

# Configure compiler flags for sanitizers
if(ENABLE_SANITIZERS AND CMAKE_BUILD_TYPE MATCHES "Debug")
  if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID STREQUAL
                                             "Clang")
    add_compile_options(-fsanitize=address -fsanitize=undefined)
    add_link_options(-fsanitize=address -fsanitize=undefined)
  endif()
endif()

# Add CMake modules path
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules")

# Enable Link Time Optimization for release builds
if(NOT CMAKE_BUILD_TYPE STREQUAL "Debug")
  set_property(TARGET dipal PROPERTY INTERPROCEDURAL_OPTIMIZATION TRUE)
endif()

# Set up include directories
target_include_directories(
  dipal
  PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
         $<INSTALL_INTERFACE:include>
  PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)

# Add compiler feature flags
target_compile_features(dipal PUBLIC cxx_std_20)

# Add examples if enabled
if(BUILD_EXAMPLES)
  add_subdirectory(examples)
endif()

# Documentation
if(BUILD_DOCUMENTATION)
  find_package(Doxygen)
  if(DOXYGEN_FOUND)
    set(DOXYGEN_IN ${CMAKE_CURRENT_SOURCE_DIR}/docs/Doxyfile.in)
    set(DOXYGEN_OUT ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)

    configure_file(${DOXYGEN_IN} ${DOXYGEN_OUT} @ONLY)

    add_custom_target(
      doc
      COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_OUT}
      WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
      COMMENT "Generating API documentation with Doxygen"
      VERBATIM)
  else()
    message(STATUS "Doxygen not found, documentation will not be built")
  endif()
endif()

# Installation
include(GNUInstallDirs)

install(
  TARGETS dipal fmt
  EXPORT dipal-targets
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
  INCLUDES
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

install(
  EXPORT dipal-targets
  FILE dipal-targets.cmake
  NAMESPACE DIPAL::
  DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/dipal)

install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

# Generate and install package configuration files
include(CMakePackageConfigHelpers)

configure_package_config_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/cmake/dipal-config.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/dipal-config.cmake
  INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/dipal)

write_basic_package_version_file(
  ${CMAKE_CURRENT_BINARY_DIR}/dipal-config-version.cmake
  VERSION ${PROJECT_VERSION}
  COMPATIBILITY SameMajorVersion)

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/dipal-config.cmake
              ${CMAKE_CURRENT_BINARY_DIR}/dipal-config-version.cmake
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/dipal)
