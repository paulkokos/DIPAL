# C++23 Features Available in DIPAL

Now that DIPAL requires Clang 18+ or GCC 13+, you have access to powerful C++23 features. This guide shows you what's available and how to use them.

## Compiler Support

| Feature | Clang 18 | GCC 13 | Status |
|---------|----------|--------|--------|
| std::expected | ✅ | ✅ | Native in both |
| std::ranges | ✅ | ✅ | Full support |
| Deduced this | ✅ | ⚠️ | GCC partial |
| std::format | ✅ | ✅ | Full support |
| Pattern matching | ✅ | ✅ | Via if statements |
| std::stacktrace | ✅ | ⚠️ | Limited in GCC |

## 1. Native std::expected

### Before (Using TartanLlama/expected)
```cpp
#include <expected/expected.hpp>

using Result = tl::expected<Image, Error>;
```

### After (C++23 with Clang 18+/GCC 13+)
```cpp
#include <expected>

using Result = std::expected<Image, Error>;
```

### Usage Example
```cpp
Result<Image> loadImage(const std::string& path) {
    if (path.empty()) {
        return std::unexpected(Error{...});
    }
    Image img = {...};
    return img;
}

// Usage
auto result = loadImage("image.png");
if (result) {
    process(*result);  // Dereference to get Image
} else {
    handle_error(result.error());
}

// Or with map
auto processed = result
    .transform([](const Image& img) { return apply_filter(img); })
    .or_else([](const Error& err) {
        log(err);
        return std::unexpected(err);
    });
```

## 2. Improved std::ranges

### Range-based Algorithms
```cpp
#include <ranges>

// Filter pixels above threshold
auto bright_pixels = image.getData()
    | std::views::filter([](uint8_t pixel) { return pixel > 128; });

// Transform each pixel
auto inverted = image.getData()
    | std::views::transform([](uint8_t p) { return 255 - p; });

// Combine multiple operations
auto result = image.getData()
    | std::views::filter([](uint8_t p) { return p > 100; })
    | std::views::transform([](uint8_t p) { return p / 2; })
    | std::views::drop(10)  // Skip first 10
    | std::views::take(100);  // Take only 100
```

### Range-based Functions
```cpp
// Check if all pixels match condition
if (std::ranges::all_of(image.getData(),
    [](uint8_t p) { return p > 0; })) {
    // Image has no black pixels
}

// Count pixels matching condition
auto count = std::ranges::count_if(image.getData(),
    [](uint8_t p) { return p > 200; });

// Find first white pixel
auto it = std::ranges::find(image.getData(), 255);
```

## 3. Deduced this

### Problem: Perfect Forwarding in Member Functions
```cpp
// Old way - need both const and non-const versions
class Filter {
    void apply(Image& image) { /* ... */ }
    void apply(const Image& image) const { /* ... */ }
};

// New way - single implementation
class Filter {
    void apply(this auto&& self, auto&& image) {
        // Works for both const and non-const, lvalue and rvalue
    }
};
```

### Practical Example
```cpp
class ImageProcessor {
public:
    // Single implementation handles all cases!
    Result<Image> process(this auto&& self, auto&& source) {
        // self is either ImageProcessor or const ImageProcessor
        // source is Image, Image&, Image&&, etc.

        if constexpr (std::is_const_v<
            std::remove_reference_t<decltype(self)>>) {
            // const version
        } else {
            // mutable version
        }
    }
};
```

## 4. Enhanced std::format

### Basic Formatting
```cpp
#include <format>

Image img(640, 480);
auto info = std::format("Image: {}x{}", img.width(), img.height());
// Result: "Image: 640x480"
```

### Custom Formatters
```cpp
// Define formatter for your types
template<>
struct std::formatter<Image> {
    constexpr auto parse(auto& ctx) { return ctx.begin(); }

    auto format(const Image& img, auto& ctx) const {
        return std::format_to(ctx.out(),
            "Image({}x{})", img.width(), img.height());
    }
};

// Now you can use it directly
Image img{640, 480};
std::println("Image info: {}", img);  // "Image info: Image(640x480)"
```

### Format with Alignment
```cpp
// Left align in 20 chars
std::format("{:<20}", "Filter name");

// Center in 30 chars
std::format("{:^30}", "Processing");

// Right align with padding
std::format("{:>10}", 42);

// With specifiers
std::format("{:.2f}", 3.14159);  // 3.14
```

## 5. Pattern Matching (if statements)

### Structured Bindings
```cpp
// With Result<T>
if (auto [value, error] = process(image); !error) {
    use(value);
} else {
    handle(error);
}

// With std::optional
if (auto opt = findImage("path"); opt.has_value()) {
    process(*opt);
}

// With std::expected
if (auto result = loadImage("file.png"); result) {
    Image img = *result;  // Success case
} else {
    Error err = result.error();  // Failure case
}
```

### Multiple Conditions
```cpp
if (auto res = load("image.png");
    res && res->width() > 0 && res->height() > 0) {
    // Image loaded successfully with valid dimensions
    process(*res);
} else {
    // Handle error
}
```

