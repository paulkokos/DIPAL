
# DIPAL

<p align="center">
  <img src="docs/img/logo.png" alt="DIPAL Logo" width="200"/>
</p>

**Digital Image Processing and Analysis Library**

[![License](https://img.shields.io/badge/License-BSD_3--Clause-blue.svg)](https://opensource.org/licenses/BSD-3-Clause)
[![C++23](https://img.shields.io/badge/C%2B%2B-23-blue.svg)](https://isocpp.org/)

A lightweight C++23 image processing and analysis library built with modern techniques. DIPAL provides high performance and ease of use with minimal external dependencies.

## Features

### Core Features

- **Modern C++23 Design**: Uses `std::expected` (via [tl::expected](https://github.com/TartanLlama/expected)), `std::span`, `std::format`, concepts, and `[[nodiscard]]`
- **Minimal Dependencies**: Only runtime dependency is [TartanLlama/expected](https://github.com/TartanLlama/expected) v1.1.0 (fetched automatically via CMake)
- **Cross-Platform**: Works on Windows, Linux, and macOS
- **Thread-Safe**: Built-in thread pool with parallel processing capabilities

### Image Processing

- **Image Types**: Binary, grayscale, RGB, and RGBA image representations
- **Filter Operations**:
  - Gaussian blur with configurable sigma and kernel size
  - Median filtering for noise reduction
  - Sobel edge detection with optional normalization
  - Unsharp mask sharpening with configurable amount, radius, and threshold
- **Geometric Transformations**:
  - Image resizing with multiple interpolation methods (Nearest Neighbor, Bilinear, Bicubic)
  - Rotation, affine, warp, and general geometric transformations
- **Color Operations**:
  - RGB to HSV conversion and back
  - Color channel manipulation
  - Grayscale conversion with proper luminance weights

### Architecture & Patterns

- **Extensible Design**: Easily add new algorithms and processing steps
- **Strategy Pattern**: Modular filter implementations
- **Command Pattern**: Operations that can be composed into pipelines
- **Observer Pattern**: Monitor processing progress in real-time
- **Factory Pattern**: Simplified creation of images and processing chains

### Error Handling & Safety

- **Result-Based API**: Uses `tl::expected<T, Error>` (aliased as `Result<T>`) for error propagation without exceptions
- **Boundary Checking**: Safe operations with proper validation
- **Strong Type Safety**: Prevents common programming errors

### Performance Features

- **Parallel Processing**: Thread pool for multi-core utilization
- **Profiling Tools**: Built-in performance measurement and logging
- **LTO/IPO**: Link-time and interprocedural optimization enabled for release builds

### I/O Capabilities

- **Image Format Support**:
  - PPM/PGM/PBM formats (full read/write)
  - BMP format (24/32-bit read/write)
  - JPEG format (read/write)
  - PNG format (read/write)

### Developer Tools

- **Logging**: Built-in logging system with multiple levels
- **Development Environment**: Includes configurations for clang-format, clang-tidy, and clangd

## Quick Start

### Installation

#### Prerequisites

- C++23 compatible compiler (GCC 12+, Clang 15+, MSVC 19.34+)
- CMake 3.24 or higher

#### Building from Source

```bash
# Clone the repository
git clone https://github.com/paulkokos/DIPAL.git
cd DIPAL

# Create build directory
mkdir build && cd build

# Configure
cmake ..

# Build
cmake --build .

# Run tests
ctest

# Install (optional)
cmake --install .
```

### Basic Usage

```cpp
#include <DIPAL/DIPAL.hpp>
#include <iostream>

int main() {
    // Load an image
    auto result = DIPAL::ImageFactory::loadImage("input.ppm");
    if (!result) {
        std::cerr << "Error loading image: " << result.error().toString() << std::endl;
        return 1;
    }

    auto image = std::move(result.value());

    // Apply a Gaussian blur filter
    auto filter = DIPAL::GaussianBlurFilter(1.5, 5);
    auto blurResult = filter.apply(*image);
    
    if (!blurResult) {
        std::cerr << "Error applying filter: " << blurResult.error().toString() << std::endl;
        return 1;
    }
    
    auto blurred = std::move(blurResult.value());

    // Apply Sobel edge detection
    auto sobelFilter = DIPAL::SobelFilter(true);
    auto edgeResult = sobelFilter.apply(*blurred);
    
    if (!edgeResult) {
        std::cerr << "Error applying edge detection: " << edgeResult.error().toString() << std::endl;
        return 1;
    }
    
    auto edges = std::move(edgeResult.value());

    // Save the result
    auto saveResult = DIPAL::ImageFactory::saveImage(*edges, "output.bmp");
    if (!saveResult) {
        std::cerr << "Error saving image: " << saveResult.error().toString() << std::endl;
        return 1;
    }

    std::cout << "Image processed successfully!" << std::endl;
    return 0;
}
```

### Using the Processing Pipeline

```cpp
#include <DIPAL/DIPAL.hpp>
#include <iostream>

int main() {
    // Create a processor with a console observer to show progress
    DIPAL::ImageProcessor processor;
    auto observer = std::make_shared<DIPAL::ConsoleObserver>();
    processor.addObserver(observer);
    
    // Load an image
    auto imageResult = DIPAL::ImageFactory::loadImage("input.ppm");
    if (!imageResult) {
        std::cerr << "Error loading image: " << imageResult.error().toString() << std::endl;
        return 1;
    }
    
    // Create a processing pipeline with multiple operations
    std::vector<std::unique_ptr<DIPAL::ProcessingCommand>> commands;
    
    // Add a Gaussian blur command
    auto blurFilter = std::make_unique<DIPAL::GaussianBlurFilter>(1.5, 5);
    commands.push_back(std::make_unique<DIPAL::FilterCommand>(std::move(blurFilter)));
    
    // Add a Sobel edge detection command
    auto sobelFilter = std::make_unique<DIPAL::SobelFilter>(true);
    commands.push_back(std::make_unique<DIPAL::FilterCommand>(std::move(sobelFilter)));
    
    // Process the image with the pipeline
    auto result = processor.processAll(*imageResult.value(), commands);
    
    if (!result) {
        std::cerr << "Error processing image: " << result.error().toString() << std::endl;
        return 1;
    }
    
    // Save the result
    auto saveResult = DIPAL::ImageFactory::saveImage(*result.value(), "output.bmp");
    if (!saveResult) {
        std::cerr << "Error saving image: " << saveResult.error().toString() << std::endl;
        return 1;
    }
    
    std::cout << "Image processed successfully!" << std::endl;
    return 0;
}
```

### Parallel Processing

```cpp
#include <DIPAL/DIPAL.hpp>
#include <iostream>

int main() {
    // Create a parallel processor to utilize multiple CPU cores
    DIPAL::ParallelProcessor processor(0);  // 0 means auto-detect thread count
    
    // Load a large image
    auto imageResult = DIPAL::ImageFactory::loadImage("large_image.ppm");
    if (!imageResult) {
        std::cerr << "Error loading image: " << imageResult.error().toString() << std::endl;
        return 1;
    }
    
    // Create a Gaussian blur filter
    DIPAL::GaussianBlurFilter filter(2.0, 7);
    
    // Apply the filter using parallel processing
    auto result = processor.applyFilter(*imageResult.value(), filter);
    
    if (!result) {
        std::cerr << "Error processing image: " << result.error().toString() << std::endl;
        return 1;
    }
    
    // Save the result
    auto saveResult = DIPAL::ImageFactory::saveImage(*result.value(), "output.bmp");
    if (!saveResult) {
        std::cerr << "Error saving image: " << saveResult.error().toString() << std::endl;
        return 1;
    }
    
    std::cout << "Image processed successfully with " << processor.getThreadCount() << " threads!" << std::endl;
    return 0;
}
```

## Examples

The `examples/` directory contains working examples:

- **basic/** - Image loading/saving, filter application, unsharp masking, transformations, JPEG I/O
- **advanced/** - Multithreaded processing, processing pipelines, filter profiling

## CMake Build Options

| Option | Default | Description |
|--------|---------|-------------|
| `BUILD_TESTS` | ON | Build the test suite (Google Test fetched automatically) |
| `BUILD_EXAMPLES` | ON | Build example applications |
| `BUILD_SHARED_LIBS` | ON | Build as shared library |
| `ENABLE_SANITIZERS` | OFF | Enable AddressSanitizer and UndefinedBehaviorSanitizer (Debug) |
| `ENABLE_LTO` | ON | Enable Link-Time Optimization |
| `ENABLE_IPO` | ON | Enable Interprocedural Optimization |

## Documentation

- [Build Instructions](docs/BUILD_INSTRUCTIONS.md)
- [C++23 Features Used](docs/CPP23_FEATURES.md)
- [Architecture Overview](docs/ARCHITECTURE_OVERVIEW.md)
- [Contributing Guide](CONTRIBUTING.md)
- [Upgrade Guide](docs/UPGRADE_GUIDE.md)

## Project Structure

```
DIPAL/
├── include/DIPAL/       # Public headers
│   ├── Core/            # Result, Error, Concepts, Types
│   ├── Image/           # Image classes (Binary, Grayscale, Color)
│   ├── Filters/         # Gaussian, Median, Sobel, UnsharpMask
│   ├── Transformation/  # Resize, Rotate, Affine, Warp
│   ├── Color/           # Color conversions and spaces
│   ├── IO/              # BMP, PPM, JPEG, PNG
│   ├── ImageProcessor/  # Sequential and parallel processing
│   ├── Observer/        # Progress monitoring
│   └── Utils/           # Logger, Profiler, Threading
├── src/                 # Implementation files
├── tests/               # Unit, integration, performance, stress tests
├── examples/            # Example applications
├── docs/                # Documentation
└── cmake/               # CMake modules
```

## Roadmap

- **v0.2.0**: Additional filters (morphological operations, Canny edge detection, bilateral filter)
- **v0.3.0**: Extended color space support (CIELAB, YCbCr, HSI/HLS)
- **v0.4.0**: Histogram operations, segmentation algorithms
- **v0.5.0**: GPU acceleration for supported operations
- **v1.0.0**: API stabilization and comprehensive documentation

See [Feature Roadmap](docs/FEATURE_ADDITIONS_ROADMAP.md) for the full development plan.

## Contributing

Please read [CONTRIBUTING.md](CONTRIBUTING.md) for details on our code of conduct and the process for submitting pull requests.

## License

This project is licensed under the BSD 3-Clause License - see the [LICENSE](LICENSE) file for details.

## Acknowledgments

- The STB library for inspiration on image handling
- The OpenCV project for reference implementations
- [TartanLlama/expected](https://github.com/TartanLlama/expected) for the `std::expected` backport
