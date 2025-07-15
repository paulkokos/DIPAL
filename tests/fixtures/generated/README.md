# Generated Test Data

Procedurally generated test data for reproducible testing.

## Usage

Test data can be generated using the TestImageGenerator utilities:

```cpp
auto noiseImage = DIPAL::TestImageGenerator::generateNoiseImage(256, 256);
auto gradientImage = DIPAL::TestImageGenerator::generateColorGradientImage(512, 512);
```

## Benefits

- Reproducible test data
- Parametric testing with various sizes and properties
- No licensing concerns
- Unlimited variations for stress testing
