//#include "catch.hpp"
//#include "Item.h"
//#include "Protagonist.h"
//#include <memory>
//#include <unordered_map>
//
//TEST_CASE("Item Creation and Basic Properties", "[Item]") {
//    SECTION("Basic Item Properties") {
//        Item basicItem("TestItem", "A test item", 10.0f, true);
//
//        REQUIRE(basicItem.getName() == "TestItem");
//        REQUIRE(basicItem.getDescription() == "A test item");
//        REQUIRE(basicItem.getValue() == 10.0f);
//        REQUIRE(basicItem.getIsConsumable() == true);
//    }
//}
//
//TEST_CASE("Consumable Item", "[Consumable]") {
//    Protagonist protagonist("test_id", "Test Protagonist");
//
//    SECTION("Food Item Usage") {
//        Food food("Bread", "Restores strength", 5.0f, 20.0f, 5.0f, 1.0f, 0.0f);
//
//        // 获取初始属性
//        auto initialAttrs = protagonist.getBaseAttrs();
//        float initialStrength = initialAttrs[BasicValue::ProtagonistAttr::STRENGTH];
//        float initialHealth = initialAttrs[BasicValue::ProtagonistAttr::HEALTH];
//
//        food.use(protagonist);
//
//        // 获取更新后的属性
//        auto updatedAttrs = protagonist.getBaseAttrs();
//        REQUIRE(updatedAttrs[BasicValue::ProtagonistAttr::STRENGTH] == initialStrength + 20.0f);
//        REQUIRE(updatedAttrs[BasicValue::ProtagonistAttr::HEALTH] == initialHealth + 5.0f);
//    }
//
//    SECTION("Food Cooldown") {
//        Food food("Bread", "Restores strength", 5.0f, 20.0f, 5.0f, 1.0f, 0.0f);
//
//        // 第一次使用
//        food.use(protagonist);
//
//        // 立即尝试第二次使用
//        REQUIRE(food.isOnCooldown() == true);
//    }
//}
//
//TEST_CASE("Equippable Item", "[Equippable]") {
//    Protagonist protagonist("test_id", "Test Protagonist");
//
//    SECTION("Study Material Equipment") {
//        StudyMaterial mathBook("Math Book", "Increases science intelligence", 50.0f, true, 0.1f);
//
//        // 获取初始属性
//        auto initialAttrs = protagonist.getHiddenAttrs();
//        float initialSciBoostRate = initialAttrs[BasicValue::ProtagonistAttr::INTELSCI_BOOST_RATE];
//
//        mathBook.equipAndUnequip(protagonist);
//        REQUIRE(mathBook.getEquipState() == true);
//
//        // 获取更新后的属性
//        auto updatedAttrs = protagonist.getHiddenAttrs();
//        REQUIRE(updatedAttrs[BasicValue::ProtagonistAttr::INTELSCI_BOOST_RATE] == initialSciBoostRate + 0.1f);
//
//        mathBook.equipAndUnequip(protagonist);
//        REQUIRE(mathBook.getEquipState() == false);
//
//        // 检查属性是否恢复
//        auto finalAttrs = protagonist.getHiddenAttrs();
//        REQUIRE(finalAttrs[BasicValue::ProtagonistAttr::INTELSCI_BOOST_RATE] == initialSciBoostRate);
//    }
//
//    SECTION("Sports Equipment") {
//        SportsEquipment basketball("Basketball", "For sports activities", 30.0f);
//
//        basketball.equipAndUnequip(protagonist);
//        REQUIRE(basketball.getEquipState() == true);
//
//        basketball.equipAndUnequip(protagonist);
//        REQUIRE(basketball.getEquipState() == false);
//    }
//
//    SECTION("Study Aid Equipment") {
//        StudyAid alarmClock("Alarm Clock", "Reduces learning time", 25.0f, 0.1f, 0.0f);
//
//        // 获取初始属性
//        auto initialAttrs = protagonist.getHiddenAttrs();
//        float initialTimeReduction = initialAttrs[BasicValue::ProtagonistAttr::LEARNING_TIME_REDUCTION_RATE];
//
//        alarmClock.equipAndUnequip(protagonist);
//        REQUIRE(alarmClock.getEquipState() == true);
//
//        // 获取更新后的属性
//        auto updatedAttrs = protagonist.getHiddenAttrs();
//        REQUIRE(updatedAttrs[BasicValue::ProtagonistAttr::LEARNING_TIME_REDUCTION_RATE] == initialTimeReduction - 0.1f);
//
//        alarmClock.equipAndUnequip(protagonist);
//        REQUIRE(alarmClock.getEquipState() == false);
//
//        // 检查属性是否恢复
//        auto finalAttrs = protagonist.getHiddenAttrs();
//        REQUIRE(finalAttrs[BasicValue::ProtagonistAttr::LEARNING_TIME_REDUCTION_RATE] == initialTimeReduction);
//    }
//}
//
//TEST_CASE("Health Item Usage", "[Consumable][Health]") {
//    Protagonist protagonist("test_id", "Test Protagonist");
//
//    SECTION("Health Restoration") {
//        HealthItem firstAid("First Aid", "Restores health", 40.0f, 50.0f, 0.0f);
//
//        // 先降低健康值
//        protagonist.updateAttr(BasicValue::ProtagonistAttr::HEALTH, -20, true);
//
//        // 获取初始健康值
//        auto initialAttrs = protagonist.getBaseAttrs();
//        float initialHealth = initialAttrs[BasicValue::ProtagonistAttr::HEALTH];
//
//        firstAid.use(protagonist);
//
//        // 获取更新后的健康值
//        auto updatedAttrs = protagonist.getBaseAttrs();
//        REQUIRE(updatedAttrs[BasicValue::ProtagonistAttr::HEALTH] == initialHealth + 50.0f);
//    }
//}
//
//TEST_CASE("Learning Aid Usage", "[Consumable][Learning]") {
//    Protagonist protagonist("test_id", "Test Protagonist");
//
//    SECTION("Intelligence Boost") {
//        LearningAid energyDrink("Energy Drink", "Boosts intelligence", 5.0f, 5.0f, 0.1f, 2.0f, true, false, 5.0f);
//
//        // 获取初始属性
//        auto initialBaseAttrs = protagonist.getBaseAttrs();
//        auto initialHiddenAttrs = protagonist.getHiddenAttrs();
//
//        float initialSciIntel = initialBaseAttrs[BasicValue::ProtagonistAttr::INTEL_SCI];
//        float initialHealth = initialBaseAttrs[BasicValue::ProtagonistAttr::HEALTH];
//
//        energyDrink.use(protagonist);
//
//        // 获取更新后的属性
//        auto updatedBaseAttrs = protagonist.getBaseAttrs();
//        auto updatedHiddenAttrs = protagonist.getHiddenAttrs();
//
//        REQUIRE(updatedBaseAttrs[BasicValue::ProtagonistAttr::INTEL_SCI] == initialSciIntel + 5.0f);
//        REQUIRE(updatedBaseAttrs[BasicValue::ProtagonistAttr::HEALTH] == initialHealth - 5.0f);
//    }
//}
//
//TEST_CASE("Item Effects on Protagonist Attributes", "[Item][Protagonist]") {
//    Protagonist protagonist("test_id", "Test Protagonist");
//
//    SECTION("Study Material Effect on Intelligence") {
//        StudyMaterial mathBook("Math Book", "Increases science intelligence", 50.0f, true, 0.1f);
//        StudyMaterial englishBook("English Book", "Increases arts intelligence", 40.0f, false, 0.1f);
//
//        // 获取初始属性
//        auto initialAttrs = protagonist.getHiddenAttrs();
//        float initialSciBoostRate = initialAttrs[BasicValue::ProtagonistAttr::INTELSCI_BOOST_RATE];
//        float initialArtsBoostRate = initialAttrs[BasicValue::ProtagonistAttr::INTELARTS_BOOST_RATE];
//
//        // 装备数学书
//        mathBook.equipAndUnequip(protagonist);
//        auto afterMathAttrs = protagonist.getHiddenAttrs();
//        REQUIRE(afterMathAttrs[BasicValue::ProtagonistAttr::INTELSCI_BOOST_RATE] == initialSciBoostRate + 0.1f);
//
//        // 装备英语书
//        englishBook.equipAndUnequip(protagonist);
//        auto afterEnglishAttrs = protagonist.getHiddenAttrs();
//        REQUIRE(afterEnglishAttrs[BasicValue::ProtagonistAttr::INTELARTS_BOOST_RATE] == initialArtsBoostRate + 0.1f);
//
//        // 卸下数学书
//        mathBook.equipAndUnequip(protagonist);
//        auto afterUnequipMathAttrs = protagonist.getHiddenAttrs();
//        REQUIRE(afterUnequipMathAttrs[BasicValue::ProtagonistAttr::INTELSCI_BOOST_RATE] == initialSciBoostRate);
//
//        // 卸下英语书
//        englishBook.equipAndUnequip(protagonist);
//        auto afterUnequipEnglishAttrs = protagonist.getHiddenAttrs();
//        REQUIRE(afterUnequipEnglishAttrs[BasicValue::ProtagonistAttr::INTELARTS_BOOST_RATE] == initialArtsBoostRate);
//    }
//}
//
//TEST_CASE("Item Validation", "[Item]") {
//    SECTION("Invalid Item Creation") {
//        // 测试无效参数创建物品
//        REQUIRE_NOTHROW(StudyMaterial("", "", -1.0f, true, -0.1f));
//        REQUIRE_NOTHROW(Food("", "", -1.0f, -1.0f, -1.0f, -1.0f, -1.0f));
//    }
//}
