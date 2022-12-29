//
// Created by paulk on 12/29/2022.
//

#ifndef DIPAL_DIPAL_HPP
#define DIPAL_DIPAL_HPP

#include "Color/Color.hpp"
#include "Compression/Compression.hpp"
class DIPAL :protected Color,Compression{
private:
    Compression compression;
};


#endif //DIPAL_DIPAL_HPP
