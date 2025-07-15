# DIPAL Test Suite

Comprehensive test suite for the DIPAL (Digital Image Processing and Analysis Library).

## Quick Start

```bash
# Build all tests
mkdir build && cd build
cmake -DBUILD_TESTS=ON ..
make

# Run smoke tests (quick validation)
make test_smoke

# Run critical tests only
make test_critical

# Run all tests
make test
```

## Test Structure

### Test Categories

- **Unit Tests** (`tests/unit/`) - Individual component testing
- **Integration Tests** (`tests/integration/`) - Component interaction testing  
- **Performance Tests** (`tests/performance/`) - Benchmarking and performance validation
- **Stress Tests** (`tests/stress/`) - Resource limits and edge cases
- **Compatibility Tests** (`tests/compatibility/`) - Cross-platform validation

### Test Phases

#### Phase 1: Foundation (Priority: Critical)
- Core functionality (Result<T>, Error handling, Concepts)
- Processing engine (ImageProcessor, ParallelProcessor)
- I/O system (BMP, PPM, ImageIO)
- Known working filters (UnsharpMask, GaussianBlur)

#### Phase 2: Image System (Priority: High)
- Image classes (Grayscale, Color, Binary)
- Image support systems (Factory, View, Iterator)

#### Phase 3: Utilities (Priority: Medium)
- Memory and concurrency utilities
- Transformations and interpolation

#### Phase 4: Extended Features (Priority: Medium-Low)
- Additional filters and color operations
- Advanced transformations

#### Phase 5: Quality Assurance (Priority: Low)
- Stress testing and edge cases
- Cross-platform compatibility

## Running Tests

### By Phase
```bash
make test_phase1    # Foundation tests
make test_phase2    # Image system tests
```

### By Category
```bash
make test_integration_only  # Integration tests only
make test_performance_only  # Performance tests only
make test_stress_only      # Stress tests only
```

### Individual Tests
```bash
# Run specific test
./core_tests
./image_processor_tests

# Run with verbose output
./core_tests --gtest_verbose
```

### Test Filtering
```bash
# Run tests matching pattern
ctest -R "image.*tests"

# Run tests with specific label
ctest -L "unit"
ctest -L "integration"
ctest -L "performance"
```

## Test Development

### Adding New Tests

1. **Unit Test**: Add to appropriate `tests/unit/` file
2. **Integration Test**: Add to `tests/integration/`
3. **Performance Test**: Add to `tests/performance/`

### Test Utilities

The test suite includes comprehensive utilities:

- **TestUtils** - Common testing utilities
- **ImageTestUtils** - Image-specific test helpers
- **MockObservers** - Mock implementations for testing
- **TestImageGenerator** - Procedural test data generation

### Example Usage

```cpp
#include <gtest/gtest.h>
#include <DIPAL/DIPAL.hpp>
#include "test_utils.hpp"
#include "image_test_utils.hpp"

TEST(ExampleTest, CreateAndProcessImage) {
    // Create test image
    auto image = DIPAL::ImageTestUtils::createTestGrayscaleImage(100, 100, 128);
    ASSERT_NE(image, nullptr);
    
    // Test processing
    DIPAL::ImageProcessor processor;
    auto filter = std::make_unique<DIPAL::UnsharpMaskFilter>(1.5f, 2.0f, 10);
    auto command = std::make_unique<DIPAL::FilterCommand>(std::move(filter));
    
    auto result = processor.process(*image, std::move(command));
    EXPECT_TRUE(result.has_value());
}
```

## File Organization

```
tests/
├── unit/                    # 34 unit test files
├── integration/             # 6 integration test files
├── performance/             # 4 performance test files
├── stress/                  # 5 stress test files
├── compatibility/           # 3 compatibility test files
├── utils/                   # Test utilities and helpers
├── fixtures/                # Test data and sample files
├── CMakeLists.txt          # Build configuration
└── README.md               # This file
```

## Test Priorities

- ⭐ **CRITICAL** - Must work immediately (fully implemented features)
- 🔧 **HIGH** - Important functionality (likely implemented)
- 🔧 **MEDIUM** - Standard functionality (partially implemented)
- 🔧 **LOW** - Optional functionality (headers only)

## Contributing

When adding tests:

1. Follow the existing naming conventions
2. Use the provided test utilities when possible
3. Include both positive and negative test cases
4. Add performance tests for computationally intensive operations
5. Document any special test requirements

## Debugging Failed Tests

```bash
# Run with detailed output
ctest --output-on-failure --verbose

# Run specific failing test with gtest options
./failing_test --gtest_break_on_failure --gtest_catch_exceptions=0

# Run under debugger
gdb ./failing_test
```
