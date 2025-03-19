#!/bin/bash

# Script to create the DIPAL project structure for AstroNvim users
# Usage: ./create_dipal_structure_astro.sh [target_directory]

set -e

TARGET_DIR="${1:-.}"  # Default to current directory if no argument provided

# Create main directory structure
echo "Creating DIPAL project structure in $TARGET_DIR with AstroNvim configuration..."

# GitHub related directories
mkdir -p "$TARGET_DIR/.github/ISSUE_TEMPLATE"
# Keep existing GitHub templates
if [ -d "$TARGET_DIR/.github/ISSUE_TEMPLATE" ]; then
  echo "GitHub templates already exist. Preserving them."
else
  # Copy from existing templates if available
  cp -n .github/ISSUE_TEMPLATE/* "$TARGET_DIR/.github/ISSUE_TEMPLATE/" 2>/dev/null || echo "No templates to copy."
fi

# Neovim/AstroNvim configuration
mkdir -p "$TARGET_DIR/.config/nvim"
cat > "$TARGET_DIR/.config/nvim/init.lua" << 'EOF'
-- AstroNvim config for DIPAL project
-- This file can be symlinked to ~/.config/nvim/init.lua or used with NVIM_APPNAME
-- Example: NVIM_APPNAME=dipal nvim .

-- You can use this to configure AstroNvim specifically for this C++ project
-- if you want to use project-specific settings

-- Load base AstroNvim configuration
local astro_path = vim.fn.stdpath("config") .. "/lua/astro"
if vim.fn.filereadable(astro_path) == 1 then
  dofile(astro_path)
else
  -- If not running a local instance, assume normal AstroNvim config is available
  -- This allows you to use this file with NVIM_APPNAME=dipal nvim
  -- or just reference it in your normal AstroNvim config
end

-- Project-specific settings
local config = {
  -- Configure C++ specific options
  lsp = {
    -- Setup C++ lsp servers
    servers = {
      "clangd",
      "cmake",
    },
    -- Configure clangd
    config = {
      clangd = {
        cmd = {
          "clangd",
          "--background-index",
          "--suggest-missing-includes",
          "--clang-tidy",
          "--header-insertion=iwyu",
        },
      },
    },
  },
  -- Configure diagnostics
  diagnostics = {
    underline = true,
    update_in_insert = false,
    virtual_text = { spacing = 4, prefix = "●" },
    severity_sort = true,
  },
  -- Configure formatting
  formatting = {
    format_on_save = {
      enabled = true,
      allow_filetypes = { "cpp", "hpp", "cmake" },
    },
    disabled = {},
    timeout_ms = 1000,
  },
  -- Configure treesitter
  treesitter = {
    ensure_installed = {
      "c",
      "cpp",
      "cmake",
      "lua",
      "markdown",
      "markdown_inline",
    },
  },
}

-- Return the configuration if being imported or apply if run directly
return config
EOF

# Create .clangd configuration for AstroNvim LSP
cat > "$TARGET_DIR/.clangd" << 'EOF'
CompileFlags:
  Add: [-std=c++23, -Wall, -Wextra, -Wpedantic]
  
Diagnostics:
  UnusedIncludes: Strict
  
InlayHints:
  Enabled: Yes
  ParameterNames: Yes
  DeducedTypes: Yes
  
Hover:
  ShowAKA: Yes

Index:
  Background: Build
EOF

# CMake directories
mkdir -p "$TARGET_DIR/cmake/modules"
cat > "$TARGET_DIR/cmake/modules/FindDIPAL.cmake" << 'EOF'
# FindDIPAL.cmake - Find DIPAL library
# This module defines:
#  DIPAL_FOUND - System has DIPAL
#  DIPAL_INCLUDE_DIRS - DIPAL include directories
#  DIPAL_LIBRARIES - Libraries needed to use DIPAL

# Placeholder for actual find module logic
EOF

# Documentation directories
mkdir -p "$TARGET_DIR/docs/api"
mkdir -p "$TARGET_DIR/docs/examples"
cat > "$TARGET_DIR/docs/README.md" << 'EOF'
# DIPAL Documentation

This directory contains documentation for the DIPAL (Digital Image Processing and Analysis Library).

## Structure

- `api/`: API documentation generated from code
- `examples/`: Example usage and tutorials

## Building Documentation

To generate API documentation, run:

```
# TODO: Add documentation generation command
```
EOF

# Examples directories
mkdir -p "$TARGET_DIR/examples/basic"
mkdir -p "$TARGET_DIR/examples/advanced"
cat > "$TARGET_DIR/examples/basic/hello_dipal.cpp" << 'EOF'
#include <DIPAL/DIPAL.hpp>
#include <iostream>

int main() {
    // Basic example of using DIPAL
    std::cout << "Hello from DIPAL!" << std::endl;
    
    // TODO: Add basic image loading and processing example
    
    return 0;
}
EOF

cat > "$TARGET_DIR/examples/CMakeLists.txt" << 'EOF'
cmake_minimum_required(VERSION 3.20)

# Basic examples
add_executable(hello_dipal basic/hello_dipal.cpp)
target_link_libraries(hello_dipal PRIVATE dipal)

# TODO: Add more example executables
EOF

# Include directories (public API)
mkdir -p "$TARGET_DIR/include/DIPAL/Core"
mkdir -p "$TARGET_DIR/include/DIPAL/Image"
mkdir -p "$TARGET_DIR/include/DIPAL/Filters"
mkdir -p "$TARGET_DIR/include/DIPAL/Transformation"
mkdir -p "$TARGET_DIR/include/DIPAL/Color"
mkdir -p "$TARGET_DIR/include/DIPAL/IO"
mkdir -p "$TARGET_DIR/include/DIPAL/Utils"

# Create main header file
cat > "$TARGET_DIR/include/DIPAL/DIPAL.hpp" << 'EOF'
#ifndef DIPAL_HPP
#define DIPAL_HPP

// Main include file for DIPAL library
// Include all public API headers here

#include "Core/Core.hpp"
#include "Image/Image.hpp"
#include "Filters/Filters.hpp"
#include "Transformation/Transformations.hpp"
#include "Color/ColorConversions.hpp"
#include "IO/ImageIO.hpp"
#include "Utils/Utils.hpp"

// Version information
#define DIPAL_VERSION_MAJOR 0
#define DIPAL_VERSION_MINOR 1
#define DIPAL_VERSION_PATCH 0

namespace DIPAL {
    // Core library class
    class DIPAL {
    public:
        DIPAL();
        ~DIPAL();
        
        // Add main library functionality here
        
    private:
        // Implementation details
    };
}

#endif // DIPAL_HPP
EOF

# Create some basic header files
cat > "$TARGET_DIR/include/DIPAL/Core/Core.hpp" << 'EOF'
#ifndef DIPAL_CORE_HPP
#define DIPAL_CORE_HPP

namespace DIPAL {
    // Core definitions and base classes
}

#endif // DIPAL_CORE_HPP
EOF

cat > "$TARGET_DIR/include/DIPAL/Image/Image.hpp" << 'EOF'
#ifndef DIPAL_IMAGE_HPP
#define DIPAL_IMAGE_HPP

#include <memory>
#include <string>
#include <vector>

namespace DIPAL {
    // Base image class
    class Image {
    public:
        virtual ~Image() = default;
        
        // Common image operations
        
    protected:
        // Base image data
    };
}

#endif // DIPAL_IMAGE_HPP
EOF

# Source implementation directories
mkdir -p "$TARGET_DIR/src/Core"
mkdir -p "$TARGET_DIR/src/Image"
mkdir -p "$TARGET_DIR/src/Filters"
mkdir -p "$TARGET_DIR/src/Transformation"
mkdir -p "$TARGET_DIR/src/Color"
mkdir -p "$TARGET_DIR/src/IO"
mkdir -p "$TARGET_DIR/src/Utils"

# Create basic implementation files
cat > "$TARGET_DIR/src/Image/Image.cpp" << 'EOF'
#include "../../include/DIPAL/Image/Image.hpp"

namespace DIPAL {
    // Image class implementation
}
EOF

# Testing directories
mkdir -p "$TARGET_DIR/tests/unit"
mkdir -p "$TARGET_DIR/tests/integration"
mkdir -p "$TARGET_DIR/tests/performance"
mkdir -p "$TARGET_DIR/tests/fixtures"

cat > "$TARGET_DIR/tests/CMakeLists.txt" << 'EOF'
cmake_minimum_required(VERSION 3.20)

# Find GTest
find_package(GTest REQUIRED)
include(GoogleTest)

# Unit tests
add_executable(image_tests unit/image_tests.cpp)
target_link_libraries(image_tests PRIVATE dipal GTest::GTest GTest::Main)
gtest_discover_tests(image_tests)

# TODO: Add more test executables
EOF

cat > "$TARGET_DIR/tests/unit/image_tests.cpp" << 'EOF'
#include <gtest/gtest.h>
#include <DIPAL/Image/Image.hpp>

// Test fixture for Image tests
class ImageTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Setup code
    }
    
    void TearDown() override {
        // Cleanup code
    }
};

// Basic test case
TEST_F(ImageTest, CreateImage) {
    // TODO: Implement test
    EXPECT_TRUE(true);
}
EOF

# Third-party dependencies
mkdir -p "$TARGET_DIR/third_party"
cat > "$TARGET_DIR/third_party/README.md" << 'EOF'
# Third-party Dependencies

This directory is for third-party libraries and dependencies used by DIPAL.

## Current Dependencies

None at this time.

## Adding a Dependency

Prefer using Git submodules or CMake FetchContent for managing dependencies.
EOF

# Tools and utilities
mkdir -p "$TARGET_DIR/tools/scripts"
mkdir -p "$TARGET_DIR/tools/ci"

cat > "$TARGET_DIR/tools/scripts/run_clang_format.sh" << 'EOF'
#!/bin/bash
# Apply clang-format to all source files

find $PWD/include $PWD/src -name "*.hpp" -o -name "*.cpp" | xargs clang-format -i
EOF
chmod +x "$TARGET_DIR/tools/scripts/run_clang_format.sh"

# Configuration files
cat > "$TARGET_DIR/.clang-format" << 'EOF'
---
Language: Cpp
BasedOnStyle: Google
AccessModifierOffset: -4
AlignAfterOpenBracket: Align
AlignConsecutiveAssignments: false
AlignConsecutiveDeclarations: false
AlignEscapedNewlines: Left
AlignOperands: true
AlignTrailingComments: true
AllowAllParametersOfDeclarationOnNextLine: false
AllowShortBlocksOnASingleLine: false
AllowShortCaseLabelsOnASingleLine: false
AllowShortFunctionsOnASingleLine: Inline
AllowShortIfStatementsOnASingleLine: false
AllowShortLoopsOnASingleLine: false
AlwaysBreakAfterReturnType: None
AlwaysBreakBeforeMultilineStrings: false
AlwaysBreakTemplateDeclarations: Yes
BinPackArguments: false
BinPackParameters: false
BreakBeforeBinaryOperators: None
BreakBeforeBraces: Attach
ColumnLimit: 100
IndentWidth: 4
TabWidth: 4
UseTab: Never
---
EOF

cat > "$TARGET_DIR/.clang-tidy" << 'EOF'
---
Checks: 'clang-diagnostic-*,clang-analyzer-*,cppcoreguidelines-*,modernize-*,bugprone-*,performance-*,readability-*,-modernize-use-trailing-return-type,-readability-magic-numbers'
WarningsAsErrors: ''
HeaderFilterRegex: '.*'
AnalyzeTemporaryDtors: false
FormatStyle: none
CheckOptions:
  - key: readability-identifier-naming.ClassCase
    value: CamelCase
  - key: readability-identifier-naming.MethodCase
    value: camelBack
  - key: readability-identifier-naming.PrivateMemberPrefix
    value: m_
---
EOF

# Create AstroNvim specific project settings
mkdir -p "$TARGET_DIR/.nvim"
cat > "$TARGET_DIR/.nvim/project.lua" << 'EOF'
-- AstroNvim project-specific configuration
return {
  -- lsp configs
  lsp = {
    -- formatter
    formatting = {
      -- control auto formatting on save
      format_on_save = {
        enabled = true,
        allow_filetypes = {
          "cpp",
          "hpp",
          "c",
          "h",
          "cmake",
        },
      },
    },
  },
  -- diagnostics configuration
  diagnostics = {
    underline = true,
    virtual_text = true,
  },
}
EOF

# Create compilation database config
cat > "$TARGET_DIR/compile_commands.json.template" << 'EOF'
[
  {
    "directory": "BUILD_DIR_PATH",
    "command": "c++ -std=c++23 -Iinclude -c -o src/example.o src/example.cpp",
    "file": "src/example.cpp"
  }
]
EOF

cat > "$TARGET_DIR/tools/scripts/update_compile_commands.sh" << 'EOF'
#!/bin/bash
# Generate compile_commands.json for LSP

BUILD_DIR=${1:-build}

if [ ! -d "$BUILD_DIR" ]; then
  mkdir -p "$BUILD_DIR"
  cd "$BUILD_DIR"
  cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=ON ..
else
  cd "$BUILD_DIR"
  cmake -DCMAKE_EXPORT_COMPILE_COMMANDS=ON ..
fi

# Link compilation database to project root for LSP
cd ..
ln -sf "$BUILD_DIR/compile_commands.json" .
EOF
chmod +x "$TARGET_DIR/tools/scripts/update_compile_commands.sh"

# Copy existing files if they exist
if [ -f "LICENSE" ]; then
  cp LICENSE "$TARGET_DIR/"
else
  # Create a placeholder BSD license (similar to your current one)
  cat > "$TARGET_DIR/LICENSE" << 'EOF'
BSD 3-Clause License

Copyright (c) 2025, DIPAL Contributors
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this
   list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its
   contributors may be used to endorse or promote products derived from
   this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
EOF
fi

# Update or create README.md
if [ -f "README.md" ]; then
  cp README.md "$TARGET_DIR/"
else
  cat > "$TARGET_DIR/README.md" << 'EOF'
# DIPAL

Digital Image Processing and Analysis Library

## Description

A lightweight, simple, easy to use/understand, C++ image processing and analysis library. 
Built with modern C++ (C++23) techniques and designed to be independent of external libraries.

## Features

- Image loading and saving
- Image filtering and transformations
- Color space conversions
- Modern C++ design

## Installation

### Prerequisites

- C++23 compatible compiler (GCC 11+, Clang 14+, MSVC 19.30+)
- CMake 3.20 or higher

### Building from Source

```bash
git clone https://github.com/yourusername/DIPAL.git
cd DIPAL
mkdir build && cd build
cmake ..
cmake --build .
```

## Development with AstroNvim

This project includes configuration for AstroNvim:

1. Run the LSP setup script to generate compile_commands.json:
   ```bash
   ./tools/scripts/update_compile_commands.sh
   ```

2. Open the project in Neovim:
   ```bash
   nvim .
   ```

## Usage

```cpp
#include <DIPAL/DIPAL.hpp>

int main() {
    // TODO: Add usage example
    return 0;
}
```

## Documentation

See the [documentation](docs/README.md) for detailed API reference and examples.

## Contributing

Please read [CONTRIBUTING.md](CONTRIBUTING.md) for details on our code of conduct and the process for submitting pull requests.

## License

This project is licensed under the BSD 3-Clause License - see the [LICENSE](LICENSE) file for details.
EOF
fi

# Create CONTRIBUTING.md
cat > "$TARGET_DIR/CONTRIBUTING.md" << 'EOF'
# Contributing to DIPAL

Thank you for your interest in contributing to DIPAL! This document provides guidelines and instructions for contributing.

## Code of Conduct

Please read and follow our [Code of Conduct](CODE_OF_CONDUCT.md).

## How to Contribute

1. Fork the repository
2. Create a feature branch (`git checkout -b feature/amazing-feature`)
3. Commit your changes (`git commit -m 'Add some amazing feature'`)
4. Push to the branch (`git push origin feature/amazing-feature`)
5. Open a Pull Request

## Pull Request Process

1. Ensure your code follows the project's coding style (run clang-format)
2. Update documentation if necessary
3. Add tests for new functionality
4. Make sure all tests pass
5. Wait for review and address any feedback

## Coding Standards

- Follow the Google C++ Style Guide with modifications as defined in .clang-format
- Use C++23 features appropriately
- Write clear, meaningful comments
- Include unit tests for all new functionality

## Development Setup

See the README.md for instructions on setting up the development environment.
EOF

# Create main CMakeLists.txt
cat > "$TARGET_DIR/CMakeLists.txt" << 'EOF'
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
EOF

# Create CMake config template
mkdir -p "$TARGET_DIR/cmake"
cat > "$TARGET_DIR/cmake/dipal-config.cmake.in" << 'EOF'
@PACKAGE_INIT@

include("${CMAKE_CURRENT_LIST_DIR}/dipal-targets.cmake")
check_required_components(dipal)
EOF

echo "DIPAL project structure created successfully with AstroNvim configuration!"
