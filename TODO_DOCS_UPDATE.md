# DIPAL TODO

**Created:** January 27, 2026
**Last Updated:** January 27, 2026
**Current Version:** 0.1.0

---

## COMPLETED - Documentation Fixes (January 27, 2026)

All 5 documentation files updated to reflect the actual codebase:

- [x] README.md - Fixed clone URL, CMake version, removed fake benchmarks, fixed dead links, updated feature lists
- [x] ARCHITECTURE_OVERVIEW.md - Full rewrite to match actual 9-module structure
- [x] FEATURE_ADDITIONS_ROADMAP.md - Fixed LOC count, updated current state, fixed dates, clarified dependencies
- [x] IMPLEMENTATION_CHECKLIST.md - Checked off ~40 implemented items, updated dates and status
- [x] ROADMAP_README.md - Rewrote with accurate stats and dates

---

## Code Quality Issues

### Empty / Stub Files
- [ ] `include/DIPAL/Utils/Utils.hpp` - essentially empty, add content or remove
- [ ] `examples/basic/transform_example.cpp` - empty file, write or remove
- [ ] `docs/tutorial.md` - 0 bytes, write or remove

### File Name Typos
- [ ] `tests/performance/filter_betchmark.cpp` - rename to `filter_benchmark.cpp`
- [ ] `tests/performance/parallel_betchmark.cpp` - rename to `parallel_benchmark.cpp`

### Stub Test Files
- [ ] Many test files contain auto-generated scaffolding with TODO placeholders instead of real assertions. Fill in actual test logic.

### Main Header Gaps
- [ ] `JPEGImageIO.hpp` and `PNGImageIO.hpp` exist but are NOT included in `DIPAL.hpp`. Add includes or document why excluded.

### CI/CD Gaps
- [ ] Add clang-tidy as a CI step in Azure Pipelines
- [ ] Add code coverage reporting (Codecov or similar)

### Git Housekeeping
- [ ] Commit the untracked docs/ files (ARCHITECTURE_OVERVIEW.md, FEATURE_ADDITIONS_ROADMAP.md, IMPLEMENTATION_CHECKLIST.md, ROADMAP_README.md)
- [ ] Verify docs/index.md contents are current
- [ ] Verify tests/README.md test file counts match actual

---

## Filters - Not Yet Implemented

### Edge Detection
- [ ] Canny edge detector (Gaussian smoothing, gradient, non-maximal suppression, hysteresis thresholding)
- [ ] Laplacian filter (4-neighbor and 8-neighbor variants, Gaussian Laplacian)
- [ ] Prewitt operator (directional edge detection)
- [ ] Roberts Cross (2x2 diagonal operators)
- [ ] Marr-Hildreth (Gaussian derivative, zero-crossing detection)

### Morphological Operations
- [ ] Erosion and Dilation (structuring element support, custom kernels)
- [ ] Opening and Closing (small object removal, hole filling)
- [ ] Morphological gradient
- [ ] Skeleton / Thinning (medial axis, topology preservation)
- [ ] Top-Hat transform (white and black variants)
- [ ] Hit-or-Miss (pattern matching)
- [ ] Reconstruction (geodesic operations)

### Other Filters
- [ ] Bilateral filter (edge-preserving blur)
- [ ] High-pass filter
- [ ] Low-pass filter

### Convolution Framework
- [ ] Generic convolution (custom kernel, border modes, normalization)
- [ ] Separable convolution (kernel decomposition, 1D chains)
- [ ] Structuring elements (rectangle, ellipse, cross, diamond, custom)

### Enhancements to Existing Filters
- [ ] Gaussian blur: separable convolution, multi-scale pyramids, variable sigma per channel, boundary modes
- [ ] Median filter: heap optimization, adaptive variant, variable kernel sizes, custom shapes
- [ ] Sobel filter: direction computation, non-maximal suppression, 8-neighbor variant

---

## Color Spaces - Not Yet Implemented

### New Conversions
- [ ] RGB to/from HSI (intensity-based)
- [ ] RGB to/from HLS (lightness-based, CSS compatibility)
- [ ] RGB to/from CIELAB (via XYZ, illuminant support D50/D65, perceptually uniform)
- [ ] RGB to/from CMY (subtractive color, printing workflows)
- [ ] RGB to/from sRGB (gamma correction 2.2)
- [ ] RGB to/from YCbCr (BT.601, BT.709, BT.2020, JPEG compatibility)
- [ ] RGB to/from YIQ (NTSC legacy)
- [ ] HMMD color space

### Color Utilities
- [ ] ColorSpaceFactory (runtime creation, automatic conversion routing)
- [ ] Color difference calculation (Euclidean, CIEDE2000, CMC, Manhattan)
- [ ] Gamut mapping (out-of-gamut handling)
- [ ] Color quantization (median cut, K-Means, octree)

