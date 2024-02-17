//
// Created by paulk on 12/13/2022.
//

#ifndef DIPAL_BINARYIMAGE_HPP
#define DIPAL_BINARYIMAGE_HPP
#include <iostream>
#include "Image.hpp"

namespace DIPAL {
    class BinaryImage : public Image {
    public:
//        BinaryImage(std::uint64_t *height, std::uint64_t *width, std::uint8_t *colorDepth, std::string *fileName);


        BinaryImage();
        std::string imShow() const override;

    };
}

#endif //DIPAL_BINARYIMAGE_HPP
