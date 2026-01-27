# DIPAL Architecture Overview

**Version:** 0.1.0
**Last Updated:** January 2026

---

## Table of Contents

1. [System Architecture](#system-architecture)
2. [Module Hierarchy](#module-hierarchy)
3. [Dependency Graph](#dependency-graph)
4. [Data Flow](#data-flow)
5. [API Layers](#api-layers)
6. [Design Patterns Used](#design-patterns-used)
7. [Module Descriptions](#module-descriptions)
8. [Extension Points](#extension-points)

---

## System Architecture

### High-Level Architecture

```
┌─────────────────────────────────────────────────────────────┐
│                     APPLICATION LAYER                        │
│  (User applications, examples)                               │
└──────────────────────────┬──────────────────────────────────┘
                           │
┌──────────────────────────▼──────────────────────────────────┐
│                     PUBLIC API (DIPAL.hpp)                    │
│  Includes all public headers from every module               │
└──────────────────────────┬──────────────────────────────────┘
                           │
      ┌────────────────────┼────────────────────┐
      │                    │                    │
┌─────▼──────┐  ┌─────────▼────────┐  ┌───────▼────────┐
│ Processing │  │  Filters Module   │  │  Transform Mod │
│            │  │                   │  │                │
│ - Processor│  │ - GaussianBlur    │  │ - Resize       │
│ - Parallel │  │ - Median          │  │ - Rotate       │
│ - Commands │  │ - Sobel           │  │ - Affine       │
│ - Observer │  │ - UnsharpMask     │  │ - Warp         │
└─────┬──────┘  └────────┬─────────┘  └───────┬────────┘
      │                  │                     │
      └──────────┬───────┴─────────────────────┘
                 │
      ┌──────────▼──────────┐
      │    Image Module     │
      │                     │
      │ - Image (base)      │
      │ - BinaryImage       │
      │ - GrayscaleImage    │
      │ - ColorImage        │
      │ - ImageFactory      │
      │ - ImageView         │
      │ - PixelIterator     │
      └──────────┬──────────┘
                 │
    ┌────────────┼────────────┐
    │            │            │
┌───▼────┐  ┌───▼────┐  ┌────▼───┐
│ Color  │  │  I/O   │  │  Core  │
│        │  │        │  │        │
│ - Conv │  │ - BMP  │  │ - Error│
│ - Space│  │ - PPM  │  │ - Result│
│ - Xform│  │ - JPEG │  │ - Types│
│        │  │ - PNG  │  │ - Conc.│
└────────┘  └────────┘  └────────┘
```

---

## Module Hierarchy

### Actual File Tree

```
include/DIPAL/
├── DIPAL.hpp                      # Main facade (includes all modules)
│
├── Core/
│   ├── Concepts.hpp               # C++20/23 concept constraints
│   ├── Core.hpp                   # Library init, version info
│   ├── Error.hpp                  # Error types, ErrorCode, ErrorCategory
│   ├── Iterators.hpp              # Custom iterator implementations
│   ├── Result.hpp                 # Result<T> alias for tl::expected<T, Error>
│   └── Types.hpp                  # Common type definitions and enums
│
├── Image/
│   ├── Image.hpp                  # Base image class
│   ├── BinaryImage.hpp            # 1-bit images
│   ├── GrayscaleImage.hpp         # 8/16-bit single channel
│   ├── ColorImage.hpp             # Multi-channel color (RGB/RGBA)
│   ├── ImageFactory.hpp           # Factory for creating/loading images
│   ├── ImageView.hpp              # Non-owning view into image data
│   └── PixelIterator.hpp          # Iterator for pixel-level access
│
├── Filters/
│   ├── Filters.hpp                # Convenience header
│   ├── FilterStrategy.hpp         # Abstract base (Strategy pattern)
│   ├── GaussianBlurFilter.hpp     # Gaussian blur
│   ├── MedianFilter.hpp           # Median filtering
│   ├── SobelFilter.hpp            # Sobel edge detection
│   └── UnsharpMaskFilter.hpp      # Unsharp mask sharpening
│
├── Transformation/
│   ├── Transformations.hpp        # InterpolationMethod enum, base types
│   ├── Interpolation.hpp          # Interpolation algorithms
│   ├── GeometricTransform.hpp     # Base geometric transform class
│   ├── ResizeTransform.hpp        # Image resizing
│   ├── RotateTransform.hpp        # Image rotation
│   ├── AffineTransform.hpp        # Affine transformations
│   └── WarpTransform.hpp          # General warping
│
├── Color/
│   ├── ColorConversions.hpp       # RGB/HSV/Grayscale conversion functions
│   ├── ColorSpace.hpp             # Abstract color space interface
│   └── ColorTransform.hpp         # Color transformation operations
│
├── IO/
│   ├── ImageIO.hpp                # Static facade for format-agnostic I/O
│   ├── BMPImageIO.hpp             # BMP read/write (24/32-bit)
│   ├── PPMImageIO.hpp             # PPM/PGM/PBM read/write
│   ├── JPEGImageIO.hpp            # JPEG read/write
│   └── PNGImageIO.hpp             # PNG read/write
│
├── ImageProcessor/
│   ├── ImageProcessor.hpp         # Sequential processor with observer support
│   ├── ParallelProcessor.hpp      # Multi-threaded processing via ThreadPool
│   ├── ProcessingCommand.hpp      # Abstract command interface
│   └── FilterCommand.hpp          # Command wrapper for filters
│
├── Observer/
│   ├── ProcessingObserver.hpp     # Abstract observer interface
│   └── ProgressObserver.hpp       # Progress tracking observer
│
└── Utils/
    ├── Concurrency.hpp            # ThreadPool implementation
    ├── Logger.hpp                 # Logging system
    ├── MemoryUtils.hpp            # Memory management utilities
    ├── Profiler.hpp               # Performance profiling
    └── Utils.hpp                  # General utilities
```

**Source files** (`src/`) mirror this structure with `.cpp` implementations for each header (40 files total).

---

## Dependency Graph

### Module Dependencies

```
DIPAL.hpp (facade)
    ├──> ImageProcessor/
    │    ├──> Image/
    │    ├──> Filters/ (via FilterCommand)
    │    ├──> Observer/
    │    └──> Utils/Concurrency (ParallelProcessor)
    │
    ├──> Filters/
    │    └──> Image/
    │
    ├──> Transformation/
    │    └──> Image/
    │
    ├──> Image/
    │    └──> Core/ (Error, Result, Types)
    │
    ├──> IO/
    │    ├──> Image/
    │    └──> Core/ (Error, Result)
    │
    ├──> Color/
    │    ├──> Image/
    │    └──> Core/
    │
    ├──> Observer/
    │    └──> Core/
    │
    ├──> Utils/
    │    └──> Core/
    │
    └──> Core/
         └──> tl::expected (external)
```

### Dependency Levels

**Level 0 (No internal dependencies):**
- Core (Result, Error, Concepts, Types) - depends only on `tl::expected`

**Level 1 (Depends on Core):**
- Image (Image, BinaryImage, GrayscaleImage, ColorImage, ImageFactory, ImageView, PixelIterator)
- Observer (ProcessingObserver, ProgressObserver)
- Utils (Concurrency, Logger, MemoryUtils, Profiler)

**Level 2 (Depends on Level 0-1):**
- Filters (FilterStrategy, GaussianBlur, Median, Sobel, UnsharpMask) - depends on Image
- Transformation (Resize, Rotate, Affine, Warp, Interpolation) - depends on Image
- Color (ColorConversions, ColorSpace, ColorTransform) - depends on Image
- IO (BMP, PPM, JPEG, PNG) - depends on Image

**Level 3 (Depends on Level 0-2):**
- ImageProcessor (ImageProcessor, ParallelProcessor, FilterCommand) - depends on Image, Filters, Observer, Utils

---

## Data Flow

### Image Processing Pipeline

```
Input File (BMP/PPM/JPEG/PNG)
       │
   ImageIO::load() / ImageFactory::loadImage()
       │
   ┌─────────────────────────────┐
   │    Image Object             │
   │  - width, height, channels  │
   │  - pixel data (uint8_t[])   │
   │  - Image::Type enum         │
   └─────────────────────────────┘
       │
   ImageProcessor::processAll() or direct filter.apply()
       │
   ┌─────────────────────────────┐
   │  Command 1: FilterCommand   │
   │  (e.g., GaussianBlurFilter) │
   │  -> Observer::onProgress()  │
   └─────────────────────────────┘
       │
   ┌─────────────────────────────┐
   │  Command 2: FilterCommand   │
   │  (e.g., SobelFilter)       │
   │  -> Observer::onProgress()  │
   └─────────────────────────────┘
       │
   ┌─────────────────────────────┐
   │    Result Image             │
   └─────────────────────────────┘
       │
   ImageIO::save() / ImageFactory::saveImage()
       │
   Output File
```

### Error Handling Flow

```
Operation Call
    │
tl::expected<T, Error>  (aliased as Result<T>)
    │
┌───────────┬─────────────┐
│           │             │
Success(T)  Error(E)
  │           │
  │         Error contains:
  │         - ErrorCode (enum)
Process     - ErrorCategory
Value       - message (string)
            - toString()
```

---

## API Layers

### Layer 1: Facade (DIPAL.hpp)

The main header `DIPAL.hpp` includes all public headers, giving users a single include:

```cpp
#include <DIPAL/DIPAL.hpp>
```

### Layer 2: Factory API

```cpp
// Load/save images (format auto-detected)
auto result = DIPAL::ImageFactory::loadImage("photo.bmp");
auto saveOk = DIPAL::ImageFactory::saveImage(*image, "out.ppm");

// Create blank images
auto img = DIPAL::ImageFactory::createGrayscaleImage(width, height);
```

### Layer 3: Processing API

```cpp
// Direct filter application
DIPAL::GaussianBlurFilter filter(sigma, kernelSize);
auto result = filter.apply(*image);

// Pipeline via processor
DIPAL::ImageProcessor processor;
processor.addObserver(observer);
auto result = processor.processAll(*image, commands);

// Parallel processing
DIPAL::ParallelProcessor parallel(threadCount);
auto result = parallel.applyFilter(*image, filter);
```

### Layer 4: Utilities

```cpp
// Color conversions
DIPAL::ColorConversions::rgbToHsv(r, g, b);
DIPAL::ColorConversions::toGrayscale(*colorImage);

// Logging
DIPAL::Logger::info("Processing complete");

// Profiling
DIPAL::Profiler profiler;
profiler.start("blur");
// ... work ...
profiler.stop("blur");
```

---

## Design Patterns Used

### 1. Strategy Pattern

**Where:** Filters, Transformations

Filters implement a common `FilterStrategy` interface, allowing interchangeable algorithms:

```
FilterStrategy (abstract)
    ├── GaussianBlurFilter
    ├── MedianFilter
    ├── SobelFilter
    └── UnsharpMaskFilter
```

### 2. Factory Pattern

**Where:** ImageFactory

Centralizes image creation and I/O with format auto-detection:

```
ImageFactory
    ├── loadImage(path)       -> detects format, delegates to IO
    ├── saveImage(img, path)  -> detects format, delegates to IO
    ├── createGrayscaleImage()
    └── createColorImage()
```

### 3. Command Pattern

**Where:** ImageProcessor, FilterCommand, ProcessingCommand

Operations are encapsulated as command objects for pipeline composition:

```
ProcessingCommand (abstract)
    └── FilterCommand (wraps a FilterStrategy)

ImageProcessor
    └── processAll(image, vector<Command>)
```

### 4. Observer Pattern

**Where:** ImageProcessor, ProcessingObserver, ProgressObserver

Decouples processing progress reporting from the processor itself:

```
ImageProcessor (subject)
    ├── addObserver(observer)
    └── notifies observers during processAll()

ProcessingObserver (abstract)
    └── ProgressObserver (concrete)
```

---

## Module Descriptions

### Core Module
**Responsibility:** Type system, error handling, concepts
**Key types:** `Result<T>`, `Error`, `ErrorCode`, `ErrorCategory`
**External dependency:** `tl::expected` v1.1.0

### Image Module
**Responsibility:** Image representation and pixel access
**Key classes:** `Image` (base), `BinaryImage`, `GrayscaleImage`, `ColorImage`, `ImageFactory`, `ImageView`, `PixelIterator`

### Filters Module
**Responsibility:** Image filtering algorithms
**Key classes:** `FilterStrategy` (base), `GaussianBlurFilter`, `MedianFilter`, `SobelFilter`, `UnsharpMaskFilter`

### Transformation Module
**Responsibility:** Geometric image transformations
**Key classes:** `GeometricTransform` (base), `ResizeTransform`, `RotateTransform`, `AffineTransform`, `WarpTransform`, `Interpolation`
**Interpolation methods:** Nearest Neighbor, Bilinear, Bicubic

### Color Module
**Responsibility:** Color space conversions
**Key classes:** `ColorConversions` (static methods), `ColorSpace` (abstract), `ColorTransform`
**Conversions available:** RGB to/from HSV, grayscale

### IO Module
**Responsibility:** Image file read/write
**Key classes:** `ImageIO` (facade), `BMPImageIO`, `PPMImageIO`, `JPEGImageIO`, `PNGImageIO`

### ImageProcessor Module
**Responsibility:** Processing orchestration
**Key classes:** `ImageProcessor` (sequential), `ParallelProcessor` (multi-threaded), `FilterCommand`, `ProcessingCommand`

### Observer Module
**Responsibility:** Progress notification
**Key classes:** `ProcessingObserver` (abstract), `ProgressObserver`

### Utils Module
**Responsibility:** Cross-cutting utilities
**Key classes:** `ThreadPool` (in Concurrency.hpp), `Logger`, `Profiler`, `MemoryUtils`

---

## Extension Points

### Adding a New Filter

1. Create a header inheriting from `FilterStrategy`:

```cpp
// include/DIPAL/Filters/MyFilter.hpp
#pragma once
#include <DIPAL/Filters/FilterStrategy.hpp>

namespace DIPAL {

class MyFilter : public FilterStrategy {
public:
    MyFilter(double param);
    [[nodiscard]] std::string getName() const override;
    [[nodiscard]] Result<std::unique_ptr<Image>> apply(const Image& input) override;

private:
    double m_param;
};

} // namespace DIPAL
```

2. Add the implementation in `src/Filters/MyFilter.cpp`
3. Include in `DIPAL.hpp` and add to `CMakeLists.txt`

### Adding a New Image Format

1. Create a class following the pattern of `BMPImageIO` or `PPMImageIO`
2. Implement static `load()` and `save()` methods returning `Result<T>`
3. Register the format in `ImageIO.cpp` for auto-detection

### Adding a New Transformation

1. Inherit from `GeometricTransform`
2. Implement the `apply()` method
3. Use `Interpolation` for sub-pixel sampling

### Adding a Custom Observer

1. Inherit from `ProcessingObserver`
2. Override the notification methods
3. Register with `ImageProcessor::addObserver()`

---

## Build Configuration

- **CMake minimum:** 3.24
- **C++ standard:** C++23 (enforced)
- **Compilers supported:** GCC 12+, Clang 15+, MSVC 19.34+
- **Warning flags:** `-Wall -Wextra -Wpedantic -Werror` (Unix), `/W4 /WX /permissive-` (MSVC)
- **Optimization:** LTO and IPO enabled in Release builds
- **Sanitizers:** ASan + UBSan available via `ENABLE_SANITIZERS`
- **Testing:** Google Test v1.15.2 (fetched via CMake FetchContent)
- **CI:** Azure Pipelines with multi-compiler matrix (GCC 13/14, Clang 19 on Ubuntu 22.04/24.04)
