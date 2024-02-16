//
// Created by paulk on 12/13/2022.
//

#ifndef DIPAL_BINARYIMAGE_HPP
#define DIPAL_BINARYIMAGE_HPP
#include <iostream>
#include "ImageFactory.hpp"

namespace DIPAL {
    class BinaryImage : public ImageFactory {
    public:
        BinaryImage(std::uint64_t *height, std::uint64_t *width, std::uint8_t *colorDepth, std::string *fileName);


        BinaryImage();

        void imShow() const override{
            std::cout << "Hello from imshow() from Binary\n";
        }
    };
}

#endif //DIPAL_BINARYIMAGE_HPP
