//
// Created by paulkokos on 11/27/2020.
//

#ifndef DIPAL_IMAGE_H
#define DIPAL_IMAGE_H

#include <string>
#include <chrono>
#include <ctime>
#include <vector>

//#include "../../include/Calendar/CalendarDate.h"
//#include "../../include/Calendar/CalendarTime.h"



//struct DefaultVariables {
//    unsigned char SIGNATURE_B = 'B';
//    unsigned char SIGNATURE_M = 'M';
//    uint8_t RESERVED1;
//    uint8_t RESERVED2;
//    const static unsigned int BITMAPINFOHEADER = 40;
//    const static unsigned int WIDTH = 512;
//    const static unsigned int HEIGHT = 512;
//    const static uint8_t PLAINS = 1;
//    const static unsigned int OFFSETIMAGESTART = 1024;
//    const static unsigned int BITDEPTH = 24;
//    const static unsigned int SIZEOFDATA=0;
//};

//const int COLOR_LAYERS = 3;         //red, green,blue
//const int FILE_HEADER_SIZE = 14;
//const int INFO_HEADER_SIZE = 40;


using std::iterator;
using std::vector;
using std::string;
namespace DIPAL {
    class Image {
    private:
        class Header {
        private:
            Header();

            unsigned char signature_B;              //'B' signature (1 byte)
            unsigned char signature_M;              //'M' signature (1 byte)
            uint8_t reserved1;
            uint8_t reserved2;                      // reserved 2+2 bytes (plz ignore)
            unsigned int bitmapInfoHeader;          //size of info header (must be 40)
            unsigned int width;                     //4 bytes in header
            unsigned int height;                    //4 bytes in header
            uint8_t planes;                         //number of planes in the image (must be 1)
            unsigned int offsetImageStart;          //4 bytes
            uint8_t bitDepth;                       //in header 2 bytes
            unsigned int fileSize;                  //in header 4 bytes (unreliable)
            unsigned int compressionType;           //compression type (0=none,1=RLE-8,2=RLE-4)
            unsigned int sizeOfData;                //Size of image data in bytes (including padding)
            unsigned int horizontalResolution;      //Unreliable
            unsigned int verticalResolution;        //Unreliable
            unsigned int numberOfColors;            //number or zero
            unsigned int numberOfImportantColors;   //number or zero
        public:
            static Header& getInstance();

            ~Header();
        };
    public:
        Image();

        ~Image();

    };
}
#endif //DIPAL_IMAGE_H
