//
// Created by paulkokos on 1/27/2024.
//

#ifndef DIPAL_HEADER_HPP
#define DIPAL_HEADER_HPP


#include <cstdint>
#include <iostream>
#include <string>
#include <vector>


namespace DIPAL {
    class Header {
    private:

        //Header Variables
        unsigned char signature_B;       //'B' signature (1 byte)
        unsigned char signature_M;    //'M' signature (1 byte)
        uint8_t reserved1;
        uint8_t reserved2;                  // reserved 2+2 bytes (plz ignore)
        unsigned int bitmapInfoHeader;    //size of info header (must be 40)
        unsigned int width;                 //4 bytes in header
        unsigned int height;             //4 bytes in header
        uint8_t planes;                     //number of planes in the image (must be 1)
        unsigned int offsetImageStart;    //4 bytes
        uint8_t bitDepth;                 //in header 2 bytes
        unsigned int fileSize;            //in header 4 bytes (unreliable)
        unsigned int compressionType;      //compression type (0=none,1=RLE-8,2=RLE-4)
        unsigned int sizeOfData;           //Size of image data in bytes (including padding)
        unsigned int horizontalResolution; //Unreliable
        unsigned int verticalResolution;    //Unreliable
        unsigned int numberOfColors;        //number or zero
        unsigned int numberOfImportantColors; //number or zero

        static Header *headerPtr;

        Header();

        ~Header();

    public:
        static Header &getHeaderInstance();

        Header(const Header &) = delete;

        Header operator=(const Header &) = delete;
    };
}

#endif //DIPAL_HEADER_HPP