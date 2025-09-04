#include <iostream>
#include "catch.hpp"
#include "tools.h"

TEST_CASE("Positioin can be set and modified", "[tools][position]") {
    Position pos1(1, 2);
    Position pos2(1, 3);
    
    // Important!!! 这种表达式必须使用一个大括号括起来
    REQUIRE((pos1.x == 1 && pos1.y == 2));
    REQUIRE((pos2.x == 1 && pos2.y == 3));

    SECTION("modifing the pos") {
        pos1.x = 10;
        REQUIRE(pos1.x == 10);
    }
}

TEST_CASE("Message should be set correctly", "[tools][message]") {
    Message msg1("abc", 0);
    Message msg2("1f221abc", -1);
    Message msg3("_asvbc", 1);

    REQUIRE(msg1.msg == "abc");
    REQUIRE(msg1.status == 0);

    REQUIRE(msg2.msg == "1f221abc");
    REQUIRE(msg2.status == -1);

    REQUIRE(msg3.msg == "_asvbc");
    REQUIRE(msg3.status == 1);
}

TEST_CASE("SpecialChar should be set correctly", "[tools][special_char]") {
    SpecialChar special_char1("\U000f1302", 2);
    REQUIRE(special_char1.special_char == "󱌂");
}
