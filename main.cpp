#include <iostream>
#include "include/Image/Image.hpp"
#include "include/image/GrayscaleImage.hpp"
#include "include/Image/BinaryImage.hpp"
#include "include/Image/ImageFactory.h"
#include "include/Image/ImageTypes.h"
using namespace DIPAL;
//Header* Header::headerPtr = nullptr;
int main() {
    Image *img = new BinaryImage();
    img->imShow();
    ImageFactory* img1 ;
//    img1->build(ImageTypes::IM_Binary);

    return 0;

}