#! /usr/bin/sh
cmake -S . -G "Unix Makefiles" -B build &&
cd build &&
make -j 3
