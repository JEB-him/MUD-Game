#!/usr/bin/bash
cmake . -B build -DCMAKE_EXPORT_COMPILE_COMMANDS=1 -DCMAKE_INSTALL_PREFIX=./
cmake --build build
cmake --install build
