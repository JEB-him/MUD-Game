//#pragma once
//#include "catch.hpp"
//#include "Item.h"
//#include "backpack.h"
//#include <memory>
//#include <vector>
//#include <string>
//#include"Protagonist.h"
//
//// 模拟 Protagonist 类用于测试
//class MockProtagonist : public Protagonist {
//public:
//    void changeHealth(int delta) override { health += delta; }
//    void changeAttack(int delta) override { attack += delta; }
//    void changeDefense(int delta) override { defense += delta; }
//
//    int getHealth() const { return health; }
//    int getAttack() const { return attack; }
//    int getDefense() const { return defense; }
//
//private:
//    int health = 100;
//    int attack = 10;
//    int defense = 5;
//};
//
//TEST_CASE("Backpack Construction", "[backpack]") {
//    Backpack backpack;
//
//    SECTION("Default constructor creates empty backpack") {
//        REQUIRE(backpack.getBackpackItems().size() == 0);
//    }
//}
//
//TEST_CASE("Adding items to backpack", "[backpack]") {
//    Backpack backpack;
//
//    SECTION("Add single item") {
//        backpack.addItem("TestItem");
//        REQUIRE(backpack.getBackpackItems().size() == 1);
//    }
//
//    SECTION("Add multiple items") {
//        backpack.addItem("TestItem1");
//        backpack.addItem("TestItem2");
//        backpack.addItem("TestItem3");
//        REQUIRE(backpack.getBackpackItems().size() == 3);
//    }
//}
//
//TEST_CASE("Using consumable items", "[backpack]") {
//    Backpack backpack;
//    MockProtagonist protagonist;
//
//    // 假设有一个名为"HealthPotion"的消耗品
//    backpack.addItem("HealthPotion");
//
//    SECTION("Consumable item is removed after use") {
//        int initialSize = backpack.getBackpackItems().size();
//        backpack.useFunctionOfItem(1, protagonist);
//        REQUIRE(backpack.getBackpackItems().size() == initialSize - 1);
//    }
//}
//
//TEST_CASE("Using equipment items", "[backpack]") {
//    Backpack backpack;
//    MockProtagonist protagonist;
//
//    // 假设有一个名为"Sword"的可装备物品
//    backpack.addItem("Sword");
//
//    SECTION("Equipment item remains after use") {
//        int initialSize = backpack.getBackpackItems().size();
//        backpack.useFunctionOfItem(1, protagonist);
//        REQUIRE(backpack.getBackpackItems().size() == initialSize);
//    }
//
//    SECTION("Equipment can be toggled (equip/unequip)") {
//        // 第一次使用是装备
//        backpack.useFunctionOfItem(1, protagonist);
//        // 第二次使用是卸下
//        backpack.useFunctionOfItem(1, protagonist);
//        REQUIRE(backpack.getBackpackItems().size() == 1);
//    }
//}
//
//TEST_CASE("Item ordering after consumption", "[backpack]") {
//    Backpack backpack;
//    MockProtagonist protagonist;
//
//    // 添加多个物品
//    backpack.addItem("HealthPotion"); // 位置1
//    backpack.addItem("Sword");        // 位置2
//    backpack.addItem("ManaPotion");   // 位置3
//
//    SECTION("Removing item shifts subsequent items") {
//        // 移除位置1的物品（消耗品）
//        backpack.useFunctionOfItem(1, protagonist);
//
//        // 现在Sword应该在位置1，ManaPotion在位置2
//        auto items = backpack.getBackpackItems();
//        REQUIRE(items.size() == 2);
//        // 这里可以添加更多检查，验证物品顺序是否正确
//    }
//}
//
//TEST_CASE("Edge cases", "[backpack]") {
//    Backpack backpack;
//    MockProtagonist protagonist;
//
//    SECTION("Using invalid order number") {
//        // 应该处理异常或 gracefully fail
//        REQUIRE_NOTHROW(backpack.useFunctionOfItem(999, protagonist));
//    }
//
//    SECTION("Adding item with invalid name") {
//        // 应该处理未知物品名称
//        REQUIRE_NOTHROW(backpack.addItem("NonExistentItem"));
//    }
//}