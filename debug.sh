#!/bin/bash

rm -rf Debug

mkdir Debug
cd Debug

cmake -S .. -DCMAKE_BUILD_TYPE=Debug

cmake --build .
