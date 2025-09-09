#include <catch.hpp>
#include <iostream>
#include "View.h"

TEST_CASE("reDraw the screen", "[view]") {
    auto controller = Controller::getInstance();
    auto view = View::getInstance(controller);
}
