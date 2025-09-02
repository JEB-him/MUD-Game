#include "catch.hpp"
#include <iostream>
#include "NPC.h"

TEST_CASE("npc seniorstudent", "[npc][SeniorStudent]") {
    system("chcp 65001");
    SeniorStudent SeniorStudent("aaa", 70, "computer");
    
    // Important!!! 这种表达式必须使用一个大括号括起来
    REQUIRE((SeniorStudent.getTitle() == "aaa"));
    REQUIRE((SeniorStudent.getInteractionText() == "你好,我是computer专业的学长aaa,最近有点忙,能帮帮我吗?"));

    SECTION("modifing the student title") {
        SeniorStudent.setTitle("李四");
        REQUIRE(SeniorStudent.getTitle() == "李四");
    }

    std::cout << SeniorStudent.getInteractionText() << std::endl;
    SeniorStudent.startInteraction();
}
