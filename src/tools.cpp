#include "tools.h"
#pragma once

Position::Position(const int& x, const int& y) : x(x), y(y) {}

Message::Message(const std::string& msg, const int& status) : msg(msg), status(status) {}

SpecialChar::SpecialChar(const std::string& special_char, const int& width):
    special_char(special_char),
    width(width) {
    // 构造函数体
}

SpecialChar::SpecialChar() : special_char(), width(-1) {}
