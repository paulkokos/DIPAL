//
// Created by paulkokos on 1/27/2024.
//

#ifndef DIPAL_HEADER_H
#define DIPAL_HEADER_H


#include <cstdint>
#include <iostream>
#include <string>
#include <vector>


class Header {
private:
    int height;
    uint8_t width;
    std::string name;
    uint8_t bitDepth;
    uint8_t fileSize;
    uint8_t minimumIntensity;
    uint8_t maximumIntensity;
    std::string imageClass;
    std::string format;
    std::string imageType;
    std::string colorType;
    std::vector<int> formatSignature;
    std::vector<int> colorMap;
    std::vector<int> histogram;
    std::vector<int>::iterator formatSignatureIterator;
    std::vector<int>::iterator colorMapIterator;
    std::vector<int>::iterator histogramIterator;
//    struct creationDate {
//        CalendarDate date;
//        CalendarTime time;
//    };
    static Header* geaderPtr;
    Header();

public:
    static Header* getInstance();
    Header(const Header& obj)= delete;

    void setValues();
    void print();

};


#endif //DIPAL_HEADER_H
