// include/DIPAL/Transformation/Interpolation.hpp
#ifndef DIPAL_INTERPOLATION_HPP
#define DIPAL_INTERPOLATION_HPP

#include "../Core/Error.hpp"
#include "../Image/ColorImage.hpp"
#include "../Image/GrayscaleImage.hpp"
#include "../Image/Image.hpp"
#include "Transformations.hpp"  // Include for InterpolationMethod enum

#include <array>
#include <cstdint>
#include <functional>

namespace DIPAL {

/**
 * @brief Static class providing interpolation functions for image transformations
 */
class Interpolation {
public:
    /**
     * @brief Interpolate grayscale value at a floating-point position
     * @param image Source grayscale image
     * @param x X coordinate (can be fractional)
     * @param y Y coordinate (can be fractional)
     * @param method Interpolation method to use
     * @return Result containing the interpolated pixel value or error
     */
    static Result<uint8_t> interpolateGray(
        const GrayscaleImage& image,
        float x,
        float y,
        InterpolationMethod method = InterpolationMethod::Bilinear);

    /**
     * @brief Interpolate color value at a floating-point position
     * @param image Source color image
     * @param x X coordinate (can be fractional)
     * @param y Y coordinate (can be fractional)
     * @param r Reference to store interpolated red value
     * @param g Reference to store interpolated green value
     * @param b Reference to store interpolated blue value
     * @param a Reference to store interpolated alpha value
     * @param method Interpolation method to use
     * @return Result indicating success or error
     */
    static VoidResult interpolateColor(const ColorImage& image,
                                       float x,
                                       float y,
                                       uint8_t& r,
                                       uint8_t& g,
                                       uint8_t& b,
                                       uint8_t& a,
                                       InterpolationMethod method = InterpolationMethod::Bilinear);

    /**
     * @brief Create a mapping function for coordinate transformation
     * @param srcWidth Source image width
     * @param srcHeight Source image height
     * @param dstWidth Destination image width
     * @param dstHeight Destination image height
     * @param transformFunc Function that maps destination coordinates to source coordinates
     * @return A function that takes (x,y) in destination space and returns (x,y) in source space
     */
    static std::function<std::pair<float, float>(int, int)> createMapping(
        int srcWidth,
        int srcHeight,
        int dstWidth,
        int dstHeight,
        std::function<std::pair<float, float>(float, float, int, int, int, int)> transformFunc);

private:
    // Private helper methods for different interpolation types

    /**
     * @brief Nearest neighbor interpolation for grayscale images
     * @param image Source grayscale image
     * @param x X coordinate (can be fractional)
     * @param y Y coordinate (can be fractional)
     * @return Result containing the interpolated pixel value or error
     */
    static Result<uint8_t> nearestNeighborGray(const GrayscaleImage& image, float x, float y);

    /**
     * @brief Bilinear interpolation for grayscale images
     * @param image Source grayscale image
     * @param x X coordinate (can be fractional)
     * @param y Y coordinate (can be fractional)
     * @return Result containing the interpolated pixel value or error
     */
    static Result<uint8_t> bilinearGray(const GrayscaleImage& image, float x, float y);

    /**
     * @brief Bicubic interpolation for grayscale images
     * @param image Source grayscale image
     * @param x X coordinate (can be fractional)
     * @param y Y coordinate (can be fractional)
     * @return Result containing the interpolated pixel value or error
     */
    static Result<uint8_t> bicubicGray(const GrayscaleImage& image, float x, float y);

    /**
     * @brief Nearest neighbor interpolation for color images
     * @param image Source color image
     * @param x X coordinate (can be fractional)
     * @param y Y coordinate (can be fractional)
     * @param r Reference to store interpolated red value
     * @param g Reference to store interpolated green value
     * @param b Reference to store interpolated blue value
     * @param a Reference to store interpolated alpha value
     * @return Result indicating success or error
     */
    static VoidResult nearestNeighborColor(const ColorImage& image,
                                           float x,
                                           float y,
                                           uint8_t& r,
                                           uint8_t& g,
                                           uint8_t& b,
                                           uint8_t& a);

    /**
     * @brief Bilinear interpolation for color images
     * @param image Source color image
     * @param x X coordinate (can be fractional)
     * @param y Y coordinate (can be fractional)
     * @param r Reference to store interpolated red value
     * @param g Reference to store interpolated green value
     * @param b Reference to store interpolated blue value
     * @param a Reference to store interpolated alpha value
     * @return Result indicating success or error
     */
    static VoidResult bilinearColor(const ColorImage& image,
                                    float x,
                                    float y,
                                    uint8_t& r,
                                    uint8_t& g,
                                    uint8_t& b,
                                    uint8_t& a);

    /**
     * @brief Bicubic interpolation for color images
     * @param image Source color image
     * @param x X coordinate (can be fractional)
     * @param y Y coordinate (can be fractional)
     * @param r Reference to store interpolated red value
     * @param g Reference to store interpolated green value
     * @param b Reference to store interpolated blue value
     * @param a Reference to store interpolated alpha value
     * @return Result indicating success or error
     */
    static VoidResult bicubicColor(const ColorImage& image,
                                   float x,
                                   float y,
                                   uint8_t& r,
                                   uint8_t& g,
                                   uint8_t& b,
                                   uint8_t& a);

    /**
     * @brief Cubic interpolation helper function for a single channel
     * @param p Array of 4 values to interpolate between
     * @param t Fractional position [0,1]
     * @return Interpolated value
     */
    static float cubicInterpolate(const std::array<float, 4>& p, float t);

    /**
     * @brief Bicubic interpolation helper function for a single channel
     * @param p 4x4 grid of values
     * @param x Fractional x position [0,1]
     * @param y Fractional y position [0,1]
     * @return Interpolated value
     */
    static float bicubicInterpolate(const std::array<std::array<float, 4>, 4>& p, float x, float y);

    /**
     * @brief Get a safe pixel value from grayscale image, with boundary handling
     * @param image Source grayscale image
     * @param x X coordinate (integer)
     * @param y Y coordinate (integer)
     * @return Pixel value (returns 0 for out-of-bounds coordinates)
     */
    static uint8_t getPixelSafe(const GrayscaleImage& image, int x, int y);

    /**
     * @brief Get a safe pixel value from color image, with boundary handling
     * @param image Source color image
     * @param x X coordinate (integer)
     * @param y Y coordinate (integer)
     * @param r Reference to store red value
     * @param g Reference to store green value
     * @param b Reference to store blue value
     * @param a Reference to store alpha value
     */
    static void getPixelSafe(const ColorImage& image,
                             int x,
                             int y,
                             uint8_t& r,
                             uint8_t& g,
                             uint8_t& b,
                             uint8_t& a);
};

}  // namespace DIPAL

#endif  // DIPAL_INTERPOLATION_HPP
