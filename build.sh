#!/bin/bash

mkdir build
cd ./build
cmake .. -DCMAKE_BUILD_TYPE=Debug
make
mv source/tetris/tetris ../