### Enhancements to Existing
- [ ] RGB-HSV: integer and float variants, gamma-corrected versions, SIMD, batch processing

---

## Transformations - Not Yet Implemented

### New Transform Types
- [ ] Translation (pixel shifting, border modes: reflect/wrap/constant/replicate, sub-pixel)
- [ ] Reflection / Flipping (horizontal, vertical, diagonal, in-place)
- [ ] Shear transformation (X/Y shear, text skew correction)
- [ ] Projective transformation (3x3 homography, 4-point correspondence, perspective correction)
- [ ] Elastic deformation (thin-plate spline, RBF interpolation)

### New Interpolation Methods
- [ ] Lanczos3
- [ ] Lanczos4
- [ ] Spline-based
- [ ] Gaussian
- [ ] Sinc-based

### Enhancements to Existing
- [ ] Rotation: output expansion handling, 90/180/270 degree fast paths
- [ ] Resize: aspect ratio preservation, content-aware (seam carving), downsampling optimization
- [ ] Affine: point correspondence (3 points), matrix inversion, batch support
- [ ] Warp: barrel/pincushion distortion, polar conversion, custom warp patterns

---

## Image I/O - Not Yet Implemented

### New Formats
- [ ] TIFF (compression: None/CCITT/LZW/JPEG/Deflate, multi-page, tile-based, metadata, BigTIFF)
- [ ] GIF (animation, frame timing, palette, interlacing, transparency)
- [ ] WEBP (lossless/lossy, animation, quality parameters)
- [ ] HEIF/HEIC (modern mobile format, multi-image, depth maps)
- [ ] RAW (generic, camera-specific, demosaicing)
- [ ] SVG export (path/contour/shape export)

### Enhancements to Existing
- [ ] PPM: ASCII and binary variant handling, comment support, whitespace edge cases
- [ ] BMP: all bit depths (1, 4, 8, 16), RLE compression, color palette
- [ ] PNG: all bit depths (1-16), all color types, EXIF, compression control, gamma
- [ ] JPEG: quality parameter, progressive JPEG, EXIF preservation, subsampling, DCT optimization

---

## Advanced Image Processing - Not Yet Implemented

### Segmentation
- [ ] Thresholding (fixed, Otsu, adaptive/local, multi-level)
- [ ] Connected component labeling (4/8-connectivity, statistics, bounding boxes, centroids)
- [ ] K-Means clustering
- [ ] Watershed segmentation (gradient-based, seed-based, marker-controlled)
- [ ] Fuzzy C-Means (soft clustering)

### Feature Extraction
- [ ] Corner detection (Harris, FAST, strength thresholding, NMS)
- [ ] Blob detection (LoG, DoG, multi-scale)
- [ ] Contour extraction (finding, area, perimeter, circularity, eccentricity, moments)
- [ ] Template matching (cross-correlation, normalized, SSD)
- [ ] Hough transform (lines, circles, ellipses)

### Histogram Operations
- [ ] Histogram calculation (single/multi-channel, configurable bins)
- [ ] Histogram equalization (global, CLAHE, contrast stretching)
- [ ] Histogram matching (source-to-reference, color transfer)

---

## Image Class Enhancements - Not Yet Implemented

### Image Metadata
- [ ] ImageMetadata structure (filename, format, dimensions, color space, timing, EXIF, processing history, custom tags)

### Image Comparison
- [ ] PSNR (Peak Signal-to-Noise Ratio)
- [ ] SSIM (Structural Similarity Index)
- [ ] MSE (Mean Squared Error)
- [ ] Equality operators

### ImageView Enhancements
- [ ] Crop operations
- [ ] Slicing support
- [ ] Tiling support

### Image Type Enhancements
- [ ] BinaryImage: invert, white pixel count, contour finding, connected component labeling
- [ ] GrayscaleImage: histogram computation, contrast calculation, histogram equalization, thresholding
- [ ] ColorImage: channel extraction, channel replacement, color space conversion delegation

### Memory Management
- [ ] Ring buffer for large images
- [ ] Memory pooling system
- [ ] Tile-based processing

---

## Mathematical Framework - Not Yet Implemented

### Linear Algebra
- [ ] Vector operations (dot, cross, magnitude, normalization, L1/L2/Linf norms)
- [ ] Matrix operations (multiply, transpose, determinant, rank, QR, SVD, eigen, solve Ax=b)

### Statistics
- [ ] Descriptive statistics (mean, median, mode, variance, stddev, skewness, kurtosis, percentiles)
- [ ] Correlation and covariance (Pearson, Spearman, covariance matrix, partial correlation)

### Calculus
- [ ] Numerical derivatives (gradient, Laplacian, directional, Hessian)
- [ ] Integration (cumulative functions, lookup tables)

