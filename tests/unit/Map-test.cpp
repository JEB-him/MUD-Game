#define CATCH_CONFIG_MAIN

#include "catch.hpp"
#include "Map.h"
#include <iostream>
#include <fstream>
#include <filesystem>

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
            "##########o   ### ####",
            "#               ###  #",
            "i                    i",
            "",
            "#                  1 #",
            "######################"
        };
        writeMap2File(6, map_str);
        Map map(TMP_FILE);
        REQUIRE(map.getValidMsg() == "");
        REQUIRE(map.valid() == true);
    }

    SECTION("map with misplaced wide characters") {
        char map_str[Map::MAX_HEIGHT][Map::MAX_WIDTH] = {
            "##########o   ### ####",
            "#               ###  #",
            "i                    i",
            "                      ",
            "#                   1#",
            "######################"
        };
        writeMap2File(6, map_str);
        Map map(TMP_FILE);
        REQUIRE(map.valid() == false);
    }

    SECTION("map with misplaced wide characters") {
        char map_str[Map::MAX_HEIGHT][Map::MAX_WIDTH] = {
            "##########o   ### ####",
            "#               ###  #",
            "i                   1i",
            "                      ",
            "#                    #",
            "######################"
        };
        writeMap2File(6, map_str);
        Map map(TMP_FILE);
        REQUIRE(map.valid() == false);
    }

    SECTION("map with wrong i/o") {
        char map_str[Map::MAX_HEIGHT][Map::MAX_WIDTH] = {
            "##########o  #### ####",
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

    SECTION("map with wrong i/o") {
        char map_str[Map::MAX_HEIGHT][Map::MAX_WIDTH] = {
            "##########o   ### ####",
            "#               ###  #",
            "i               i    i",
            "                      ",
            "#                    #",
            "######################"
        };
        writeMap2File(6, map_str);
        Map map(TMP_FILE);
        REQUIRE(map.valid() == false);
    }

    const auto path = Map::BASE_DIR + TMP_FILE;
    std::filesystem::remove(path);
}

TEST_CASE("Move protagonist", "[Map][move]") {
    int event_type, id;
    char map_str[Map::MAX_HEIGHT][Map::MAX_WIDTH] = {
    "#########################o   ##################               #################",
    "#                      #      #               #               #               #",
    "#                      #      #               #               #               #",
    "#     9      0         #      #     9         #               #               #",
    "#                      #      #               #               #               i",
    "#                                             #               #       9        ",
    "#                                             #               #               #",
    "#                                             #               #               #",
    "o                                           1 #               #               #",
    "             2                                #######o   ######               #",
    "#                                                                             #",
    "#   9                                                                         #",
    "#                                                                             #",
    "#                                                                             #",
    "#                                                                             #",
    "#                                                                             #",
    "#                                                                             #",
    "#                                                                             #",
    "#                                                                             #",
    "################################i   ###########################################"
    };
    writeMap2File(20, map_str);
    Map map(TMP_FILE);

    char map_str_2[Map::MAX_HEIGHT][Map::MAX_WIDTH] = {
    "#########################o   ##################",
    "#                      #      #               #",
    "#                      #      #               #",
    "#     9      0         #      #     9         #",
    "#                      #      #               #",
    "#        1                                    #",
    "o                2                            #",
    "                                              #",
    "#                                             #",
    "########i   #############i   #######i   #######"
    };
    writeMap2File(20, map_str_2);
    Map map2(TMP_FILE);

    SECTION("load map correctly.") {
        // Map 应该成功加载
        REQUIRE(map.valid());
        REQUIRE(map2.valid());
    }

    SECTION("load position correctly.") {
        Position pos = map.getPos();
        REQUIRE(pos.x == 8);
        // 数不清了，大概是就行了
        REQUIRE(pos.y == 44);
    }

    SECTION("Can't move to wall.") {
        Position pos1 = map.getPos();
        REQUIRE(pos1.x == 8);
        REQUIRE(pos1.y == 44);
        map.moveProtagonist(1, event_type, id);
        REQUIRE(event_type == 4);
        REQUIRE(id == -1);
        Position pos = map.getPos();
        REQUIRE(pos.x == 8);
        REQUIRE(pos.y == 44);
    }

    SECTION("Move in empty spaces.") {
        // Before moving
        Position pos = map.getPos();
        REQUIRE(pos.x == 8);
        REQUIRE(pos.y == 44);

        Position tmp_pos;
        for (int i = 0; i < 5; ++i) {
            map.moveProtagonist(0, event_type, id);
            REQUIRE(event_type == 0);
            REQUIRE(id == -1);
            tmp_pos = map.getPos();
            REQUIRE(tmp_pos.x == pos.x - 1);
            REQUIRE(tmp_pos.y == pos.y);
            pos = tmp_pos;
        }
        for (int i = 0; i < 6; ++i) {
            map.moveProtagonist(3, event_type, id);
            REQUIRE(event_type == 0);
            REQUIRE(id == -1);
            tmp_pos = map.getPos();
            REQUIRE(tmp_pos.x == pos.x);
            REQUIRE(tmp_pos.y == pos.y - 1);
            pos = tmp_pos;
        }
    }
    SECTION("Interact with NPC.") {
        map.moveProtagonist(3, event_type, id);
        REQUIRE(event_type == 0);
        REQUIRE(id == -1);
    }

    SECTION("Interact with bed.") {
        for (int i = 0;i < 2; ++i) {
            map2.moveProtagonist(0, event_type, id);
            REQUIRE(event_type == 0);
            REQUIRE(id == -1);
            map2.moveProtagonist(1, event_type, id);
            REQUIRE(event_type == 0);
            REQUIRE(id == -1);
        }
        map2.moveProtagonist(1, event_type, id);
        REQUIRE(event_type == 3);
        REQUIRE(id == 0);
    }

    SECTION("Move to entry.") {
        for (int i = 0;i < 3; ++i) {
            map2.moveProtagonist(2, event_type, id);
            REQUIRE(event_type == 0);
            REQUIRE(id == -1);
        }
        Position pos1 = map2.getPos();
        map2.moveProtagonist(2, event_type, id);
        Position pos2 = map2.getPos();
        REQUIRE(event_type == 0);
        REQUIRE(id == -1);
        REQUIRE(pos1.x == pos2.x);
    }
    SECTION("Move to exit.") {
        map2.moveProtagonist(2, event_type, id);
        REQUIRE(event_type == 0);
        REQUIRE(id == -1);
        for (int i = 0;i < 8; ++i) {
            map2.moveProtagonist(3, event_type, id);
            REQUIRE(event_type == 0);
            REQUIRE(id == -1);
        }
        map2.moveProtagonist(3, event_type, id);
        REQUIRE(event_type == 2);
        REQUIRE(id == -1);
    }
    const auto path = Map::BASE_DIR + TMP_FILE;
    std::filesystem::remove(path);
}
