#include "tools.h"

Position::Position(const int& x, const int& y) : x(x), y(y) {}

Message::Message(const std::string& msg, const int& status) : msg(msg), status(status) {}

RGB::RGB(std::uint8_t r, std::uint8_t g, std::uint8_t b): r(r), g(g), b(b) {}

SpecialChar::SpecialChar(
    const std::string& special_char,
    const int& width,
    const std::string& simple_color,
    const RGB& rgb_color):
    special_char(special_char),
    width(width),
    simple_color(simple_color),
    rgb_color(rgb_color) {
    // 构造函数体
}

SpecialChar::SpecialChar() : width(-1) {}
