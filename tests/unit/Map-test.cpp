#include "catch.hpp"
#include "Map.h"
#include <iostream>
#include <fstream>

const std::string TMP_FILE = "tmp_map.txt";

void writeMap2File(int n, const char map[][Map::MAX_WIDTH], const std::string& filename=TMP_FILE) {
    const auto path = Map::BASE_DIR + filename;
    std::ofstream file(path.c_str());
    for (int i = 0; i < n; ++ i) {
        file << map[i] << std::endl;
    }
}

TEST_CASE("Map can load correct map files and reject wrong map files", "[Map][constructor]") {
    SECTION("Simple map i/o in row") {
        char map_str[Map::MAX_HEIGHT][Map::MAX_WIDTH] = {
            "##########o   ###",
            "#               #",
            "#               #",
            "#               #",
            "##########i   ###"
        };
        writeMap2File(5, map_str);
        Map map(TMP_FILE);
        REQUIRE(map.getValidMsg() == "");
        REQUIRE(map.valid() == true);
    }
    SECTION("Simple map i/o in col") {
        char map_str[Map::MAX_HEIGHT][Map::MAX_WIDTH] = {
            "##########o   ###",
            "#               #",
            "#               i",
            "#                ",
            "#               #",
            "#################"
        };
        writeMap2File(6, map_str);
        Map map(TMP_FILE);
        REQUIRE(map.getValidMsg() == "");
        REQUIRE(map.valid() == true);
    }
    SECTION("Simple map with middle empty line") {
        char map_str[Map::MAX_HEIGHT][Map::MAX_WIDTH] = {
            "##########o   ###",
            "#               #",
            "i               i",
            "                 ",
            "#               #",
            "#################"
        };
        writeMap2File(6, map_str);
        Map map(TMP_FILE);
        REQUIRE(map.getValidMsg() == "");
        REQUIRE(map.valid() == true);
    }
    SECTION("Simple map with top empty line(1)") {
        char map_str[Map::MAX_HEIGHT][Map::MAX_WIDTH] = {
            "                 ",
            "##########o   ###",
            "#               #",
            "i               i",
            "                 ",
            "#               #",
            "#################"
        };
        writeMap2File(7, map_str);
        Map map(TMP_FILE);
        REQUIRE(map.getValidMsg() == "");
        REQUIRE(map.valid() == true);
    }
    SECTION("Simple map with top empty line(2)") {
        char map_str[Map::MAX_HEIGHT][Map::MAX_WIDTH] = {
            "",
            "##########o   ###",
            "#               #",
            "i               i",
            "                 ",
            "#               #",
            "#################"
        };
        writeMap2File(7, map_str);
        Map map(TMP_FILE);
        REQUIRE(map.getValidMsg() == "");
        REQUIRE(map.valid() == true);
    }
    SECTION("Simple map with bottom empty line") {
        char map_str[Map::MAX_HEIGHT][Map::MAX_WIDTH] = {
            "##########o   ###",
            "#               #",
            "i               i",
            "                 ",
            "#               #",
            "#################",
            "                 ",
        };
        writeMap2File(7, map_str);
        Map map(TMP_FILE);
        REQUIRE(map.getValidMsg() == "");
        REQUIRE(map.valid() == true);
    }
    SECTION("Simple map with top empty line(1)") {
        char map_str[Map::MAX_HEIGHT][Map::MAX_WIDTH] = {
            "##########o   ###",
            "#               #",
            "i               i",
            "                 ",
            "#               #",
            "#################",
            "",
        };
        writeMap2File(7, map_str);
        Map map(TMP_FILE);
        REQUIRE(map.getValidMsg() == "");
        REQUIRE(map.valid() == true);
    }
    SECTION("Simple map with internal wall") {
        char map_str[Map::MAX_HEIGHT][Map::MAX_WIDTH] = {
            "##########o   ###",
            "#               #",
            "i               i",
            "                 ",
            "#      #        #",
            "#################",
            "",
        };
        writeMap2File(7, map_str);
        Map map(TMP_FILE);
        REQUIRE(map.getValidMsg() == "");
        REQUIRE(map.valid() == true);
    }
    SECTION("Invalid map with single \'#\'") {
        char map_str[Map::MAX_HEIGHT][Map::MAX_WIDTH] = {
            "#",
            "##########o   ###",
            "#               #",
            "i               i",
            "                 ",
            "#               #",
            "#################",
            "",
        };
        writeMap2File(8, map_str);
        Map map(TMP_FILE);
        REQUIRE(map.getValidMsg() == "在classifyLine 中检测出：无效地图");
        REQUIRE(map.valid() == false);
    }

    SECTION("Invalid map with single \'#\'") {
        char map_str[Map::MAX_HEIGHT][Map::MAX_WIDTH] = {
            "##########o   ###",
            "#               #",
            "i               i",
            "                 ",
            "#               #",
            "#################",
            "#"
        };
        writeMap2File(7, map_str);
        Map map(TMP_FILE);
        REQUIRE(map.getValidMsg() == "在classifyLine 中检测出：无效地图");
        REQUIRE(map.valid() == false);
    }

    SECTION("Invalid map with unclosed wall") {
        char map_str[Map::MAX_HEIGHT][Map::MAX_WIDTH] = {
            "###### ###o   ###",
            "#               #",
            "i               i",
            "                 ",
            "#               #",
            "#################"
        };
        writeMap2File(6, map_str);
        Map map(TMP_FILE);
        REQUIRE(map.getValidMsg() == "在classifyLine 中检测出：无效地图");
        REQUIRE(map.valid() == false);
    }

    SECTION("Invalid map with bad wall") {
        char map_str[Map::MAX_HEIGHT][Map::MAX_WIDTH] = {
            "##########o   ####",
            "#               #",
            "i               i",
            "                 ",
            "#               #",
            "#################"
        };
        writeMap2File(6, map_str);
        Map map(TMP_FILE);
        REQUIRE(map.valid() == false);
    }

    SECTION("Invalid map with bad wall") {
        char map_str[Map::MAX_HEIGHT][Map::MAX_WIDTH] = {
            "##########o   ###",
            "#               #",
            "i               i",
            "                 ",
            "#               #",
            "################"
        };
        writeMap2File(6, map_str);
        Map map(TMP_FILE);
        REQUIRE(map.valid() == false);
    }

    SECTION("non-standard rectangular map") {
        char map_str[Map::MAX_HEIGHT][Map::MAX_WIDTH] = {
            "##########o   ### ####",
            "#               ###  #",
            "i                    i",
            "                      ",
            "#                    #",
            "######################"
        };
        writeMap2File(6, map_str);
        Map map(TMP_FILE);
        REQUIRE(map.getValidMsg() == "");
        REQUIRE(map.valid() == true);
    }

    SECTION("non-standard rectangular map with wrong structure") {
        char map_str[Map::MAX_HEIGHT][Map::MAX_WIDTH] = {
            "##########o   ### ###",
            "#               ###  #",
            "i                    i",
            "                      ",
            "#                    #",
            "######################"
        };
        writeMap2File(6, map_str);
        Map map(TMP_FILE);
        REQUIRE(map.valid() == false);
    }

    SECTION("map with wide characters") {
        char map_str[Map::MAX_HEIGHT][Map::MAX_WIDTH] = {
            "##########o   ### ###",
            "#               ###  #",
            "i                    i",
            "                      ",
            "#                  9 #",
            "######################"
        };
        writeMap2File(6, map_str);
        Map map(TMP_FILE);
        REQUIRE(map.getValidMsg() == "");
        REQUIRE(map.valid() == true);
    }

    SECTION("map with misplaced wide characters") {
        char map_str[Map::MAX_HEIGHT][Map::MAX_WIDTH] = {
            "##########o   ### ###",
            "#               ###  #",
            "i                    i",
            "                      ",
            "#                   9#",
            "######################"
        };
        writeMap2File(6, map_str);
        Map map(TMP_FILE);
        REQUIRE(map.valid() == false);
    }

    SECTION("map with misplaced wide characters") {
        char map_str[Map::MAX_HEIGHT][Map::MAX_WIDTH] = {
            "##########o   ### ###",
            "#               ###  #",
            "i                   9i",
            "                      ",
            "#                    #",
            "######################"
        };
        writeMap2File(6, map_str);
        Map map(TMP_FILE);
        REQUIRE(map.valid() == false);
    }
}
