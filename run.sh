#!/bin/sh

cd ./build
cmake ..
make
./runTests
./pricer