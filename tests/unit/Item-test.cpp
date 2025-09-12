#include "catch.hpp"
#include "Item.h"
#include "backpack.h"
#include "Protagonist.h"
#include <memory>
#include <unordered_map>

TEST_CASE("Item function test", "[item]") {
    Protagonist protagonist("114514", "小王");
    Backpack backpack;
    backpack.addItem("advanced_mathematics");
    backpack.addItem("eye_protector_lamps");
    CHECK(backpack.getBackpack‌Items().size() == 2);
    backpack.addItem("nutritious_meals");
    CHECK(backpack.getBackpack‌Items().size() == 3);
    backpack.addItem("milk");
    CHECK(backpack.getBackpack‌Items().size() == 4);
    backpack.addItem("vitamins");
    REQUIRE(backpack.getBackpack‌Items().size() == 5);
    cout << backpack.getBackpack‌Items()[0].get()->getName();
    cout << backpack.getBackpack‌Items()[0].get()->getDescription();
    CHECK(backpack.getBackpack‌Items()[0].get()->getValue() == 30);
    REQUIRE(backpack.getBackpack‌Items()[0].get()->getValue() == 30);
    REQUIRE(backpack.getBackpack‌Items()[0].get()->getIsConsumable() == false);

    SECTION("milk") {
        CHECK(protagonist.getHiddenAttrs()[BasicValue::ProtagonistAttr::INTELSCI_BOOST] == 1);
        CHECK(protagonist.getHiddenAttrs()[BasicValue::ProtagonistAttr::INTELARTS_BOOST] == 1);
        CHECK(protagonist.getHiddenAttrs()[BasicValue::ProtagonistAttr::INTELSCI_BOOST_RATE] == 1);
        CHECK(protagonist.getHiddenAttrs()[BasicValue::ProtagonistAttr::INTELARTS_BOOST_RATE] == 1);
        backpack.useFunctionOfItem(4, protagonist);
        CHECK(backpack.getBackpack‌Items().size() == 4);
        CHECK(protagonist.getHiddenAttrs()[BasicValue::ProtagonistAttr::INTELSCI_BOOST]==1);
        CHECK(protagonist.getHiddenAttrs()[BasicValue::ProtagonistAttr::INTELARTS_BOOST]==1);
        CHECK(protagonist.getHiddenAttrs()[BasicValue::ProtagonistAttr::INTELSCI_BOOST_RATE]==1);
        CHECK(protagonist.getHiddenAttrs()[BasicValue::ProtagonistAttr::INTELARTS_BOOST_RATE]==1);
    }

    SECTION("milk") {
        CHECK(protagonist.getHiddenAttrs()[BasicValue::ProtagonistAttr::INTELSCI_BOOST] == 1);
        CHECK(protagonist.getHiddenAttrs()[BasicValue::ProtagonistAttr::INTELARTS_BOOST] == 1);
        CHECK(protagonist.getHiddenAttrs()[BasicValue::ProtagonistAttr::INTELSCI_BOOST_RATE] == 1);
        CHECK(protagonist.getHiddenAttrs()[BasicValue::ProtagonistAttr::INTELARTS_BOOST_RATE] == 1);
        backpack.useFunctionOfItem(4, protagonist);
        CHECK(backpack.getBackpack‌Items().size() == 4);
        CHECK(protagonist.getHiddenAttrs()[BasicValue::ProtagonistAttr::INTELSCI_BOOST] == 1);
        CHECK(protagonist.getHiddenAttrs()[BasicValue::ProtagonistAttr::INTELARTS_BOOST] == 1);
        CHECK(protagonist.getHiddenAttrs()[BasicValue::ProtagonistAttr::INTELSCI_BOOST_RATE] == 1);
        CHECK(protagonist.getHiddenAttrs()[BasicValue::ProtagonistAttr::INTELARTS_BOOST_RATE] == 1);
    }
}