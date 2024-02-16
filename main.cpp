#include <iostream>
#include "include/image/Image.hpp"
#include "include/image/GrayscaleImage.hpp"
#include "include/Image/BinaryImage.hpp"
using namespace DIPAL;
//Header* Header::headerPtr = nullptr;
int main() {

    //
    ImageCreator* img2 = new ColorImage();
    img2->imShow();
    img2->helloWorld();

    //
    ImageCreator* img1 = new BinaryImage();
    img1->imShow();
    img1->helloWorld();
    //
    ImageCreator* img = new GrayscaleImage();
    img->imShow();
    img->imShow();
    return 0;
}