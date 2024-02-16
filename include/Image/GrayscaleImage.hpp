//
// Created by paulkokos on 23/1/21.
//

#ifndef DIPAL_GRAYSCALEIMAGE_HPP
#define DIPAL_GRAYSCALEIMAGE_HPP


#include "Image.hpp"
#include "ColorImage.hpp"
namespace DIPAL {
    class GrayscaleImage :  public Image {
    public:
        GrayscaleImage();

        Image RGB2Gray(ColorImage image);

        void imRead(std::string fileName);
    };
}

#endif //DIPAL_GRAYSCALEIMAGE_HPP
