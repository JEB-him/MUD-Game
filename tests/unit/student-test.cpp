#include "catch.hpp"
#include "Protagonist.h"

TEST_CASE("Protagonist Initialization") {
    Protagonist hero("OUC_2024_001", "Hero");

    SECTION("基本信息") {
        REQUIRE(hero.getProtagonistId() == "OUC_2024_001");
        REQUIRE(hero.getName() == "Hero");
    }

    SECTION("属性初始化") {
        auto attrs = hero.getBaseAttrs();
        REQUIRE(attrs[BasicValue::ProtagonistAttr::INTEL_SCI] == 0);
        REQUIRE(attrs[BasicValue::ProtagonistAttr::INTEL_ARTS] == 0);
        REQUIRE(attrs[BasicValue::ProtagonistAttr::STRENGTH] == 0);
        REQUIRE(attrs[BasicValue::ProtagonistAttr::MONEY] == 0);
        REQUIRE(attrs[BasicValue::ProtagonistAttr::HEALTH] == 100);
    }
}
