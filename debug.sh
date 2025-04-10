#!/bin/bash

rm -rf build

mkdir build
cd build

cmake -S .. -DCMAKE_BUILD_TYPE=Debug

cmake --build .

