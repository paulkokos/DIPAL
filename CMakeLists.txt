cmake_minimum_required(VERSION 3.16)
project(DIPAL)
set(CMAKE_CXX_STANDARD 17)

add_library(DIPAL STATIC  src/DIPAL/DIPAL.cpp  src/Image/Image.cpp
        src/Calendar/CalendarDate.cpp src/Calendar/CalendarTime.cpp

        include/DIPAL/ImgProc.hpp include/Image/Image.hpp
        include/Calendar/CalendarDate.h include/Calendar/CalendarTime.h


        main.cpp)
#add_executable(Main main.cpp main1.cpp)