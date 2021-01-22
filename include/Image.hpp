//
// Created by paulkokos on 11/27/2020.
//

#ifndef DIPAL_IMAGE_H
#define DIPAL_IMAGE_H

#include <string>
using std::string;
class Image {
private:
    int height;
    int width;
    int colorDepth;
    string name;
    string extension;
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
