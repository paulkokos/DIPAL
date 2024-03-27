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

namespace DIPAL {
    class Image {
        class Header {
        public:
            unsigned char getSignatureB() const;

            void setSignatureB(unsigned char signatureB);

            unsigned char getSignatureM() const;

            void setSignatureM(unsigned char signatureM);

            uint8_t getReserved1() const;

            void setReserved1(uint8_t reserved1);

            uint8_t getReserved2() const;

            void setReserved2(uint8_t reserved2);

            unsigned int getBitmapInfoHeader() const;

            void setBitmapInfoHeader(unsigned int bitmapInfoHeader);

            unsigned int getWidth() const;

            void setWidth(unsigned int width);

            unsigned int getHeight() const;

            void setHeight(unsigned int height);

            uint8_t getPlanes() const;

            void setPlanes(uint8_t planes);

            unsigned int getOffsetImageStart() const;

            void setOffsetImageStart(unsigned int offsetImageStart);

            uint8_t getBitDepth() const;

            void setBitDepth(uint8_t bitDepth);

            unsigned int getFileSize() const;

            void setFileSize(unsigned int fileSize);

            unsigned int getCompressionType() const;

            void setCompressionType(unsigned int compressionType);

            unsigned int getSizeOfData() const;

            void setSizeOfData(unsigned int sizeOfData);

            unsigned int getHorizontalResolution() const;

            void setHorizontalResolution(unsigned int horizontalResolution);

            unsigned int getVerticalResolution() const;

            void setVerticalResolution(unsigned int verticalResolution);

            unsigned int getNumberOfColors() const;

            void setNumberOfColors(unsigned int numberOfColors);

            unsigned int getNumberOfImportantColors() const;

            void setNumberOfImportantColors(unsigned int numberOfImportantColors);

        private:
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
            Header();
            ~Header();
        };
    public:
        Image();
        Image(std::string locationFoFile,std::string nameOfFile);
        ~Image();
    private:
        Header *header;
        std::string nameOfFile;
        std::string locationOfFile;
    };
}
#endif //DIPAL_IMAGE_H
