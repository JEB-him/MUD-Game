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


bool isValidUsername(const std::string& username) {
    // 白名单正则表达式：
    // - 允许英文字母（大小写）、数字、下划线
    // - 允许中文字符（UTF-8编码范围）
    // 注意：中文字符在UTF-8中通常占用3个字节，范围是[\u4e00-\u9fa5]（基本多文种平面）
    // 但由于C++标准库正则表达式对Unicode支持有限，我们直接使用字节模式匹配
    static const std::regex pattern("^[A-Za-z0-9_\u4e00-\u9fa5]+$");;
    return std::regex_match(username, pattern);
}
