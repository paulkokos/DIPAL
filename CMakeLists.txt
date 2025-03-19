cmake_minimum_required(VERSION 3.20)
project(DIPAL VERSION 0.1.0 LANGUAGES CXX)

# Set C++23 as the required standard
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

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

# Main library sources
set(DIPAL_SOURCES
    src/Image/Image.cpp
    #src/Image/BinaryImage.cpp
    #src/Image/GrayscaleImage.cpp
    #src/Image/ColorImage.cpp
    #src/Image/ImageFactory.cpp
    #src/Filters/Filters.cpp
    #src/Color/ColorConversions.cpp
    #src/Transformation/Transformations.cpp
    #src/DIPAL/DIPAL.cpp
    #src/Observer/ProcessingObserver.cpp
    #src/ImageProcessor/ImageProcssor.cpp
)

# Main library headers
set(DIPAL_HEADERSs
    include/Image/Image.hpp
    #include/DIPAL/Image/BinaryImage.hpp
    #include/DIPAL/Image/GrayscaleImage.hpp
    #include/DIPAL/Image/ColorImage.hpp
    #include/DIPAL/Image/ImageTypes.hpp
    #include/DIPAL/Image/ImageFactory.hpp
    #include/DIPAL/Filters/Filters.hpp
    #include/DIPAL/Color/ColorConversions.hpp
    #include/DIPAL/Transformation/Transformations.hpp
    #include/DIPAL/Observer/ProcessingObserver.hpp
    #include/DIPAL/ImageProcessor/ImageProcessor.hpp
    #include/DIPAL/DIPAL.hpp
)

# Create the library target
add_library(dipal ${DIPAL_SOURCES} ${DIPAL_HEADERS})

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

# Create demo application
if(BUILD_EXAMPLES)
    # add_executable(dipal_demo examples/demo.cpp)
    # target_link_libraries(dipal_demo PRIVATE dipal)
    # replaced the above
    add_executable (Main main.cpp)
endif()

# Testing
# if(BUILD_TESTS)
#    enable_testing()
    
    # Find Google Test
    #    find_package(GTest REQUIRED)
    
    # Test executables
    # add_executable(dipal_tests
    #    tests/image_tests.cpp
    #    tests/filter_tests.cpp
    #    tests/transformation_tests.cpp
    #    tests/color_tests.cpp
    #)
    
    #target_link_libraries(dipal_tests PRIVATE dipal GTest::GTest GTest::Main)
    
    # Add tests to CTest
    # add_test(NAME DIPAL_Tests COMMAND dipal_tests)
    #sendif()

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
# include(CMakePackageConfigHelpers)

# configure_package_config_file(
#    ${CMAKE_CURRENT_SOURCE_DIR}/cmake/DIPAL-config.cmake.in
#    ${CMAKE_CURRENT_BINARY_DIR}/DIPAL-config.cmake
#    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/DIPAL
#)

# write_basic_package_version_file(
#    ${CMAKE_CURRENT_BINARY_DIR}/DIPAL-config-version.cmake
#    VERSION ${PROJECT_VERSION}
#    COMPATIBILITY SameMajorVersion
# )

# install(FILES
#    ${CMAKE_CURRENT_BINARY_DIR}/DIPAL-config.cmake
#    ${CMAKE_CURRENT_BINARY_DIR}/DIPAL-config-version.cmake
#    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/DIPAL
# ) 

# Export the package for use from the build tree
# export(EXPORT dipal-targets
#    FILE ${CMAKE_CURRENT_BINARY_DIR}/DIPAL-targets.cmake
#    NAMESPACE DIPAL::
# )
