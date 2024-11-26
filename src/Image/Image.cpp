#include <iostream>
#include <fstream>
#include "../../include/Image/Image.hpp"


namespace DIPAL{
  Image::Image(){

  }

  void Image::Header::helloWorld() {
    std::cout << "Hello world from header\n";
  }

  Image::Header::~Header() {
    std::cout << "Good bye from Header\n";
  }

  Image::Header::Header() {
    std::cout << "Header just created\n";
    //Header header1;
    std::cout << "Hello world from the class image\n";

  }

  Image::~Image() {
    //        delete ;
    std::cout << "Good bye world from the class image\n";

  }
}
