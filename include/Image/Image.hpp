//
// Created by paulkokos on 11/27/2020.
//

#ifndef DIPAL_IMAGE_H
#define DIPAL_IMAGE_H

#include <string>
#include <chrono>
#include <ctime>
#include <vector>
//#include "Image.hpp"
#include "../CalendarDate.h"
#include "../CalendarTime.h"
#include "Header.h"

using std::iterator;
using std::vector;
using std::string;

class Image {
private:
    Header *header  = new Header();
    int height;
    uint8_t width;
    string name;
    uint8_t bitDepth;
    uint8_t fileSize;
    uint8_t minimumIntensity;
    uint8_t maximumIntensity;
    string imageClass;
    string format;
    string imageType;
    string colorType;
    vector<int> formatSignature;
    vector<int> colorMap;
    vector<int> histogram;
    vector<int>::iterator formatSignatureIterator;
    vector<int>::iterator colorMapIterator;
    vector<int>::iterator histogramIterator;
    struct creationDate {
        CalendarDate date;
        CalendarTime time;
    };
//    struct header {
//
//    };

public:
    Image();

    void imageShow();

    vector<int> getBitmapImageHeader();

    int getHeight() const;

    void setHeight(int height);

    int getWidth() const;

    void setWidth(int width);

    const string &getName() const;

    void setName(const string &name);

    int getBitDepth() const;

    void setBitDepth(int bitDepth);

    unsigned int getFileSize() const;

    void setFileSize(unsigned int fileSize);

    int getMinimumIntensity() const;

    void setMinimumIntensity(int minimumIntensity);

    int getMaximumIntensity() const;

    void setMaximumIntensity(int maximumIntensity);

    const string &getImageClass() const;

    void setImageClass(const string &imageClass);

    const string &getFormat() const;

    void setFormat(const string &format);

    const string &getImageType() const;

    void setImageType(const string &imageType);

    const string &getColorType() const;

    void setColorType(const string &colorType);

    const vector<int> &getFormatSignature() const;

    void setFormatSignature(const vector<int> &formatSignature);

    const vector<int> &getColorMap() const;

    void setColorMap(const vector<int> &colorMap);

    const vector<int> &getHistogram() const;

    void setHistogram(const vector<int> &histogram);

    const vector<int>::iterator &getFormatSignatureIterator() const;

    void setFormatSignatureIterator(const vector<int>::iterator &formatSignatureIterator);

    const vector<int>::iterator &getColorMapIterator() const;

    void setColorMapIterator(const vector<int>::iterator &colorMapIterator);

    const vector<int>::iterator &getHistogramIterator() const;

    void setHistogramIterator(const vector<int>::iterator &histogramIterator);

    FILE imageRead(std::string location, std::string fileType);

    std::string imageWrite(std::string name, std::string location, std::string fileType);

};

#endif //DIPAL_IMAGE_H
