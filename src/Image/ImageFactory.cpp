//
// Created by paulk on 2/17/2024.
//

#include "../../include/Image/ImageFactory.h"
#include "../../include/Image/BinaryImage.hpp"
#include "../../include/Image/ColorImage.hpp"
#include "../../include/Image/GrayscaleImage.hpp"

namespace DIPAL{

    Image *ImageFactory::build(ImageTypes imgType){
        if(imgType == IM_Binary) {
            return new BinaryImage();
        } else if (imgType == IM_Color){
            return new ColorImage();
        } else if (imgType == IM_Grayscale) {              //<<--- Not a good tactic
            return new GrayscaleImage();
        } else {
            return nullptr;
        }
    }


}