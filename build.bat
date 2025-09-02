cmake . -B build -DCMAKE_INSTALL_PREFIX=. -G "MinGw Makefiles"
cmake --build build
cmake --install build
