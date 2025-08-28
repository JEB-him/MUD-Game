#include "Map.h"

Position::Position(const int& x, const int& y) : x(x), y(y) {}

Map::Map(std::string map_path, const int& x, const int& y){
    // TODO: load map
    this->x = x;
    this->y = y;
    this->map_path = std::move(map_path);
}

Position Map::getPos() const {
    return {x, y};
}
