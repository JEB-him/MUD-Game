#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#ifdef _WIN32
// 在main函数之前设置locale
#include <clocale>

struct SetupLocale {
    SetupLocale() {
        std::setlocale(LC_ALL, ".UTF-8");
    }
};

SetupLocale setupLocale;

#endif