#include "include/Image/Image.hpp"
using namespace DIPAL;
int main() {
  Image *img = new Image();
  img->headerHelloWorld();
  delete img;
  return 0;
}
