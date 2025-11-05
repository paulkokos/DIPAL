# DIPAL Dependency Upgrade Guide (November 2024)

## Overview
This guide documents the upgrade of DIPAL to use the latest available tools and C++23 features. The project now requires modern compilers and build tools to take full advantage of language features.

## What's Been Updated

### 1. CMake (3.16.3 → 3.28.3+)
- **Minimum Required:** CMake 3.24
- **Benefits:**
  - Better C++23 support
  - Modern FetchContent improvements
  - Generator expressions support
  - Colored diagnostics

### 2. Clang Compiler
- **Recommended:** Clang 18+
- **C++23 Support:** Full support for all C++23 features
- **Key Features:**
  - `std::expected` now available natively
  - Complete ranges support
  - Deduced this for member functions
  - Pattern matching (if statements)

### 3. GCC Compiler
- **Current:** GCC 13.3.0
- **C++23 Support:** Good, but not as complete as Clang
- **Still works:** All project code compatible

### 4. GDB Debugger
- **Current:** GDB 15.0.50 (latest available)
- **Benefits:** Better C++23 debugging support

### 5. Development Tools
- **clang-format-18:** Code formatting with latest standards
- **clang-tidy-18:** Static analysis and modernization suggestions
- **llvm-18-dev:** LLVM development files for optimization passes

## System Requirements

### Minimum
- CMake 3.24+
- C++23 compatible compiler (GCC 13+ or Clang 15+)
- Linux, macOS, or Windows with MSVC 19.34+

### Recommended
- CMake 3.28.3+
- Clang 18+ (best C++23 support)
- GDB 15.0+ (enhanced debugging)
- Ninja build system (faster than make)

## Installation Steps

### Step 1: Update System Packages
```bash
sudo apt update
sudo apt upgrade -y
```

### Step 2: Install Latest Clang
```bash
# Install Clang 18 and related tools
sudo apt install -y \
    clang-18 \
    clang-tools-18 \
    clang-format-18 \
    clang-tidy-18 \
    lldb-18 \
    lld-18 \
    llvm-18-dev

# Set as default
sudo update-alternatives --install /usr/bin/clang clang /usr/bin/clang-18 100
sudo update-alternatives --install /usr/bin/clang++ clang++ /usr/bin/clang++-18 100
sudo update-alternatives --install /usr/bin/clang-format clang-format /usr/bin/clang-format-18 100
sudo update-alternatives --install /usr/bin/clang-tidy clang-tidy /usr/bin/clang-tidy-18 100
```

### Step 3: Install Build Tools
```bash
sudo apt install -y \
    cmake \
    build-essential \
    g++-13 \
    gdb \
    valgrind \
    git \
    ninja-build
```

### Step 4: Verify Installation
```bash
cmake --version          # Should be 3.28.3+
clang-18 --version       # Clang 18
gcc --version            # GCC 13+
gdb --version            # GDB 15+
ninja --version          # Latest
```

## Building DIPAL with New Tools

### Clean Build
```bash
cd /path/to/DIPAL
rm -rf build cmake-build-debug

# Create build directory
mkdir build
cd build

# Configure with CMake 3.24+ and modern settings
cmake -DCMAKE_BUILD_TYPE=Release \
       -DCMAKE_CXX_COMPILER=clang++-18 \
       -DBUILD_TESTS=ON \
       -DBUILD_EXAMPLES=ON \
       -DENABLE_LTO=ON \
       ..

# Build with Ninja (faster) or Make
cmake --build . --config Release --parallel $(nproc)

# Run tests
ctest --output-on-failure
```

### Using Ninja (Recommended)
```bash
cmake -DCMAKE_BUILD_TYPE=Release \
       -DCMAKE_CXX_COMPILER=clang++-18 \
       -GNinja \
       -DBUILD_TESTS=ON \
       -DENABLE_LTO=ON \
       ..

ninja
ninja test
```

## New CMakeLists.txt Features

### 1. Policy Settings
```cmake
cmake_policy(SET CMP0074 NEW)  # find_package() uses <PackageName>_ROOT
cmake_policy(SET CMP0115 NEW)  # Source file properties
cmake_policy(SET CMP0135 NEW)  # DOWNLOAD_EXTRACT_TIMESTAMP
```

