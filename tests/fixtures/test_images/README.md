# Test Images

Sample images for I/O and processing tests.

## Files

- Add sample images here for testing
- Include various formats: BMP, PPM, PGM, PBM
- Include various sizes and content types
- Document any special properties

## Generating Test Images

You can generate basic test images using the test utilities:

```cpp
auto testImage = DIPAL::ImageTestUtils::createTestGrayscaleImage(100, 100, 128);
// Save to file for use in tests
```
