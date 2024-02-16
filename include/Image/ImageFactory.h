//
// Created by paulk on 2/17/2024.
//

#ifndef DIPAL_IMAGEFACTORY_H
#define DIPAL_IMAGEFACTORY_H
#include "Image.hpp"
#include "ImageTypes.h"


namespace DIPAL {
    class ImageFactory {
    public:
        Image *build(ImageTypes imgType);
    };
}

#endif //DIPAL_IMAGEFACTORY_H
