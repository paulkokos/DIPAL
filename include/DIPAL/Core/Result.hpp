// include/DIPAL/Core/Result.hpp
#ifndef DIPAL_RESULT_HPP
#define DIPAL_RESULT_HPP

#include <utility>
#include <optional>
#include <type_traits>
#include <functional>

#include "Error.hpp"
// Note: Error.hpp includes tl/expected.hpp

namespace DIPAL {

// Helper methods for working with Result<T> types

/**
 * @brief Map a successful result to a new type
 * @tparam T Source type
 * @tparam U Target type
 * @param result Source result
 * @param f Mapping function
 * @return Mapped result or propagated error
 */
template <typename T, typename U>
Result<U> map(const Result<T>& result, std::function<U(const T&)> f) {
    if (result) {
        return makeSuccessResult(f(result.value()));
    } else {
        return tl::unexpected(result.error());
    }
}

/**
 * @brief Map a successful result to a new type, with move semantics
 * @tparam T Source type
 * @tparam U Target type
 * @param result Source result
 * @param f Mapping function
 * @return Mapped result or propagated error
 */
template <typename T, typename U>
Result<U> map(Result<T>&& result, std::function<U(T&&)> f) {
    if (result) {
        return makeSuccessResult(f(std::move(result.value())));
    } else {
        return tl::unexpected(result.error());
    }
}

/**
 * @brief Flat-map a successful result to a new result
 * @tparam T Source type
 * @tparam U Target type
 * @param result Source result
 * @param f Mapping function returning a Result
 * @return Mapped result or propagated error
 */

template <typename T, typename U>
Result<U> flatMap(const Result<T>& result, std::function<Result<U>(const T&)> f) {
    if (result) {
        return f(result.value());
    } else {
        return tl::unexpected(result.error());
    }
}

/**
 * @brief Flat-map a successful result to a new result, with move semantics
 * @tparam T Source type
 * @tparam U Target type
 * @param result Source result
 * @param f Mapping function returning a Result
 * @return Mapped result or propagated error
 */
template <typename T, typename U>
Result<U> flatMap(Result<T>&& result, std::function<Result<U>(T&&)> f) {
    if (result) {
        return f(std::move(result.value()));
    } else {
        return tl::unexpected(result.error());
    }
}

/**
 * @brief Apply a function to a successful result
 * @tparam T Result type
 * @param result Source result
 * @param f Function to apply
 * @return The original result
 */
template <typename T>
Result<T> tap(const Result<T>& result, std::function<void(const T&)> f) {
    if (result) {
        f(result.value());
    }
    return result;
}

/**
 * @brief Convert a result to an optional, discarding the error
 * @tparam T Result type
 * @param result Source result
 * @return Optional containing the value or empty if error
 */
template <typename T>
std::optional<T> toOptional(const Result<T>& result) {
    if (result) {
        return result.value();
    } else {
        return std::nullopt;
    }
}

/**
 * @brief Return result value or a fallback if error
 * @tparam T Result type
 * @param result Source result
 * @param fallback Fallback value
 * @return Value or fallback
 */
template <typename T>
T valueOr(const Result<T>& result, const T& fallback) {
    if (result) {
        return result.value();
    } else {
        return fallback;
    }
}

/**
 * @brief Return result value or throw the error
 * @tparam T Result type
 * @param result Source result
 * @return Value if available
 * @throws std::runtime_error with error message if result contains error
 */
template <typename T>
T valueOrThrow(const Result<T>& result) {
    if (result) {
        return result.value();
    } else {
        throw std::runtime_error(result.error().toString());
    }
}

} // namespace DIPAL

#endif // DIPAL_RESULT_HPP
