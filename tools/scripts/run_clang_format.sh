#!/bin/bash
# Apply clang-format to all source files

find $PWD/include $PWD/src -name "*.hpp" -o -name "*.cpp" | xargs clang-format -i
