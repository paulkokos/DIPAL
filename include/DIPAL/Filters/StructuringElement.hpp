// include/DIPAL/Filters/StructuringElement.hpp
#ifndef DIPAL_STRUCTURING_ELEMENT_HPP
#define DIPAL_STRUCTURING_ELEMENT_HPP

#include <vector>

namespace DIPAL {

/**
 * @brief Structuring element for morphological operations
 *
 * Stores a 2D binary mask. Width and height must be odd and positive.
 */
class StructuringElement {
public:
    enum class Shape { Rect, Ellipse, Cross, Diamond };

    /** Construct from an explicit boolean mask (row-major, width*height entries). */
    StructuringElement(int width, int height, std::vector<bool> mask);

    [[nodiscard]] int  getWidth()      const noexcept { return m_width; }
    [[nodiscard]] int  getHeight()     const noexcept { return m_height; }
    [[nodiscard]] int  getHalfWidth()  const noexcept { return m_width  / 2; }
    [[nodiscard]] int  getHalfHeight() const noexcept { return m_height / 2; }

    [[nodiscard]] bool at(int row, int col) const noexcept {
        return m_mask[static_cast<size_t>(row * m_width + col)];
    }

    // -------------------------------------------------------------------------
    // Factory methods
    // -------------------------------------------------------------------------

    /** Filled rectangle */
    [[nodiscard]] static StructuringElement rect(int width, int height);

    /** Filled ellipse inscribed in width × height */
    [[nodiscard]] static StructuringElement ellipse(int width, int height);

    /** Plus-shaped cross of given size (must be odd) */
    [[nodiscard]] static StructuringElement cross(int size);

    /** Diamond (rhombus) of given size (must be odd) */
    [[nodiscard]] static StructuringElement diamond(int size);

    /** 3×3 rect — the most common default */
    [[nodiscard]] static StructuringElement defaultElement();

private:
    int              m_width;
    int              m_height;
    std::vector<bool> m_mask;
};

} // namespace DIPAL

#endif // DIPAL_STRUCTURING_ELEMENT_HPP
