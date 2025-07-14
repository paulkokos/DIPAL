// include/DIPAL/Color/Concepts.hpp
#ifndef DIPAL_CONCEPTS_HPP
#define DIPAL_CONCEPTS_HPP

#include <cstdint>
#include <iostream>
#include <memory>
#include <ranges>
#include <span>
#include <string>
#include <type_traits>

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
template <typename T, typename ImageT>
concept Filter = requires(T filter, const T& img) {
    requires ImageType<ImageT>;

    { filter.apply(img) } -> std::convertible_to<std::unique_ptr<T>>;

    { filter.getName() } -> std::convertible_to<std::string>;
};

// Concept for processing commands
template <typename T, typename ImageT>
concept Command = requires(T cmd, const ImageT& img) {
    requires ImageType<ImageT>;
    { cmd.execute(img) } -> std::convertible_to<std::unique_ptr<ImageT>>;
    { cmd.getName() } -> std::convertible_to<std::string>;
    { cmd.isUndoable() } -> std::convertible_to<bool>;
};

// Concept for range of data that can be used in image processing
template <typename T>
concept DataRange = std::ranges::contiguous_range<T> && requires {
    typename std::ranges::range_value_t<T>;
} && std::is_trivially_copyable_v<std::ranges::range_value_t<T>>;

// Concept for geometric transformations (rotation, scaling etc)
template <typename T, typename ImageT>
concept Transformation = requires(T transform, const ImageT& img) {
    requires ImageType<ImageT>;
    { transform.apply(img) } -> std::convertible_to<std::unique_ptr<ImageT>>;
    { transform.getName() } -> std::convertible_to<std::string>;
};

}  // namespace DIPAL

#endif  // DIPAL_CONCEPTS_HPP
