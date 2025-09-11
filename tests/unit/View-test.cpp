#include <catch.hpp>
#include <iostream>
#include "View.h"

const std::string TMP_FILE = "tmp_map.txt";

void writeMap2File(int n, const char map[][Map::MAX_WIDTH], const std::string& filename=TMP_FILE);
TEST_CASE("reDraw the screen", "[view]") {
    char map_str_2[Map::MAX_HEIGHT][Map::MAX_WIDTH] = {
    "#########################o   ##################               #################",
    "i                      #      #               #               #               #",
    "                       #      #               #               #               #",
    "#     9      0         #      #     9         #               #               #",
    "#                      #      #               #               #               i",
    "#                                             #               #       9        ",
    "#                                             #               #               #",
    "#                                             #               #               #",
    "o                                           1 #               #               #",
    "             2                                #######o   ######               #",
    "#                                                                             #",
    "#   9                                                                         o",
    "#                                                                              ",
    "#                                                                             #",
    "#                                                                             #",
    "#                                                                             #",
    "#                                                                             #",
    "#                                                                             #",
    "#                                                                             #",
    "################################i   ###########################################"
    };
    writeMap2File(20, map_str_2);
}
