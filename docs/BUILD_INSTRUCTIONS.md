# DIPAL Build Instructions

## Quick Build

### Prerequisites
```bash
# Ubuntu/Debian
sudo apt install -y cmake clang-18 build-essential ninja-build

# macOS
brew install cmake llvm ninja
```

### Build Steps
```bash
cd DIPAL
mkdir build && cd build

# With Clang 18 (recommended)
cmake -DCMAKE_CXX_COMPILER=clang++-18 \
       -GNinja \
       -DCMAKE_BUILD_TYPE=Release \
       -DBUILD_TESTS=ON \
       -DENABLE_LTO=ON \
       ..

ninja
ninja test
```

## Build Options

| Option | Default | Description |
|--------|---------|-------------|
| `CMAKE_BUILD_TYPE` | Debug | Debug/Release/RelWithDebInfo/MinSizeRel |
| `CMAKE_CXX_COMPILER` | system | Path to C++ compiler (clang++-18, g++-13, etc) |
| `BUILD_TESTS` | ON | Build test executables |
| `BUILD_EXAMPLES` | ON | Build example programs |
| `BUILD_SHARED_LIBS` | ON | Build shared library (.so/.dll) instead of static |
| `ENABLE_SANITIZERS` | OFF | Enable ASan/UBSan in Debug builds |
| `ENABLE_LTO` | ON | Enable Link Time Optimization |

## Build Scenarios

### Debug Build with Sanitizers
```bash
cmake -DCMAKE_BUILD_TYPE=Debug \
       -DCMAKE_CXX_COMPILER=clang++-18 \
       -DENABLE_SANITIZERS=ON \
       -GNinja \
       ..
ninja
```

### Release Build (Optimized)
```bash
cmake -DCMAKE_BUILD_TYPE=Release \
       -DCMAKE_CXX_COMPILER=clang++-18 \
       -DENABLE_LTO=ON \
       -GNinja \
       ..
ninja
```

### Static Library
```bash
cmake -DBUILD_SHARED_LIBS=OFF \
       -DCMAKE_BUILD_TYPE=Release \
       -GNinja \
       ..
ninja
```

### Tests Only
```bash
cmake -DBUILD_TESTS=ON \
       -DBUILD_EXAMPLES=OFF \
       -GNinja \
       ..
ninja
```

## Running Tests

### All Tests
```bash
ninja test
# or
ctest
```

### Specific Test Category
```bash
ctest -R "image_tests"
ctest -R "filter"
ctest -R "performance"
```

### With Verbose Output
```bash
ctest --output-on-failure
```

### Run Single Test
```bash
./tests/unit/image_tests
./tests/integration/filter_pipeline_integration_tests
```

## Compiler Selection

### Using Clang 18 (Recommended)
```bash
cmake -DCMAKE_CXX_COMPILER=clang++-18 ..
```

### Using GCC 13
```bash
cmake -DCMAKE_CXX_COMPILER=g++-13 ..
```

### Using MSVC (Windows)
```bash
cmake -G "Visual Studio 17 2022" ..
cmake --build . --config Release
```

## Installation

### System-wide Install
```bash
cmake --build . --target install
# or with sudo for system directories
sudo cmake --build . --target install
```

### Custom Install Location
```bash
cmake -DCMAKE_INSTALL_PREFIX=/custom/path ..
cmake --build . --target install
```

## Cleaning Build

### Remove Build Directory
```bash
rm -rf build
```

### In-source Clean (if using CMake)
```bash
cmake --build . --target clean
```

## Performance Tips

1. **Use Ninja instead of Make**
   - Faster builds, better parallelization
   ```bash
   cmake -GNinja ..
   ```

2. **Use Clang 18+**
   - Best C++23 support
   - Better optimization

3. **Enable LTO for Release**
   ```bash
   -DENABLE_LTO=ON
   ```

4. **Parallel Building**
   ```bash
   ninja -j$(nproc)
   ctest -j$(nproc)
   ```

5. **Skip Tests/Examples if Not Needed**
   ```bash
   -DBUILD_TESTS=OFF -DBUILD_EXAMPLES=OFF
   ```

## Troubleshooting

### CMake not found
```bash
sudo apt install cmake
```

### Clang 18 not found
```bash
sudo apt install clang-18
sudo update-alternatives --install /usr/bin/clang++ clang++ /usr/bin/clang++-18 100
```

### Ninja not found
```bash
sudo apt install ninja-build
```

### C++23 features not available
- Use Clang 18+ or GCC 13+
- Check: `cmake --version` and `clang++ --version`

### Build fails with "expected" not found
- This is expected for older compilers, library will fetch it
- Use Clang 18+ to use native std::expected

### Out of Memory During LTO
- Disable LTO: `-DENABLE_LTO=OFF`
- Use Debug build: `-DCMAKE_BUILD_TYPE=Debug`

## Using with IDEs

### CLion / Visual Studio Code + CMake Tools
```bash
# Select as generator
cmake -GNinja ..
# IDE will auto-detect and configure
```

### Visual Studio (Windows)
```bash
cmake -G "Visual Studio 17 2022" ..
# Open DIPAL.sln in Visual Studio
```

### Qt Creator
```bash
# Open CMakeLists.txt directly
# Kit will auto-detect compiler
```

## Generating Documentation

### Doxygen (if installed)
```bash
doxygen Doxyfile  # if it exists
# or create one for the project
```

## Continuous Integration

### GitHub Actions / CI Servers
```bash
#!/bin/bash
mkdir build && cd build
cmake -DCMAKE_CXX_COMPILER=clang++-18 \
       -GNinja \
       -DCMAKE_BUILD_TYPE=Release \
       -DBUILD_TESTS=ON \
       ..
ninja
ninja test
```

## Next Steps

1. Read [UPGRADE_GUIDE.md](./UPGRADE_GUIDE.md) for detailed upgrade info
2. Check project README.md for library usage
3. Review [examples/](../examples/) for usage examples
4. Run tests to verify your installation
