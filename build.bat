cmake . -B build -DCMAKE_INSTALL_PREFIX=. -G "MinGW Makefiles"
cmake --build build
cmake --install build
