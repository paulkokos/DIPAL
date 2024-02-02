//
// Created by paulk on 12/13/2022.
//

#ifndef DIPAL_BINARYIMAGE_HPP
#define DIPAL_BINARYIMAGE_HPP
#include <iostream>
#include "Image.hpp"
namespace DIPAL {
    class BinaryImage : protected Image {
        BinaryImage(std::uint64_t *height, std::uint64_t *width, std::uint8_t *colorDepth, std::string *fileName);

    };
}

#endif //DIPAL_BINARYIMAGE_HPP