### 2. Link Time Optimization
```cmake
option(ENABLE_LTO "Enable Link Time Optimization" ON)
```
This enables interprocedural optimization for faster, smaller binaries.

### 3. Color Diagnostics
```cmake
set(CMAKE_COLOR_DIAGNOSTICS ON)  # Colored compiler output
```

### 4. Modern Compiler Detection
```cmake
# Automatically uses native std::expected on Clang 18+/GCC 13+
# Falls back to TartanLlama/expected for older compilers
```

### 5. Better Install Configuration
Uses `GNUInstallDirs` for platform-aware installation:
- Linux: `/usr/local/lib`, `/usr/local/include`
- macOS: `/usr/local/lib`, `/usr/local/include`
- Windows: Program Files paths

### 6. Build Summary
CMake now prints a configuration summary:
```
DIPAL Configuration Summary
========================================
C++ Standard:           C++23
Build Type:             Release
Compiler:               Clang 18.0.0
Build Tests:            ON
Build Examples:         ON
Build Shared Libs:      ON
Sanitizers:             OFF
Link Time Optimization: ON
========================================
```

## C++23 Features Now Available

### 1. std::expected (Native)
Clang 18+ and GCC 13+ now have `std::expected` in standard library:
```cpp
#include <expected>

Result<Image> load(const std::string& path) {
    // Now using native std::expected!
}
```

### 2. Extended Floating Point Types
```cpp
std::float64_t x = 3.14;  // Now available natively
```

### 3. Deduced this
```cpp
struct Filter {
    void apply(this auto&& self) {
        // Can forward correctly in templates
    }
};
```

### 4. Improved Ranges
```cpp
std::ranges::filter_view filtered =
    image.getData() | std::views::filter(predicate);
```

### 5. Pattern Matching (via if statements)
```cpp
if (auto result = processor.process(image); result) {
    // Use result.value()
} else {
    // Handle error: result.error()
}
```

## Performance Improvements

### 1. Link Time Optimization (LTO)
- Enables interprocedural optimization
- Can result in 10-20% smaller binaries
- Slightly longer build time (worth it for Release builds)

### 2. Native Code Generation
- Release builds use `-march=native` on GCC/Clang
- Optimizes for your specific CPU architecture
- Significant performance gains on x86-64

### 3. Clang 18 Improvements
- Better inlining decisions
- Improved vectorization
- Better memory layout

## Troubleshooting

### Issue: CMake says "minimum version required is 3.24"
**Solution:** Update CMake
```bash
sudo apt install --only-upgrade cmake
```

### Issue: Clang not found
**Solution:** Install and set as default
```bash
sudo apt install -y clang-18
sudo update-alternatives --install /usr/bin/clang clang /usr/bin/clang-18 100
sudo update-alternatives --install /usr/bin/clang++ clang++ /usr/bin/clang++-18 100
```

### Issue: std::expected not found
**Solution:** Use C++23 flag and appropriate compiler
```bash
# Clang 18+
clang++-18 -std=c++23 ...

# GCC 13+
g++-13 -std=c++23 ...
```

### Issue: Build is slow
**Solution:** Use Ninja and parallel building
```bash
cmake -GNinja -DCMAKE_CXX_COMPILER=clang++-18 ..
ninja -j$(nproc)
```

## Migration Notes

### For Users
1. Update CMake to 3.24+
2. Install Clang 18 for best results
3. Rebuild the library
4. Run tests to verify

### For Contributors
1. Use clang-format-18 for formatting
2. Use clang-tidy-18 for analysis
3. Follow C++23 best practices
4. Test with both Clang and GCC

## Future Improvements

### Next Steps
1. ✓ Complete ProgressObserver implementation
2. ✓ Implement PNG/JPEG I/O
3. ✓ Fix Undo functionality
4. Implement SIMD optimizations
5. Add GPU acceleration support
6. Extended image formats (WebP, HEIF)

## References

- [CMake Documentation](https://cmake.org/documentation/)
- [Clang C++23 Support](https://clang.llvm.org/cxx_status.html)
- [GCC C++23 Support](https://gcc.gnu.org/projects/cxx-status.html)
- [C++23 Features](https://en.cppreference.com/)

## Support

For issues or questions:
1. Check this guide's Troubleshooting section
2. Review CMake build output
3. Check compiler version compatibility
4. Report issues on the project GitHub
