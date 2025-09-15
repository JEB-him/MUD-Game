#include "tools.h"
#include "Map.h"
#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

void gameSleep(unsigned int time) {
#ifdef _WIN32
    Sleep(time);
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
    const Rgb& rgb_color):
    special_char(special_char),
    width(width),
    need_empty(need_empty),
    simple_color(simple_color),
    rgb_color(rgb_color) {
    // 构造函数体
}

SpecialChar::SpecialChar() : width(-1), need_empty(false) {}

bool isValidUsername(const std::string& username) {
    // 白名单正则表达式：
    // - 允许英文字母（大小写）、数字、下划线
    // - 允许中文字符（UTF-8编码范围）
    // 注意：中文字符在UTF-8中通常占用3个字节，范围是[\u4e00-\u9fa5]（基本多文种平面）
    // 但由于C++标准库正则表达式对Unicode支持有限，我们直接使用字节模式匹配
    static const std::regex pattern("^[A-Za-z0-9_\u4e00-\u9fa5]+$");;
    return std::regex_match(username, pattern);
}

std::string getStr(EventType type)
{
    switch (type)
    {
    case EventType::MOVE:
        return "移动主角";
    case EventType::AC_NPC:
        return "与NPC互动";
    case EventType::AC_INST:
        return "与器械互动";
    case EventType::OPEN_PACK:
        return "打开背包";
    case EventType::REFRESH:
        return "刷新地图";
    case EventType::STATUS:
        return "显示状态栏";
    case EventType::JUMP:
        return "跳转场景";
    case EventType::STORE:
        return "商店购买";
    case EventType::HELP:
        return "帮助信息";
    case EventType::USE:
        return "使用物品";
    case EventType::QUIT:
        return "退出游戏";
    case EventType::NONE:
        return "无事件";
    default:
        return "未知事件";
    }
}