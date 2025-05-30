cmake_minimum_required(VERSION 3.20)
project(DIPAL VERSION 0.1.0 LANGUAGES CXX)

# Set C++23 as the required standard
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Generate compile_commands.json for LSP support
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Define build types
set(CMAKE_CONFIGURATION_TYPES "Debug;Release;RelWithDebInfo;MinSizeRel")

# Set default build type to Release if not specified
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
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
if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic -Werror)
elseif(MSVC)
  add_compile_options(/W4 /WX)
endif()

# Option to build shared or static library
option(BUILD_SHARED_LIBS "Build shared libraries" ON)

# Option to build tests
option(BUILD_TESTS "Build test programs" ON)

# Option to build examples
option(BUILD_EXAMPLES "Build example programs" ON)

# Option to enable sanitizers in debug mode
option(ENABLE_SANITIZERS "Enable Address and UB sanitizers in Debug mode" OFF)

# Configure compiler flags for sanitizers
if(ENABLE_SANITIZERS AND CMAKE_BUILD_TYPE MATCHES "Debug")
  if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    add_compile_options(-fsanitize=address -fsanitize=undefined)
    add_link_options(-fsanitize=address -fsanitize=undefined)
  endif()
endif()

# Add CMake modules path
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake/modules")

# Main library sources (update as you add more files)
file(GLOB_RECURSE DIPAL_SOURCES 
     "src/*.cpp"
)

# Create the library target
add_library(dipal ${DIPAL_SOURCES})

# Enable Link Time Optimization for release builds
if(NOT CMAKE_BUILD_TYPE STREQUAL "Debug")
  set_property(TARGET dipal PROPERTY INTERPROCEDURAL_OPTIMIZATION TRUE)
endif()

# Set up include directories
target_include_directories(dipal
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

# Add examples if enabled
if(BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()

# Add tests if enabled
if(BUILD_TESTS)
    # Now proceed with tests as usual
    enable_testing()
    add_subdirectory(tests)
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










