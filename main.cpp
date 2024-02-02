#include <iostream>
#include "include/image/Image.hpp"
#include "include/Image/BinaryImage.hpp"

using namespace DIPAL;
int main() {
    Image img;
    img.helloWorld();
    Image *img2 = new BinaryImage();
    img2->helloWorld();
//    img.helloWorld();
//    img.imageShow();
    delete img2;







//    img.helloWorld();
//    std::cout << "Hello !!!" << std::endl;
    return 0;
}
