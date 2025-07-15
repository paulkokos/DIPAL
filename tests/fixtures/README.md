# Test Fixtures

This directory contains test data and sample files for DIPAL tests.

## Structure

- `test_images/` - Sample images in various formats for testing I/O operations
- `corrupted/` - Intentionally malformed files for error handling tests
- `generated/` - Procedurally generated test data
- `large/` - Large test images for performance testing

## Usage

Test utilities can access these files using:

```cpp
auto testDataPath = DIPAL::TestUtils::getTestDataPath();
auto sampleImage = testDataPath / "test_images" / "sample.bmp";
```

## Adding Test Data

When adding new test files:

1. Keep file sizes reasonable (< 1MB for regular tests, mark large files clearly)
2. Include a variety of formats and properties
3. Document any special characteristics in this README
4. Consider licensing and copyright for any external images

## Test Image Properties

### Small Test Images (< 100KB)
- `small_grayscale.pgm` - 100x100 grayscale PGM
- `small_color.ppm` - 100x100 color PPM  
- `small_binary.pbm` - 100x100 binary PBM
- `tiny.bmp` - 32x32 BMP for quick tests

### Medium Test Images (100KB - 1MB)
- `medium_photo.bmp` - 500x500 photographic content
- `medium_graphics.bmp` - 500x500 synthetic graphics

### Large Test Images (> 1MB)
- `large_test.bmp` - 2048x2048 for performance testing
- Place in `large/` subdirectory

### Corrupted Test Files
- `invalid_header.bmp` - Invalid BMP header
- `truncated.ppm` - Incomplete PPM file
- `wrong_extension.txt` - Text file with image extension
