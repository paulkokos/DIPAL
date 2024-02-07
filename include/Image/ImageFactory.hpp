//
// Created by paulkokos on 2/2/2024.
//

#ifndef DIPAL_IMAGEFACTORY_H
#define DIPAL_IMAGEFACTORY_H

#include "Image.hpp"

namespace DIPAL {
    class ImageFactory {
    public:
        virtual Image *createImage() = 0;
        virtual  ~ImageFactory() {}
    };
}

#endif //DIPAL_IMAGEFACTORY_H
