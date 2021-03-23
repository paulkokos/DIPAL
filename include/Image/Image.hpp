//
// Created by paulkokos on 11/27/2020.
//

#ifndef DIPAL_IMAGE_H
#define DIPAL_IMAGE_H

#include <string>
#include <chrono>
#include <ctime>
#include <vector>
#include "../CalendarDate.h"
#include "../CalendarTime.h"

using std::iterator;
using std::vector;

using std::string;
class Image {
private:
    int height;
    int width;
    int bitDepth;
    unsigned int fileSize;
    int minimumIntensity;
    int maximumIntensity;
    string imageClass ;
    string name;
    string format;
    string imageType;
    string colorType;
    vector<int> formatSignature;
    vector<int>::iterator iterator;
    struct creationDate {
        CalendarDate date;
        CalendarTime time;
    };



public:
    Image();
    Image(int height, int width, int colorDepth, string name,string extension);

    int getHeight() const;

    int getWidth() const;

    int getColorDepth() const;

    void setColorDepth(int colorDepth);

    const string &getName() const;

    const string &getExtension() const;
};


#endif //DIPAL_IMAGE_H
