#include "catch.hpp"
#include "Item.h"
#include "backpack.h"
#include "Protagonist.h"
#include <memory>
#include <unordered_map>

TEST_CASE("Item function test", "[item]") {
    Backpack backpack;
    Protagonist protagonist("114514", "小王");

    //背包当前物品总量(size)测试
    backpack.addItem("advanced_mathematics");
    CHECK(backpack.getBackpack‌Items().size() == 1);

    backpack.addItem("eye_protector_lamps");
    CHECK(backpack.getBackpack‌Items().size() == 2);

    backpack.addItem("nutritious_meals");
    CHECK(backpack.getBackpack‌Items().size() == 3);

    backpack.addItem("milk");
    CHECK(backpack.getBackpack‌Items().size() == 4);

    backpack.addItem("vitamins");
    REQUIRE(backpack.getBackpack‌Items().size() == 5);

    //物品基础属性获取测试
    cout << backpack.getBackpack‌Items()[0].get()->getName();
    cout << backpack.getBackpack‌Items()[0].get()->getDescription();
    CHECK(backpack.getBackpack‌Items()[0].get()->getValue() == 30);
    REQUIRE(backpack.getBackpack‌Items()[0].get()->getValue() == 30);
    REQUIRE(backpack.getBackpack‌Items()[0].get()->getIsConsumable() == false);


    SECTION("compressed_crackers") { // CD
        protagonist.updateAttr(BasicValue::ProtagonistAttr::HEALTH, 10, false);
        protagonist.updateAttr(BasicValue::ProtagonistAttr::STRENGTH, 10, false);
        CHECK(protagonist.getHiddenAttrs()[BasicValue::ProtagonistAttr::T_USED_COMPRESSED_CRACKER] == 100000);
        CHECK(protagonist.getBaseAttrs()[BasicValue::ProtagonistAttr::HEALTH] == 10);
        CHECK(protagonist.getBaseAttrs()[BasicValue::ProtagonistAttr::STRENGTH] == 10);
        backpack.addItem("compressed_crackers");
        backpack.useFunctionOfItem(6, protagonist);
        CHECK(protagonist.getHiddenAttrs()[BasicValue::ProtagonistAttr::T_USED_COMPRESSED_CRACKER] == 10000 - 0);
        CHECK(protagonist.getBaseAttrs()[BasicValue::ProtagonistAttr::HEALTH] == 10);
        CHECK(protagonist.getBaseAttrs()[BasicValue::ProtagonistAttr::STRENGTH] == 18);
        //有CD（4）
        protagonist.addGameTime(3);
        backpack.addItem("compressed_crackers");
        backpack.useFunctionOfItem(6, protagonist);
        CHECK(protagonist.getHiddenAttrs()[BasicValue::ProtagonistAttr::T_USED_COMPRESSED_CRACKER] == 10000 - 0);
        CHECK(protagonist.getBaseAttrs()[BasicValue::ProtagonistAttr::HEALTH] == 10);
        CHECK(protagonist.getBaseAttrs()[BasicValue::ProtagonistAttr::STRENGTH] == 18);
        //无CD
        backpack.addItem("compressed_crackers");
        protagonist.addGameTime(18);
        backpack.useFunctionOfItem(6, protagonist);
        CHECK(protagonist.getHiddenAttrs()[BasicValue::ProtagonistAttr::T_USED_COMPRESSED_CRACKER] == 10000 - 21);
        CHECK(protagonist.getBaseAttrs()[BasicValue::ProtagonistAttr::HEALTH] == 10);
        CHECK(protagonist.getBaseAttrs()[BasicValue::ProtagonistAttr::STRENGTH] == 26);
    }
    SECTION("nutritious_meals") {
        protagonist.updateAttr(BasicValue::ProtagonistAttr::HEALTH, 10, false);
        protagonist.updateAttr(BasicValue::ProtagonistAttr::STRENGTH, 10, false);
        CHECK(protagonist.getHiddenAttrs()[BasicValue::ProtagonistAttr::T_USED_COMPRESSED_CRACKER] == 100000);
        CHECK(protagonist.getBaseAttrs()[BasicValue::ProtagonistAttr::HEALTH] == 10);
        CHECK(protagonist.getBaseAttrs()[BasicValue::ProtagonistAttr::STRENGTH] == 10);
        backpack.addItem("nutritious_meals");
        backpack.useFunctionOfItem(6, protagonist);
        CHECK(protagonist.getHiddenAttrs()[BasicValue::ProtagonistAttr::T_USED_COMPRESSED_CRACKER] == 10000 - 0);
        CHECK(protagonist.getBaseAttrs()[BasicValue::ProtagonistAttr::HEALTH] == 40);
        CHECK(protagonist.getBaseAttrs()[BasicValue::ProtagonistAttr::STRENGTH] == 50);
        protagonist.addGameTime(3);
        backpack.addItem("nutritious_meals");
        backpack.useFunctionOfItem(6, protagonist);
        CHECK(protagonist.getHiddenAttrs()[BasicValue::ProtagonistAttr::T_USED_COMPRESSED_CRACKER] == 10000 - 3);
        CHECK(protagonist.getBaseAttrs()[BasicValue::ProtagonistAttr::HEALTH] == 70);
        CHECK(protagonist.getBaseAttrs()[BasicValue::ProtagonistAttr::STRENGTH] == 90);
    }

    SECTION("energy_drink") { //buff punish
        CHECK(protagonist.getHiddenAttrs()[BasicValue::ProtagonistAttr::T_BUFF_ENERGY_DRINK] == 100000);
        CHECK(protagonist.getBaseAttrs()[BasicValue::ProtagonistAttr::HEALTH] == 100);
        CHECK(protagonist.getHiddenAttrs()[BasicValue::ProtagonistAttr::INTELSCI_BOOST] == 0);
        CHECK(protagonist.getHiddenAttrs()[BasicValue::ProtagonistAttr::INTELARTS_BOOST] == 0);
        CHECK(protagonist.getHiddenAttrs()[BasicValue::ProtagonistAttr::INTELSCI_BOOST_RATE] == 1);
        CHECK(protagonist.getHiddenAttrs()[BasicValue::ProtagonistAttr::INTELARTS_BOOST_RATE] == 1);
        backpack.addItem("energy_drink");
        backpack.useFunctionOfItem(6, protagonist);
        CHECK(protagonist.getHiddenAttrs()[BasicValue::ProtagonistAttr::T_BUFF_ENERGY_DRINK] == 10000 - 0);
        CHECK(protagonist.getBaseAttrs()[BasicValue::ProtagonistAttr::HEALTH] == 100);
        CHECK(protagonist.getHiddenAttrs()[BasicValue::ProtagonistAttr::INTELSCI_BOOST] == 5);
        CHECK(protagonist.getHiddenAttrs()[BasicValue::ProtagonistAttr::INTELARTS_BOOST] == 5);
        CHECK(protagonist.getHiddenAttrs()[BasicValue::ProtagonistAttr::INTELSCI_BOOST_RATE] == 1.2f);
        CHECK(protagonist.getHiddenAttrs()[BasicValue::ProtagonistAttr::INTELARTS_BOOST_RATE] == 1.2f);
        //有滥用（4）
        backpack.addItem("energy_drink");
        protagonist.addGameTime(3);
        backpack.useFunctionOfItem(6, protagonist);
        CHECK(protagonist.getHiddenAttrs()[BasicValue::ProtagonistAttr::T_BUFF_ENERGY_DRINK] == 10000 - 3);
        CHECK(protagonist.getBaseAttrs()[BasicValue::ProtagonistAttr::HEALTH] == 95);
        CHECK(protagonist.getHiddenAttrs()[BasicValue::ProtagonistAttr::INTELSCI_BOOST] == 5);
        CHECK(protagonist.getHiddenAttrs()[BasicValue::ProtagonistAttr::INTELARTS_BOOST] == 5);
        CHECK(protagonist.getHiddenAttrs()[BasicValue::ProtagonistAttr::INTELSCI_BOOST_RATE] == 1.2f);
        CHECK(protagonist.getHiddenAttrs()[BasicValue::ProtagonistAttr::INTELARTS_BOOST_RATE] == 1.2f);
        //无滥用
        backpack.addItem("energy_drink");
        protagonist.addGameTime(5);
        backpack.useFunctionOfItem(6, protagonist);
        CHECK(protagonist.getHiddenAttrs()[BasicValue::ProtagonistAttr::T_BUFF_ENERGY_DRINK] == 10000 - 8);
        CHECK(protagonist.getBaseAttrs()[BasicValue::ProtagonistAttr::HEALTH] == 95);
        CHECK(protagonist.getHiddenAttrs()[BasicValue::ProtagonistAttr::INTELSCI_BOOST] == 5);
        CHECK(protagonist.getHiddenAttrs()[BasicValue::ProtagonistAttr::INTELARTS_BOOST] == 5);
        CHECK(protagonist.getHiddenAttrs()[BasicValue::ProtagonistAttr::INTELSCI_BOOST_RATE] == 1.2f);
        CHECK(protagonist.getHiddenAttrs()[BasicValue::ProtagonistAttr::INTELARTS_BOOST_RATE] == 1.2f);
        //buff测试
        protagonist.addGameTime(3);
        CHECK(protagonist.getHiddenAttrs()[BasicValue::ProtagonistAttr::T_BUFF_ENERGY_DRINK] == 10000 - 8);
        CHECK(protagonist.getBaseAttrs()[BasicValue::ProtagonistAttr::HEALTH] == 95);
        CHECK(protagonist.getHiddenAttrs()[BasicValue::ProtagonistAttr::INTELSCI_BOOST] == 0);
        CHECK(protagonist.getHiddenAttrs()[BasicValue::ProtagonistAttr::INTELARTS_BOOST] == 0);
        CHECK(protagonist.getHiddenAttrs()[BasicValue::ProtagonistAttr::INTELSCI_BOOST_RATE] == 1.0f);
        CHECK(protagonist.getHiddenAttrs()[BasicValue::ProtagonistAttr::INTELARTS_BOOST_RATE] == 1.0f);
    }

    SECTION("milk") { // buff
        CHECK(protagonist.getHiddenAttrs()[BasicValue::ProtagonistAttr::T_BUFF_MILK] == 100000);
        CHECK(protagonist.getHiddenAttrs()[BasicValue::ProtagonistAttr::INTELSCI_BOOST] == 0);
        CHECK(protagonist.getHiddenAttrs()[BasicValue::ProtagonistAttr::INTELARTS_BOOST] == 0);
        CHECK(protagonist.getHiddenAttrs()[BasicValue::ProtagonistAttr::INTELSCI_BOOST_RATE] == 1.0f);
        CHECK(protagonist.getHiddenAttrs()[BasicValue::ProtagonistAttr::INTELARTS_BOOST_RATE] == 1.0f);
        backpack.useFunctionOfItem(4, protagonist);
        REQUIRE(backpack.getBackpack‌Items().size() == 4);
        CHECK(protagonist.getHiddenAttrs()[BasicValue::ProtagonistAttr::T_BUFF_MILK] == 10000-0);
        CHECK(protagonist.getHiddenAttrs()[BasicValue::ProtagonistAttr::INTELSCI_BOOST] == 3);
        CHECK(protagonist.getHiddenAttrs()[BasicValue::ProtagonistAttr::INTELARTS_BOOST] == 3);
        CHECK(protagonist.getHiddenAttrs()[BasicValue::ProtagonistAttr::INTELSCI_BOOST_RATE] == 1.15f);
        CHECK(protagonist.getHiddenAttrs()[BasicValue::ProtagonistAttr::INTELARTS_BOOST_RATE] == 1.15f);
        //buff测试
        protagonist.addGameTime(1);
        CHECK(protagonist.getHiddenAttrs()[BasicValue::ProtagonistAttr::T_BUFF_MILK] == 10000-0);
        CHECK(protagonist.getHiddenAttrs()[BasicValue::ProtagonistAttr::INTELSCI_BOOST] == 3);
        CHECK(protagonist.getHiddenAttrs()[BasicValue::ProtagonistAttr::INTELARTS_BOOST] == 3);
        CHECK(protagonist.getHiddenAttrs()[BasicValue::ProtagonistAttr::INTELSCI_BOOST_RATE] == 1.15f);
        CHECK(protagonist.getHiddenAttrs()[BasicValue::ProtagonistAttr::INTELARTS_BOOST_RATE] == 1.15f);
        protagonist.addGameTime(2);
        CHECK(protagonist.getHiddenAttrs()[BasicValue::ProtagonistAttr::T_BUFF_MILK] == 10000-0);
        CHECK(protagonist.getHiddenAttrs()[BasicValue::ProtagonistAttr::INTELSCI_BOOST] == 0);
        CHECK(protagonist.getHiddenAttrs()[BasicValue::ProtagonistAttr::INTELARTS_BOOST] == 0);
        CHECK(protagonist.getHiddenAttrs()[BasicValue::ProtagonistAttr::INTELSCI_BOOST_RATE] == 1.0f);
        CHECK(protagonist.getHiddenAttrs()[BasicValue::ProtagonistAttr::INTELARTS_BOOST_RATE] == 1.0f);
        backpack.addItem("milk");
        protagonist.addGameTime(100);
        backpack.useFunctionOfItem(5, protagonist);
        CHECK(protagonist.getHiddenAttrs()[BasicValue::ProtagonistAttr::T_BUFF_MILK] == 10000 - 103);
        CHECK(protagonist.getHiddenAttrs()[BasicValue::ProtagonistAttr::INTELSCI_BOOST] == 3);
        CHECK(protagonist.getHiddenAttrs()[BasicValue::ProtagonistAttr::INTELARTS_BOOST] == 3);
        CHECK(protagonist.getHiddenAttrs()[BasicValue::ProtagonistAttr::INTELSCI_BOOST_RATE] == 1.15f);
        CHECK(protagonist.getHiddenAttrs()[BasicValue::ProtagonistAttr::INTELARTS_BOOST_RATE] == 1.15f);
    }

    SECTION("first_aid_kit") { // CD
        protagonist.updateAttr(BasicValue::ProtagonistAttr::HEALTH, 10, false);
        CHECK(protagonist.getHiddenAttrs()[BasicValue::ProtagonistAttr::T_USED_FIRST_AID_KIT] == 100000);
        CHECK(protagonist.getBaseAttrs()[BasicValue::ProtagonistAttr::HEALTH] == 10);
        backpack.addItem("first_aid_kit");
        backpack.useFunctionOfItem(6, protagonist);
        CHECK(protagonist.getHiddenAttrs()[BasicValue::ProtagonistAttr::T_USED_FIRST_AID_KIT] == 10000);
        CHECK(protagonist.getBaseAttrs()[BasicValue::ProtagonistAttr::HEALTH] == 50);
        //有CD（20）
        protagonist.addGameTime(3);
        backpack.addItem("first_aid_kit");
        backpack.useFunctionOfItem(6, protagonist);
        CHECK(protagonist.getHiddenAttrs()[BasicValue::ProtagonistAttr::T_USED_FIRST_AID_KIT] == 10000);
        CHECK(protagonist.getBaseAttrs()[BasicValue::ProtagonistAttr::HEALTH] == 50);
        //无CD
        backpack.addItem("first_aid_kit");
        protagonist.addGameTime(18);
        backpack.useFunctionOfItem(6, protagonist);
        CHECK(protagonist.getHiddenAttrs()[BasicValue::ProtagonistAttr::T_USED_FIRST_AID_KIT] == 10000-21);
        CHECK(protagonist.getBaseAttrs()[BasicValue::ProtagonistAttr::HEALTH] == 90);
    }

    SECTION("vitamins") { // buff
        protagonist.updateAttr(BasicValue::ProtagonistAttr::HEALTH, 10, false);
        CHECK(protagonist.getHiddenAttrs()[BasicValue::ProtagonistAttr::T_BUFF_VITAMINS] == 100000);
        CHECK(protagonist.getHiddenAttrs()[BasicValue::ProtagonistAttr::BUFF_VITAMINS] == false);
        CHECK(protagonist.getHiddenAttrs()[BasicValue::ProtagonistAttr::VITMIN_EFFECT_RATE] == 1.0f);
        CHECK(protagonist.getBaseAttrs()[BasicValue::ProtagonistAttr::HEALTH] == 10);
        backpack.addItem("vitamins");
        backpack.useFunctionOfItem(6, protagonist);
        CHECK(protagonist.getHiddenAttrs()[BasicValue::ProtagonistAttr::T_BUFF_VITAMINS] == 10000);
        CHECK(protagonist.getHiddenAttrs()[BasicValue::ProtagonistAttr::BUFF_VITAMINS] == true);
        CHECK(protagonist.getHiddenAttrs()[BasicValue::ProtagonistAttr::VITMIN_EFFECT_RATE] == 0.0f);
        CHECK(protagonist.getBaseAttrs()[BasicValue::ProtagonistAttr::HEALTH] == 10);
        protagonist.addGameTime(3);
        backpack.addItem("vitamins");
        backpack.useFunctionOfItem(6, protagonist);
        CHECK(protagonist.getHiddenAttrs()[BasicValue::ProtagonistAttr::T_BUFF_VITAMINS] == 10000-3);
        CHECK(protagonist.getHiddenAttrs()[BasicValue::ProtagonistAttr::BUFF_VITAMINS] == true);
        CHECK(protagonist.getHiddenAttrs()[BasicValue::ProtagonistAttr::VITMIN_EFFECT_RATE] == 0.0f);
        CHECK(protagonist.getBaseAttrs()[BasicValue::ProtagonistAttr::HEALTH] == 10);
        //buff测试
        CHECK(protagonist.getHiddenAttrs()[BasicValue::ProtagonistAttr::T_BUFF_VITAMINS] == 10000-3);
        CHECK(protagonist.getHiddenAttrs()[BasicValue::ProtagonistAttr::BUFF_VITAMINS] == true);
        CHECK(protagonist.getHiddenAttrs()[BasicValue::ProtagonistAttr::VITMIN_EFFECT_RATE] == 0.0f);
        CHECK(protagonist.getBaseAttrs()[BasicValue::ProtagonistAttr::HEALTH] == 10);
        protagonist.addGameTime(23);
        CHECK(protagonist.getHiddenAttrs()[BasicValue::ProtagonistAttr::T_BUFF_VITAMINS] == 10000-3);
        CHECK(protagonist.getHiddenAttrs()[BasicValue::ProtagonistAttr::BUFF_VITAMINS] == true);
        CHECK(protagonist.getHiddenAttrs()[BasicValue::ProtagonistAttr::VITMIN_EFFECT_RATE] == 0.0f);
        CHECK(protagonist.getBaseAttrs()[BasicValue::ProtagonistAttr::HEALTH] == 10);
        protagonist.addGameTime(2);
        CHECK(protagonist.getHiddenAttrs()[BasicValue::ProtagonistAttr::T_BUFF_VITAMINS] == 10000-3);
        CHECK(protagonist.getHiddenAttrs()[BasicValue::ProtagonistAttr::BUFF_VITAMINS] == false);
        CHECK(protagonist.getHiddenAttrs()[BasicValue::ProtagonistAttr::VITMIN_EFFECT_RATE] == 1.0f);
        CHECK(protagonist.getBaseAttrs()[BasicValue::ProtagonistAttr::HEALTH] == 10);
    }
}