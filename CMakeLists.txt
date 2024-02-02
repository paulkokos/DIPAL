cmake_minimum_required(VERSION 3.16)
project(DIPAL)
set(CMAKE_CXX_STANDARD 17)

add_executable(DIPAL   src/DIPAL/DIPAL.cpp  src/Image/Image.cpp
        src/Calendar/CalendarDate.cpp src/Calendar/CalendarTime.cpp
        src/Image/Header.cpp src/Image/BinaryImage.cpp src/Image/ImageFactory.cpp
        src/Image/Header.cpp src/Image/GrayscaleImage.cpp
        src/Image/ColorImage.cpp

        include/DIPAL/ImgProc.hpp include/Image/Image.hpp
        include/Calendar/CalendarDate.h include/Calendar/CalendarTime.h
        include/Image/Header.h include/Image/BinaryImage.hpp include/Image/ImageFactory.h



        include/Image/Header.h include/Image/GrayscaleImage.hpp
        include/Image/ColorImage.hpp


        main.cpp)
#add_executable(Main main.cpp)