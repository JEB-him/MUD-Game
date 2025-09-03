#define CATCH_CONFIG_MAIN

#include "catch.hpp"
#include <vector>
#include <iostream>
using std::cin, std::cout, std::endl;

int test(int a, int b) {
    return a + b;
}

int test2(int n) {
    if(n == 0) return 0;
    return n == 1 ? 1 : test2(n - 1) * n;
}

TEST_CASE("能够获得主角的正确位置", "[map]") {
    std::vector<int> my_vector(10);
    REQUIRE(test(10, 2) == 12);
    REQUIRE(test2(1) == 1);
    REQUIRE(test2(2) == 2);
    REQUIRE(test2(0) == 1);
}
