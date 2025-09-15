// #include "catch.hpp"
// #include "Item.h"
// #include "backpack.h"
// #include <memory>
// #include <vector>
// #include <string>
// #include"Protagonist.h"

// TEST_CASE("All of backpack function and part of item function", "[backpack][part of item]") {
//     Protagonist protagonist("114514", "小王");
//     Backpack backpack;
//     backpack.addItem("advanced_mathematics");

//     SECTION("test_body") {
//         backpack.addItem("eye_protector_lamps");
//         CHECK(backpack.getBackpack‌Items().size() == 2);
//         backpack.addItem("nutritious_meals");
//         CHECK(backpack.getBackpack‌Items().size() == 3);
//         backpack.addItem("milk");
//         CHECK(backpack.getBackpack‌Items().size() == 4);
//         backpack.addItem("vitamins");
//         REQUIRE(backpack.getBackpack‌Items().size() == 5);
//         cout << backpack.getBackpack‌Items()[0].get()->getName();
//         cout << backpack.getBackpack‌Items()[0].get()->getDescription();
//         CHECK(backpack.getBackpack‌Items()[0].get()->getValue() == 30);
//         REQUIRE(backpack.getBackpack‌Items()[0].get()->getValue() == 30);   
//         REQUIRE(backpack.getBackpack‌Items()[0].get()->getIsConsumable() == false);

//         backpack.useFunctionOfItem(4, protagonist);
//         CHECK(backpack.getBackpack‌Items().size() == 4);
//     }
// }
