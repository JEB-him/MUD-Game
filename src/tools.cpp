#include "tools.h"
#include "Map.h"
#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

void gameSleep(unsigned int time) {
#ifdef _WIN32
    Sleep(ms);
#else
    usleep(time * 1000); // 转换为微秒
#endif
}

Position::Position(const int& x, const int& y) : x(x), y(y) {}

bool operator == (const Position& a, const Position& b) {
    return a.x == b.x && a.y == b.y;
}

bool Position::ifInMap(const Position& pos, const Map& map) {
    if (pos.x < 0 || pos.x >= map.getMaxHeight()) return false;
    if (pos.y < 0 || pos.y >= map.getMaxWidth()) return false;
    return true;
}

Message::Message(const std::string& msg, const int& status) : msg(msg), status(status) {}

Rgb::Rgb(int r, int g, int b): r(r), g(g), b(b) {}

SpecialChar::SpecialChar(
    const std::string& special_char,
    const int& width,
    const bool& need_empty,
    const std::string& simple_color,
    const Rgb& Rgb_color):
    special_char(special_char),
    width(width),
    need_empty(need_empty),
    simple_color(simple_color),
    Rgb_color(Rgb_color) {
    // 构造函数体
}

SpecialChar::SpecialChar() : width(-1), need_empty(false) {}
