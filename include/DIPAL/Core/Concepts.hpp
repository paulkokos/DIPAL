// include/DIPAL/Color/Concepts.hpp
#ifndef DIPAL_CONCEPTS_HPP
#define DIPAL_CONCEPTS_HPP

#include <memory>
#include <concepts>
#include <type_traits>
#include <span>
#include <ranges>

namespace DIPAL {

// Concept for types that can be used as pixel values
template <typename T>
concept Pixel = std::is_arithmetic_v<T> && !std::is_same_v<T, bool>;

// Concept for image types
template <typename T>
concept ImageType = requires(T t, int x, int y) {
    { t.getWidth() } -> std::convertible_to<int>;
    { t.getHeight() } -> std::convertible_to<int>;
    { t.isEmpty() } -> std::convertible_to<bool>;
    { t.getData() } -> std::convertible_to<const std::uint8_t*>;
};

// Concept for filter types
template <typename T>
concept Filter = ImageType<T> && requires(T t, const T& img) {
    { t.apply(img) } -> std::convertible_to<std::unique_ptr<T>>;
    { t.getName() } -> std::convertible_to<std::string>;
};

// Concept for processing commands
template <typename T>
concept Command = ImageType<T> && requires(T t, const T& img) {
    { t.execute(img) } -> std::convertible_to<std::unique_ptr<T>>;
    { t.getName() } -> std::convertible_to<std::string>;
    { t.isUndoable() } -> std::convertible_to<bool>;
};


// Concept for range of data
template <typename T>
concept DataRange = std::ranges::contiguous_range<T> && 
                    requires { typename std::ranges::range_value_t<T>; } &&
                    std::is_trivially_copyable_v<std::ranges::range_value_t<T>>;

// Concept for transformations
template <typename T>
concept Transformation = ImageType<T> && requires(T t, const T& img) {
    { t.apply(img) } -> std::convertible_to<std::unique_ptr<T>>;
    { t.getName() } -> std::convertible_to<std::string>;
};

} // namespace DIPAL


#endif  // DIPAL_CONCEPTS_HPP
