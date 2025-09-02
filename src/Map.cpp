#include "Map.h"
#include <fstream>

Map::Map(const std::string& map_path, const Position& pos){
    // TODO: load map
    this->is_valid = loadMap(map_path);
    this->x = pos.x;
    this->y = pos.y;
    this->map_path = std::move(map_path);
}

bool Map::valid() const {
    return this->is_valid;
}

Position Map::getPos() const {
    return {x, y};
}

int Map::loadMap(const std::string& map_path) {
    std::fstream map_file(map_path);
    std::string line;
    std::getline(map_file, line);
    return 0;
}
