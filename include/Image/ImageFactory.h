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
