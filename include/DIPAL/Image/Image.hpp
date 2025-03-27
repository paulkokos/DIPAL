#ifndef DIPAL_IMAGE_HPP
#define DIPAL_IMAGE_HPP

#include <cstdint>
#include <memory>
#include <string>
#include <vector>
#include <span>

namespace DIPAL {
// Forward declarations
class ImageData;
/*
 * @brief Base class for all image types in DIPAL
 *
 * Provides common interface and functionality for working with different
 * images types like binary ,grayscale , and color images.
 *
 */
// Base image class
class Image {
public:
    // Image types
    enum class Type { Binary, Grayscale, RGB, RGBA };

    // Rule of five (virtual destructor and default others)
    virtual ~Image() = default;
    Image(const Image&) = default;
    Image& operator=(const Image&) = default;
    Image(Image&&) = default;
    Image& operator=(Image&&) = default;

    /*
     * @brief Create a new empty image
     * @param width Image width in pixels
     * @param height Image height in pixels
     * @param type Image type
     */

    Image(int width, int height, Type type);

    /*
     * @brief Get the width of the image
     * @return Width in pixels
     */

    int getWidth() const;

    /*
     * @brief get the Height if the image
     * @return Height in pixels
     */
    int getHeight() const ;

    /**
     * @brief Get the type of the image
     * @return Image type
     */

    [[nodiscard]] Type getType() const;

    /**
     * @brief Check if the image is empty
     * @return true if the image is empty, false otherwise
     */

    [[nodiscard]] bool isEmpty() const;

    /**
     * @brief Get a pointer to the image data
     * @return Raw pointer to the beginning of the image
     */

    [[nodiscard]] const uint8_t* getData() const;
    /*
     * @brief Get a modifiable pointer to the image data
     * return Raw pointer to the beginning of the image
     */

    [[nodiscard]] uint8_t* getData();
    /**
     * @brief Get the number of channels in the image
     * @return number of channels (1 for Grayscale,3 for RGB, 4 for RGBA)
     */

    [[nodiscard]] int getChannels() const;

    /**
     * @brief Get the number of bytes per pixel
     * @return Bytes per pixel
     */

    [[nodiscard]] int getBytesPerPixel() const;

    /**
     * @brief Get the total size of the image data in bytes
     * @return Size in bytes
     */

    [[nodiscard]] size_t getDataSize() const;

    /**
     * @brief Clone the image
     * @return A new image that is a deep copy of this image
     */

    virtual std::unique_ptr<Image> clone() const;

        /**
     * @brief Check if a pixel coordinate is valid
     * @param x X coordinate
     * @param y Y coordinate
     * @return true if the coordinate is valid, false otherwise
     */
    [[nodiscard]] bool isValidCoordinate(int x, int y) const ;
    
    [[nodiscard]] std::span<const std::uint8_t> getDataSpan() const noexcept;
    
    [[nodiscard]] std::span<std::uint8_t> getDataSpan();

    std::string toString() const; 
    
    std::size_t getPixelIndex(int x, int y) const;
protected:
    int m_width;
    int m_height;
    Type m_type;
    std::vector<uint8_t> m_data;
    int m_channels;
    int m_bytesPerPixel;

    // Protected constructor for derived classes
    Image();
};
}  // namespace DIPAL


#endif  // DIPAL_IMAGE_HPP
