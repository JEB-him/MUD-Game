#pragma once
#include "catch.hpp"
#include "Item.h"
#include "backpack.h"
#include <memory>
#include <vector>
#include <string>
#include"Protagonist.h"

TEST_CASE("Backpack Construction", "[backpack]") {
    Protagonist protagonist("114514", "小王");
    Backpack backpack;
    backpack.addItem("高等数学");
    backpack.addItem("篮球");
    backpack.addItem("护眼台灯");
    backpack.addItem("营养餐");
    backpack.addItem("牛奶");
    backpack.addItem("维生素");

    SECTION("Default constructor creates empty backpack") {
        REQUIRE(backpack.getBackpack‌Items().size() == 6);
        REQUIRE(backpack.getBackpack‌Items()[0].get()->getName() == "高等数学");
    }
}
