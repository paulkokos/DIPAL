// include/DIPAL/Core/Iterators.hpp
#ifndef DIPAL_ITERATORS_HPP
#define DIPAL_ITERATORS_HPP

#include <iterator>
#include <cstdint>
#include <span>

namespace DIPAL {

/**
 * @brief Pixel iterator for grayscale images
 * 
 * Allows iteration over pixels in a grayscale image
 */
class GrayscalePixelIterator {
public:
    // Iterator traits
    using iterator_category = std::random_access_iterator_tag;
    using value_type = uint8_t;
    using difference_type = std::ptrdiff_t;
    using pointer = uint8_t*;
    using reference = uint8_t&;
    
    /**
     * @brief Create a pixel iterator
     * @param data Pointer to pixel data
     * @param stride Row stride in bytes
     */
    GrayscalePixelIterator(pointer data, size_t stride = 0)
        : m_data(data), m_stride(stride) {}
    
    // Iterator operations
    reference operator*() const { return *m_data; }
    pointer operator->() const { return m_data; }
    
    GrayscalePixelIterator& operator++() {
        ++m_data;
        return *this;
    }
    
    GrayscalePixelIterator operator++(int) {
        GrayscalePixelIterator tmp = *this;
        ++(*this);
        return tmp;
    }
    
    GrayscalePixelIterator& operator--() {
        --m_data;
        return *this;
    }
    
    GrayscalePixelIterator operator--(int) {
        GrayscalePixelIterator tmp = *this;
        --(*this);
        return tmp;
    }
    
    GrayscalePixelIterator& operator+=(difference_type n) {
        m_data += n;
        return *this;
    }
    
    GrayscalePixelIterator operator+(difference_type n) const {
        GrayscalePixelIterator tmp = *this;
        return tmp += n;
    }
    
    GrayscalePixelIterator& operator-=(difference_type n) {
        return *this += -n;
    }
    
    GrayscalePixelIterator operator-(difference_type n) const {
        GrayscalePixelIterator tmp = *this;
        return tmp -= n;
    }
    
    difference_type operator-(const GrayscalePixelIterator& other) const {
        return m_data - other.m_data;
    }
    
    reference operator[](difference_type n) const {
        return *(m_data + n);
    }
    
    bool operator==(const GrayscalePixelIterator& other) const {
        return m_data == other.m_data;
    }
    
    bool operator!=(const GrayscalePixelIterator& other) const {
        return !(*this == other);
    }
    
    bool operator<(const GrayscalePixelIterator& other) const {
        return m_data < other.m_data;
    }
    
    bool operator>(const GrayscalePixelIterator& other) const {
        return other < *this;
    }
    
    bool operator<=(const GrayscalePixelIterator& other) const {
        return !(other < *this);
    }
    
    bool operator>=(const GrayscalePixelIterator& other) const {
        return !(*this < other);
    }
    
    /**
     * @brief Move to the next row
     * @return Reference to this iterator
     */
    GrayscalePixelIterator& nextRow() {
        if (m_stride > 0) {
            m_data += m_stride;
        }
        return *this;
    }
    
private:
    pointer m_data;
    size_t m_stride;
};

/**
 * @brief RGB pixel iterator
 * 
 * Provides iteration over RGB pixel data
 */
class RGBPixelIterator {
public:
    // Define RGB pixel structure for iteration
    struct RGBPixel {
        uint8_t r;
        uint8_t g;
        uint8_t b;
    };
    
    // Iterator traits
    using iterator_category = std::random_access_iterator_tag;
    using value_type = RGBPixel;
    using difference_type = std::ptrdiff_t;
    using pointer = RGBPixel*;
    using reference = RGBPixel&;
    
    /**
     * @brief Create a RGB pixel iterator
     * @param data Pointer to pixel data
     * @param stride Row stride in bytes
     */
    RGBPixelIterator(uint8_t* data, size_t stride = 0)
        : m_data(reinterpret_cast<pointer>(data)), m_stride(stride / sizeof(RGBPixel)) {}
    
    // Iterator operations (similar to grayscale iterator)
    reference operator*() const { return *m_data; }
    pointer operator->() const { return m_data; }
    
    RGBPixelIterator& operator++() {
        ++m_data;
        return *this;
    }
    
    RGBPixelIterator operator++(int) {
        RGBPixelIterator tmp = *this;
        ++(*this);
        return tmp;
    }
    
    // Additional methods similar to GrayscalePixelIterator...
    
    /**
     * @brief Move to the next row
     * @return Reference to this iterator
     */
    RGBPixelIterator& nextRow() {
        if (m_stride > 0) {
            m_data += m_stride;
        }
        return *this;
    }
    
private:
    pointer m_data;
    size_t m_stride;
};

} // namespace DIPAL

#endif // DIPAL_ITERATORS_HPP
