#include "catch.hpp"
#include "Item.h"
#include "Protagonist.cpp"
#include <memory>

TEST_CASE("Item Creation and Basic Properties", "[Item]") {
    SECTION("Basic Item Properties") {
        Item basicItem("TestItem", "A test item", 10.0f, true);

        REQUIRE(basicItem.getName() == "TestItem");
        REQUIRE(basicItem.getDescription() == "A test item");
        REQUIRE(basicItem.getValue() == 10.0f);
        REQUIRE(basicItem.getIsConsumable() == true);
    }
}

TEST_CASE("Consumable Item", "[Consumable]") {
    Protagonist protagonist; // 假设Protagonist有默认构造函数

    SECTION("Food Item Usage") {
        Food food("Bread", "Restores strength", 5.0f, 20.0f, 5.0f, 1.0f, 0.0f);

        float initialStrength = protagonist.getStrength();
        float initialHealth = protagonist.getHealth();

        food.use(protagonist);

        REQUIRE(protagonist.getStrength() == initialStrength + 20.0f);
        REQUIRE(protagonist.getHealth() == initialHealth + 5.0f);
    }

    SECTION("Food Cooldown") {
        Food food("Bread", "Restores strength", 5.0f, 20.0f, 5.0f, 1.0f, 0.0f);

        // 第一次使用
        food.use(protagonist);

        // 立即尝试第二次使用
        REQUIRE(food.isOnCooldown() == true);
    }
}

TEST_CASE("Equippable Item", "[Equippable]") {
    Protagonist protagonist;

    SECTION("Study Material Equipment") {
        StudyMaterial mathBook("Math Book", "Increases science intelligence", 50.0f, true, 0.1f);

        float initialSciIntel = protagonist.getIntelSci();

        mathBook.equipAndUnequip(protagonist);
        REQUIRE(mathBook.getEquipState() == true);
        REQUIRE(protagonist.getIntelSci() == initialSciIntel * 1.1f);

        mathBook.equipAndUnequip(protagonist);
        REQUIRE(mathBook.getEquipState() == false);
        REQUIRE(protagonist.getIntelSci() == initialSciIntel);
    }

    SECTION("Sports Equipment") {
        SportsEquipment basketball("Basketball", "For sports activities", 30.0f);

        basketball.equipAndUnequip(protagonist);
        REQUIRE(basketball.getEquipState() == true);

        basketball.equipAndUnequip(protagonist);
        REQUIRE(basketball.getEquipState() == false);
    }
}

TEST_CASE("Item Creator", "[ItemCreator]") {
    ItemCreator creator("test_items_config.json");

    SECTION("Create Study Material") {
        auto item = creator.createItem("math_book");
        REQUIRE(item->getName() == "高等数学");
        REQUIRE(item->getIsConsumable() == false);
    }

    SECTION("Create Food Item") {
        auto item = creator.createItem("nutrition_food");
        REQUIRE(item->getName() == "营养餐");
        REQUIRE(item->getIsConsumable() == true);
    }

    SECTION("Create Non-Existent Item") {
        REQUIRE_THROWS_AS(creator.createItem("non_existent_item"), std::runtime_error);
    }
}

TEST_CASE("Item Effects on Protagonist", "[Item][Protagonist]") {
    Protagonist protagonist;

    SECTION("Health Item Usage") {
        HealthItem firstAid("First Aid", "Restores health", 40.0f, 50.0f, 0.0f);

        float initialHealth = protagonist.getHealth();
        protagonist.reduceHealth(20.0f); // 假设有减少健康的方法

        firstAid.use(protagonist);
        REQUIRE(protagonist.getHealth() == initialHealth - 20.0f + 50.0f);
    }

    SECTION("Learning Aid Usage") {
        LearningAid energyDrink("Energy Drink", "Boosts intelligence", 5.0f, 5.0f, 0.1f, 2.0f, true, false, 5.0f);

        float initialIntel = protagonist.getIntelSci();
        float initialHealth = protagonist.getHealth();

        energyDrink.use(protagonist);
        REQUIRE(protagonist.getIntelSci() == initialIntel + 5.0f);
        REQUIRE(protagonist.getHealth() == initialHealth - 5.0f);
    }
}
