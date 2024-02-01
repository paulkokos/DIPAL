#include <iostream>
#include "include/image/Image.hpp"
using namespace DIPAL;
int main() {
    Image img;
    img.helloWorld();
    Image *img2 = new Image();
    img.helloWorld();
    img.imageShow();
    delete img2;






//    img.helloWorld();
//    std::cout << "Hello !!!" << std::endl;
    return 0;
}
