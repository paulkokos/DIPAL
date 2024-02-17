#include <iostream>
#include "include/Image/Image.hpp"
#include "include/Image/BinaryImage.hpp"
#include "include/Image/ImageFactory.h"
#include "include/Image/ImageTypes.h"
using namespace DIPAL;
//Header* Header::headerPtr = nullptr;
int main() {
//    Image *img = new BinaryImage();
//    img->imShow();
//    ImageFactory* img1 ;
//    ImageFactory* img2 ;
//    ImageFactory* img3 ;
//    ImageFactory* img4 ;
//    ImageFactory* img5 ;
//    img1->build(ImageTypes::IM_Binary);
//    img2->build(ImageTypes::IM_Binary);
//    img3->build(ImageTypes::IM_Binary);
//    img4->build(ImageTypes::IM_Binary);
//    img5->build(ImageTypes::IM_Binary);
    Image *i = new BinaryImage();
    i->imShow();
//    i->helloWorld();

    return 0;

}