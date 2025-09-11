#include "tools.h"
#pragma once

Position::Position(const int& x, const int& y) : x(x), y(y) {}

Message::Message(const std::string& msg, const int& status) : msg(msg), status(status) {}

Rgb::Rgb(std::uint8_t r, std::uint8_t g, std::uint8_t b): r(r), g(g), b(b) {}

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

SpecialChar::SpecialChar() : need_empty(false), width(-1) {}
