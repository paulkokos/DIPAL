# DIPAL Getting Started Tutorial

## Prerequisites

- C++23 compatible compiler (GCC 12+, Clang 15+, MSVC 19.34+)
- CMake 3.24 or higher

## Building

```bash
git clone https://github.com/paulkokos/DIPAL.git
cd DIPAL
mkdir build && cd build
cmake ..
cmake --build .
```

## 1. Loading and Saving Images

```cpp
#include <DIPAL/DIPAL.hpp>
#include <iostream>

int main() {
    // Load a PPM image
    auto result = DIPAL::ImageFactory::loadImage("photo.ppm");
    if (!result) {
        std::cerr << result.error().toString() << std::endl;
        return 1;
    }
    auto image = std::move(result.value());

    // Save as BMP
    auto save = DIPAL::ImageFactory::saveImage(*image, "output.bmp");
    if (!save) {
        std::cerr << save.error().toString() << std::endl;
        return 1;
    }
    return 0;
}
```

Supported formats: PPM/PGM/PBM, BMP (24/32-bit), JPEG, PNG.

## 2. Creating Images Programmatically

```cpp
// Create a 640x480 color image
auto color = DIPAL::ImageFactory::createColor(640, 480);

// Create a 256x256 grayscale image
auto gray = DIPAL::ImageFactory::createGrayscale(256, 256);

// Set individual pixels
color->setPixel(10, 20, 255, 0, 0);  // red pixel at (10, 20)
```

## 3. Applying Filters

```cpp
// Gaussian blur (sigma=1.5, kernel size=5)
DIPAL::GaussianBlurFilter blur(1.5f, 5);
auto blurred = blur.apply(*image);

// Sobel edge detection (with normalization)
DIPAL::SobelFilter sobel(true);
auto edges = sobel.apply(*image);

// Median filter (kernel size 3)
DIPAL::MedianFilter median(3);
auto denoised = median.apply(*image);

// Unsharp mask (amount=1.5, radius=2.0, threshold=10)
DIPAL::UnsharpMaskFilter sharpen(1.5f, 2.0f, 10);
auto sharpened = sharpen.apply(*image);
```

All filter methods return `Result<std::unique_ptr<Image>>`. Check the result before using:

```cpp
auto result = blur.apply(*image);
if (!result) {
    std::cerr << result.error().toString() << std::endl;
    return 1;
}
auto output = std::move(result.value());
```

## 4. Geometric Transformations

```cpp
// Resize with bilinear interpolation
DIPAL::ResizeTransform resize(320, 240, DIPAL::InterpolationMethod::Bilinear);
auto resized = resize.apply(*image);

// Rotate 90 degrees around center
DIPAL::RotateTransform rotate(90.0f);
auto rotated = rotate.apply(*image);
```

Available interpolation methods: `NearestNeighbor`, `Bilinear`, `Bicubic`.

## 5. Processing Pipelines

Chain multiple operations using the command pattern:

```cpp
DIPAL::ImageProcessor processor;

// Add a progress observer
auto observer = std::make_shared<DIPAL::ConsoleObserver>();
processor.addObserver(observer);

// Create commands
auto blurFilter = std::make_unique<DIPAL::GaussianBlurFilter>(1.5f, 5);
auto command = std::make_unique<DIPAL::FilterCommand>(std::move(blurFilter));

// Process
auto result = processor.process(*image, std::move(command));
```

## 6. Parallel Processing

Use `ParallelProcessor` for multi-threaded execution:

```cpp
DIPAL::ParallelProcessor parallel(0);  // 0 = auto-detect thread count

DIPAL::GaussianBlurFilter filter(2.0f, 7);
auto result = parallel.applyFilter(*image, filter);

std::cout << "Processed with " << parallel.getThreadCount() << " threads" << std::endl;
```

## 7. Color Conversions

```cpp
// RGB to HSV
auto hsv = DIPAL::ColorConversions::rgbToHsv(r, g, b);

// HSV to RGB
auto rgb = DIPAL::ColorConversions::hsvToRgb(h, s, v);
```

## 8. Error Handling

DIPAL uses `Result<T>` (based on `tl::expected`) instead of exceptions for error reporting:

```cpp
auto result = DIPAL::ImageFactory::loadImage("missing.ppm");
if (!result) {
    // Access the error
    auto& err = result.error();
    std::cerr << "Error: " << err.toString() << std::endl;
}
```

## Next Steps

- Browse the `examples/` directory for complete working programs
- See `docs/ARCHITECTURE_OVERVIEW.md` for system design
- See `docs/FEATURE_ADDITIONS_ROADMAP.md` for the full development plan
