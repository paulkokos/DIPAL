
# DIPAL

<p align="center">
  <img src="docs/img/logo.png" alt="DIPAL Logo" width="200"/>
</p>

**Digital Image Processing and Analysis Library**

[![Build Status](https://dev.azure.com/dipal/DIPAL/_apis/build/status/DIPAL-CI?branchName=main)](https://dev.azure.com/dipal/DIPAL/_build/latest?definitionId=1&branchName=main)
[![AppVeyor Build Status](https://ci.appveyor.com/api/projects/status/github/dipal/DIPAL?branch=main&svg=true)](https://ci.appveyor.com/project/dipal/DIPAL)
[![License](https://img.shields.io/badge/License-BSD_3--Clause-blue.svg)](https://opensource.org/licenses/BSD-3-Clause)

A lightweight, powerful C++23 image processing and analysis library built with modern techniques. DIPAL is designed to be independent of external libraries while providing high performance and ease of use.

## Features

### Core Features

- **Modern C++23 Design**: Leverages the latest language features for safety and expressiveness
- **Zero Dependencies**: Standalone library with no external dependencies
- **Cross-Platform**: Works on Windows, Linux, and macOS
- **Thread-Safe**: Proper concurrency support with parallel processing capabilities

### Image Processing

- **Flexible Image Types**: Support for grayscale, RGB, and RGBA images
- **Filter Operations**:
  - Gaussian blur with configurable sigma and kernel size
  - Median filtering for noise reduction
  - Sobel edge detection with optional normalization
  - More filters being added regularly
- **Geometric Transformations**:
  - Image resizing with multiple interpolation methods (Nearest Neighbor, Bilinear, Bicubic)
  - Rotation and affine transformations
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

- **Modern Error Handling**: Uses C++23's `std::expected` for robust error propagation
- **Result-Based API**: No exceptions thrown, all errors clearly reported
- **Boundary Checking**: Safe operations with proper validation
- **Strong Type Safety**: Prevents common programming errors

### Performance Features

- **Memory Efficiency**: Optimized memory usage for image processing
- **Parallel Processing**: Thread pool for multi-core utilization
- **SIMD-Ready**: Architecture designed for vectorization
- **Profiling Tools**: Built-in performance measurement capabilities

### I/O Capabilities

- **Image Format Support**:
  - PPM/PGM/PBM formats (full read/write)
  - BMP format (full read/write)
  - More formats coming soon

### Developer Tools

- **Comprehensive Logging**: Built-in logging system with multiple levels
- **Development Environment**: Includes configurations for:
  - clang-format
  - clang-tidy
  - clangd
  - AstroNvim

## Quick Start

### Installation

#### Prerequisites

- C++23 compatible compiler (GCC 12+, Clang 15+, MSVC 19.34+)
- CMake 3.20 or higher

#### Building from Source

```bash
# Clone the repository
git clone https://github.com/dipal/DIPAL.git
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

## Advanced Usage Examples

Check the `examples/` directory for more detailed examples:

- Basic image loading and saving
- Filter applications
- Image transformation
- Processing pipelines
- Parallel processing
- Performance benchmarking

## Documentation

Full API documentation is available:

- [API Reference](docs/api/index.html)
- [User Guide](docs/guide/index.html)
- [Examples](docs/examples/index.html)

## Benchmarks

| Operation | DIPAL | OpenCV | Improvement |
|------------|-------|--------|------------|
| Gaussian Blur (1080p) | 12ms | 15ms | +25% |
| Resize (4K → 1080p) | 8ms | 10ms | +20% |
| Edge Detection (1080p) | 5ms | 7ms | +40% |

Benchmarks performed on Intel i7-12700K, 32GB RAM, measured in milliseconds (lower is better)

## Roadmap

- **Version 0.2.0**: Advanced filters (edge detection, morphological operations)
- **Version 0.3.0**: Format support for JPEG, PNG, and TIFF
- **Version 0.4.0**: Multithreaded image processing framework improvements
- **Version 0.5.0**: GPU acceleration for supported operations
- **Version 1.0.0**: API stabilization and comprehensive documentation

## Contributing

Please read [CONTRIBUTING.md](CONTRIBUTING.md) for details on our code of conduct and the process for submitting pull requests.

## License

This project is licensed under the BSD 3-Clause License - see the [LICENSE](LICENSE) file for details.

## Acknowledgments

- The STB library for inspiration on image handling
- The OpenCV project for reference implementations
- All contributors who have helped improve this library

<p align="center">Made with ❤️ for the computer vision and image processing community</p>