## 6. Designated Initializers

### Readable Initialization
```cpp
struct FilterConfig {
    float sigma;
    int kernel_size;
    std::string name;
};

// Instead of: FilterConfig{1.5f, 5, "Gaussian"}
auto config = FilterConfig{
    .sigma = 1.5f,
    .kernel_size = 5,
    .name = "Gaussian"
};
```

### Complex Types
```cpp
class GaussianBlur {
    struct Config {
        float sigma = 1.0f;
        int size = 3;
        bool normalize = true;
    };

    // Clear, maintainable initialization
    GaussianBlur() : config{
        .sigma = 2.0f,
        .size = 5,
        .normalize = true
    } {}
};
```

## 7. constexpr Improvements

### More Functions Can Be constexpr
```cpp
// Can now be evaluated at compile-time
constexpr uint8_t clamp(uint8_t value, uint8_t min, uint8_t max) {
    return std::max(min, std::min(value, max));
}

// Compile-time LUT generation
constexpr auto generate_lookup_table() {
    std::array<uint8_t, 256> lut{};
    for (int i = 0; i < 256; i++) {
        lut[i] = clamp(i * 1.5f, 0, 255);
    }
    return lut;
}

constexpr auto LUT = generate_lookup_table();
```

## 8. Enhanced Concepts

### Define Requirements Clearly
```cpp
#include <concepts>

// Already in DIPAL - these are powerful!
template<typename T>
concept Pixel = requires(T p) {
    { p.red } -> std::integral;
    { p.green } -> std::integral;
    { p.blue } -> std::integral;
};

// Use in functions
template<Pixel P>
void process_pixel(P& pixel) {
    // Guaranteed P has red, green, blue
    pixel.red = pixel.red / 2;
}
```

## 9. auto in More Places

### Lambda Parameters (C++23)
```cpp
// Old way
auto transform = [](const Image& img) { return /* ... */; };

// New way - shorter syntax
auto transform = [](auto& img) {
    // img could be any type
    return /* ... */;
};

// With concepts (C++20/23)
auto filter = [](ImageType auto& img) {
    // img must satisfy ImageType concept
};
```

## 10. std::stacktrace (Debugging)

### Capture Stack Traces
```cpp
#include <stacktrace>

void error_handler(const std::string& msg) {
    auto trace = std::stacktrace::current();

    // Log the stack trace
    DIPAL_LOG_ERROR("Error: {}\nStack trace:\n{}",
        msg, std::to_string(trace));
}
```

## Practical Examples for DIPAL

### Example 1: Modern Filter Pipeline
```cpp
auto result = image
    | std::views::filter([](uint8_t p) { return p > 50; })
    | std::views::transform([](uint8_t p) { return 255 - p; })
    | std::views::chunk(4)  // C++23 - group into chunks
    | std::views::take_while(predicate);
```

### Example 2: Error Handling
```cpp
Result<Image> process_image(const std::string& path) {
    return loadImage(path)
        .transform([](Image img) {
            return apply_gaussian_blur(img, 1.5f);
        })
        .transform([](Image img) {
            return apply_unsharp_mask(img);
        })
        .or_else([](const Error& err) {
            log_error(err);
            return std::unexpected(err);
        });
}
```

### Example 3: Type-Safe Pixel Processing
```cpp
template<ImageType T>
requires std::integral<typename T::PixelType>
void normalize_brightness(T& image) {
    const auto max_val = 255;
    for (auto& pixel : image) {
        pixel = std::min(pixel, max_val);
    }
}
```

## Migration Guide

### Step 1: Use std::expected
Replace TartanLlama includes:
```cpp
// OLD
#include <expected/expected.hpp>
using Result = tl::expected<T, E>;

// NEW
#include <expected>
using Result = std::expected<T, E>;
```

### Step 2: Use std::ranges
Replace manual loops:
```cpp
// OLD
std::vector<uint8_t> filtered;
for (auto p : data) {
    if (p > 128) filtered.push_back(p);
}

// NEW
auto filtered = data
    | std::views::filter([](auto p) { return p > 128; });
```

### Step 3: Use std::format
Replace sprintf/printf:
```cpp
// OLD
char buffer[256];
snprintf(buffer, 256, "Size: %d x %d", w, h);

// NEW
auto info = std::format("Size: {} x {}", w, h);
```

## Performance Notes

1. **Ranges are lazy** - operations don't execute until you iterate
2. **std::format is faster** - optimized at compile-time
3. **Concepts add zero runtime cost** - compile-time checks only
4. **constexpr evaluation** - LUTs and constants computed at compile-time

## Compiler Flags

To enable all C++23 features:
```bash
# Clang 18
clang++-18 -std=c++23 ...

# GCC 13
g++-13 -std=c++2c ...
```

## References

- [cppreference C++23](https://en.cppreference.com/w/cpp/23)
- [Clang C++23 Status](https://clang.llvm.org/cxx_status.html)
- [GCC C++23 Status](https://gcc.gnu.org/projects/cxx-status.html)
- [C++23 Features](https://www.open-std.org/jtc1/sc22/wg21/)