### Geometry
- [ ] Distance metrics (Euclidean, Manhattan, Chebyshev, Mahalanobis)
- [ ] Geometric operations (angle between vectors, point-to-line, point-in-polygon, line intersections, circle/ellipse fitting)

---

## Performance & Optimization - Not Yet Implemented

### Threading Enhancements
- [ ] Load balancing for ParallelProcessor
- [ ] Work stealing queue
- [ ] Cache-aware scheduling

### SIMD
- [ ] Runtime CPU feature detection
- [ ] SSE intrinsics for filters
- [ ] AVX / AVX-512 intrinsics
- [ ] NEON (ARM)
- [ ] Compiler auto-vectorization hints

### GPU (Future)
- [ ] CUDA support
- [ ] OpenCL support
- [ ] Vulkan compute shaders

### Memory Optimization
- [ ] Buffer pooling (pre-allocated, size-stratified, NUMA-aware)
- [ ] Tile-based processing (configurable size, boundary handling, streaming I/O)
- [ ] Copy-on-write semantics (lazy duplication, shared views, modification tracking)

### Caching
- [ ] Image result cache (LRU eviction, size limits, hit/miss stats)
- [ ] Intermediate result caching (pipeline stage, undo/redo, checkpoints)

### Profiling Enhancements
- [ ] Memory profiling
- [ ] CPU profiling
- [ ] Flamegraph generation
- [ ] Benchmark suite (standard ops, scaling tests, regression detection, report generation)

---

## Design Patterns - Not Yet Implemented

### New Patterns
- [ ] FilterFactory (registration system, parameter validation)
- [ ] TransformationFactory (registry-based creation, parameter builders, composition)
- [ ] CompositeStrategy (pre-optimized chains, common operation groups)
- [ ] PIMPL pattern (ABI stability across public classes)
- [ ] Builder pattern (image creation)
- [ ] Decorator pattern (filter decoration)
- [ ] Chain of Responsibility (validation)

### Observer Enhancements
- [ ] Performance metrics observer
- [ ] Memory monitoring observer
- [ ] Warning/error reporting observer
- [ ] Stage completion observer
- [ ] ConsoleObserver (progress bars)
- [ ] FileObserver (logging to file)
- [ ] MetricsCollector
- [ ] DiagnosticsObserver
- [ ] ValidationObserver

### Processor Enhancements
- [ ] Pipeline checkpoint support
- [ ] Stage caching
- [ ] Pipeline serialization

---

## Testing - Not Yet Implemented

- [ ] Segmentation tests
- [ ] Feature extraction tests
- [ ] Mathematical operation tests
- [ ] Large file handling tests
- [ ] Clang-tidy CI enforcement
- [ ] Pre-commit hooks for clang-format
- [ ] Static analysis in CI (Valgrind/ASan for memory leaks, use-after-free, data races)
- [ ] Fuzz testing (format parsers, color space conversions, filters)
- [ ] Code coverage reporting (80%+ goal)
- [ ] Branch coverage tracking
- [ ] Compilation time tracking
- [ ] Binary size tracking

---

## Documentation - Not Yet Implemented

- [ ] Doxygen configuration and API reference generation
- [ ] Quick start guide
- [ ] User guide
- [ ] Design patterns guide
- [ ] Performance optimization guide
- [ ] Troubleshooting guide / FAQ
- [ ] Tutorial (docs/tutorial.md is currently empty)
- [ ] Complete example programs (25+ planned, ~12 exist)

---

## Infrastructure & DevOps - Not Yet Implemented

### Docker
- [ ] Dockerfile (multi-stage development image)
- [ ] Dockerfile.prod (production optimized)
- [ ] docker-compose.dev.yml / docker-compose.test.yml
- [ ] GitHub Container Registry integration

### CI/CD Enhancements
- [ ] GitHub Actions workflows (build, test, release)
- [ ] Performance regression detection in CI
- [ ] Artifact caching strategy

### Build System
- [ ] Vcpkg integration (vcpkg.json)
- [ ] Conan integration (conanfile.py)
- [ ] CMake BUILD_BENCHMARKS option
- [ ] CMake BUILD_DOCS option
- [ ] Dev container configuration (.devcontainer/)

### Distribution
- [ ] Vcpkg port
- [ ] Conan package
- [ ] System packages (apt, rpm, brew)
- [ ] GitHub Releases with multi-platform binaries
- [ ] Changelog generation
- [ ] GPG signing

---

## Summary

| Category | Remaining Items |
|----------|----------------|
| Code quality fixes | ~10 |
| Filters | ~30 |
| Color spaces | ~15 |
| Transformations | ~20 |
| I/O formats | ~25 |
| Advanced processing | ~25 |
| Image class | ~20 |
| Math framework | ~25 |
| Performance | ~25 |
| Design patterns | ~15 |
| Testing | ~12 |
| Documentation | ~8 |
| Infrastructure | ~15 |
| **Total** | **~245** |
