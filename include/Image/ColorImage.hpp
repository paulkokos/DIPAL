//
// Created by paulk on 12/13/2022.
//

#ifndef DIPAL_COLORIMAGE_HPP
#define DIPAL_COLORIMAGE_HPP


#include "../../include/Image/Image.hpp"
namespace DIPAL {
    class ColorImage : protected Image {
#include "Image.hpp"

class ColorImage : public Image{

    public:
        ColorImage();
    };
}


#endif //DIPAL_COLORIMAGE_HPP
