/**
 * @file View.cpp
 * @author Jie Jiang
 */

#include "Controller.h"
#include "tools.h"
#include "Protagonist.h"
#include "NPC.h"
#include "InputHandler.h"
#include "Map.h"
#include "backpack.h"
#include "View.h"
#if defined(__linux__)
#   include <unistd.h>
#   include <sys/ioctl.h>
#   include <termios.h>
#elif defined(_WIN32)
#   include <windows.h>
#   include <conio.h>
#endif
#include <iostream>
#include <string>

void View::enableCursor() {
#if defined(__linux__)
    std::cout << "\x1b[?25h";
    std::cout.flush();
#elif defined(__WIN32)
    HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(handle, &cursorInfo);
    cursorInfo.bVisible = true; // 显示光标
    SetConsoleCursorInfo(handle, &cursorInfo);
#endif
}

void View::disableCursor() {
#if defined(__linux__)
    // 使用ANSI转义序列隐藏光标
    std::cout << "\x1b[?25l";
    std::cout.flush();
#elif defined(__WIN32)
    HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(handle, &cursorInfo);
    cursorInfo.bVisible = false; // 隐藏光标
    SetConsoleCursorInfo(handle, &cursorInfo);
#endif
}

void View::get_cursor_position(int& x, int& y) {
#ifdef __linux__
    struct termios oldt, newt;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;

    // 关闭回显和缓冲
    newt.c_lflag &= static_cast<tcflag_t>(~(ICANON | ECHO));
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);

    // 隐藏光标
    std::cout << "\x1b[?25l";
    std::cout.flush();
#elif defined(_WIN32)
    HANDLE hIn = GetStdHandle(STD_INPUT_HANDLE);
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD mode;
    GetConsoleMode(hIn, &mode);

    // 保存原始状态
    DWORD oldMode = mode;
    CONSOLE_CURSOR_INFO cursorInfo;
    GetConsoleCursorInfo(hOut, &cursorInfo);
    bool oldVisible = cursorInfo.bVisible;

    // 设置为逐字符 + 无回显
    mode &= ~(ENABLE_LINE_INPUT | ENABLE_ECHO_INPUT);
    SetConsoleMode(hIn, mode);

    // 隐藏光标
    cursorInfo.bVisible = FALSE;
    SetConsoleCursorInfo(hOut, &cursorInfo);
#endif
    std::cout << "\x1b[6n" << std::flush; // 请求光标位置
    std::string response;
    char ch;
    while (std::cin.get(ch)) {
        response += ch;
        if (ch == 'R') break;
    }
    int row, col;
    if (sscanf(response.c_str(), "\x1b[%d;%dR", &row, &col) == 2) {
        x = row, y = col;
    } else {
        x = -1, y = -1;
    }
#ifdef __linux__
    // 恢复状态
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
#elif defined(_WIN32)
    // 恢复状态
    SetConsoleMode(hIn, oldMode);
    cursorInfo.bVisible = oldVisible;
    SetConsoleCursorInfo(hOut, &cursorInfo);
#endif
}

std::shared_ptr<View> View::getInstance() {
    static auto instance = std::shared_ptr<View>(new View());
    return instance;
}

View::View():
    controller(Controller::getInstance()) {
    disableCursor();
}

View::~View() {
    enableCursor();
}

bool View::reDraw() {
    // 获取屏幕高度和宽度
#if defined(__linux__)
    struct winsize current_ws;
    ioctl(STDIN_FILENO, TIOCGWINSZ, &current_ws);
    int width = current_ws.ws_col, height = current_ws.ws_row;
#elif defined(_WIN32)
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    int width = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    int height = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
#endif
    // 检测地图指针是否悬空
    if (controller->map == nullptr) {
        controller->log(Controller::LogLevel::ERR, "悬空 Map");
        return false;
    } else if (!controller->map->valid()) {
        controller->log(Controller::LogLevel::ERR, controller->map->getValidMsg());
    }

    // 设置最小宽度和高度
    min_win_width = controller->map->getMaxWidth() +
                  40 +
                  LEFT_MARGIN +
                  RIGHT_MARGIN +
                  LEFT_PADDING +
                  RIGHT_PADDING;
    min_win_height = controller->map->getMaxHeight() +
                   10 +
                   TOP_MARGIN +
                   BOTTOM_MARGIN +
                   TOP_PADDING +
                   BOTTOM_PADDING;

    if (width < min_win_width || height < min_win_height) {
        return false;
    }
    int map_width =  controller->map->getMaxWidth();
    int map_height = controller->map->getMaxHeight();
    // 设置日志和游戏输出的最大行数
    // 窗口高度-顶部与底部还有中部分割线-地图高度-预留空白
    logs_height = (height - 3 - map_height - TOP_MARGIN -
                   TOP_PADDING - BOTTOM_MARGIN - BOTTOM_PADDING);
    logs_width = map_width + LEFT_PADDING + RIGHT_PADDING;
    // 窗口宽度-日志宽度-边框-预留空白
    puts_width = width - logs_width - 3 - LEFT_MARGIN - RIGHT_MARGIN;
    // 留出命令输入的位置
    puts_height = height - 3 - TOP_MARGIN - BOTTOM_MARGIN - 1;
    // 清屏
    std::cout << REASE_S << MOVHOME << std::flush;
    // 首先绘制所有框架
    // 为了避免 Windows 和 Linux 的行为差别，统一多加一个 \r
    // 顶部
    std::cout << gotoXY(TOP_MARGIN + 1, LEFT_MARGIN + 1);
    std::cout << BLT;
    for (int i = 0; i < map_width + LEFT_PADDING + RIGHT_PADDING; ++i)
        std::cout << BH;
    std::cout << BMT;
    for (int i = 0; i < puts_width; ++i)
        std::cout << BH;
    std::cout << BRT << "\r" << std::endl;
    // 中部
    for (int i = 0; i < height - 2 - TOP_MARGIN - BOTTOM_MARGIN; ++i) {
        std::cout << rCols(LEFT_MARGIN) << BV;
        std::cout << rCols(logs_width);
        std::cout << BV;
        std::cout << rCols(puts_width);
        std::cout << BV << "\r" << std::endl;
    }
    // 中部的分割线
    std::cout << SAVECUS;
    // 地图高度+边框+顶部空白
    std::cout << gotoXY(map_height + 1 + TOP_MARGIN + TOP_PADDING + BOTTOM_PADDING + 1, LEFT_MARGIN + 1);
    std::cout << BLM;
    for (int i = 0; i < logs_width; ++i) {
        std::cout << BH;
    }
    std::cout << BMM;
    std::cout << LOADCUS;
    // 底部
    std::cout << rCols(LEFT_MARGIN) << BLB;
    for (int i = 0; i < logs_width; ++i)
        std::cout << BH;
    std::cout << BMB;
    for (int i = 0; i < puts_width; ++i)
        std::cout << BH;
    std::cout << BRB;

    // 打印地图
    std::cout << SAVECUS;
    std::cout << gotoXY(TOP_MARGIN + 2, LEFT_MARGIN + 1 + LEFT_PADDING + 1);
    for (int i = 0, tx, ty; i < map_height; ++i) {
        for (int j = 0; j < map_width; ++j) {
            std::cout << charToSpecial(i, j, tx, ty);
            i = tx, j = ty;
        }
        std::cout << "\r" << std::endl << rCols(LEFT_MARGIN + 1 + LEFT_PADDING);
    }
    while(!in_positions.empty()) {
        auto& pos = in_positions.front();
        std::cout << gotoXY(pos.x, pos.y) << "\x1b[48;2;126;192;12m \x1b[0m";
        std::cout << gotoXY(pos.x + 1, pos.y) << "\x1b[48;2;126;192;12m \x1b[0m";
        in_positions.pop();
    }
    while(!out_positions.empty()) {
        auto& pos = out_positions.front();
        std::cout << gotoXY(pos.x, pos.y) << "\x1b[48;2;51;102;255m \x1b[0m";
        std::cout << gotoXY(pos.x + 1, pos.y) << "\x1b[48;2;51;102;255m \x1b[0m";
        out_positions.pop();
    }
    std::cout << LOADCUS;
    for (int i = 0; i < puts_width + 1; ++i)
        std::cout << "\b";
    std::cout << SAVECUS;
    std::cout << MOVU;
    std::cout << "\b\x1b[38;5;214m\x1b[1m$\x1b[0m";
    std::cout << MOVU << "\b" << BLM;
    for (int i = 0; i < puts_width; ++i)
        std::cout << BH;
    std::cout << BMM;
    std::cout << LOADCUS;
    return true;
}

Message View::drawPoMove(const Position& last_pos, const Position& pos) {
    if (last_pos == pos) {
        return {"位置相同", 1};
    }
    if (!Position::ifInMap(last_pos, *(controller->map)) || !Position::ifInMap(pos, *(controller->map))) {
        return {"不合法的位置", -1};
    }
    std::cout << SAVECUS;
    gotoMap(last_pos);
    const auto& protago = controller->map->SPECIAL_CHARS[Map::PROTAGONIST_INDEX];
    for (int i = 0;i < protago.width; ++i) {
        std::cout << " ";
    }
    std::cout << std::flush;
    gotoMap(pos);
    std::cout << "\x1b[38;5;87m";
    std::cout << protago.special_char;
    std::cout << "\x1b[0m";
    std::cout << std::flush;
    std::cout << LOADCUS;
    return {"Success", 0};
}

Message View::printLog(const std::string& msg, const std::string& simple_color, const  Rgb& rgb_color) {
    if (logs_width <= 0) {
        return {"窗口过小", 1};
    }
    colorPrint(msg, simple_color, rgb_color, logs, logs_width);
    return {"Success", 0};
}

Message View::printQuestion(const std::string &person, const std::string &msg, const std::string &simple_color, const Rgb &rgb_color)
{
    if (logs_width <= 0) {
        return {"窗口过小", 1};
    }
    std::string text = msg;
    if (person.length()) {
        text = person + ": " + msg;
    }
    colorPrint(text, simple_color, rgb_color, game_outputs, puts_width);
    return {"Success", 0};
}

Message View::printOptions(const std::vector<std::string>& options) {
    if (logs_width <= 0) {
        return {"窗口过小", 1};
    }
    for (auto& msg : options) {
        std::string text = msg;
        colorPrint(msg, "white", Rgb(255, 255, 255), game_outputs, puts_width);
    }
    return {"Success", 0};
}

Message View::printCmd(const std::string& cmd) {
    std::cout << SAVECUS << MOVU;
    std::cout << "\x1b[38;5;214m\x1b[1m " << cmd << "\x1b[0m";
    for (size_t i = 0; i < static_cast<size_t>(puts_width) - cmd.length() - 1; ++i)
        std::cout << " ";
    std::cout  << LOADCUS;
    // 刷新缓冲区
    std::cout << std::flush;
    return {"Success", 0};
}

void View::colorPrint(
    const std::string &text,
    const std::string &simple_color,
    const  Rgb & rgb_color,
    std::deque<std::string> &outputs,
    const int &width) {
    // 添加颜色
    size_t index = 0, old_index = 0;
    while (index < text.length()) {
        std::stringstream ss;
        if (simple_color == "") {
            ss << "\x1b[" << "38;2;" <<  rgb_color.r << ";" <<  rgb_color.g << ";" <<  rgb_color.b << "m";
        } else {
            ss << "\x1b[1;" << simple_colors.at(simple_color) << "m";
        }
        // 插入文本
        size_t insert_len = cutUTFString(text, index, width);
        if (insert_len > static_cast<size_t>(width)) {
            controller->log(Controller::LogLevel::ERR, "消息打印错误");
            gameSleep(1500);
            return;
        }
        ss << text.substr(old_index, index - old_index);
        size_t len = index - old_index;
        while(len < static_cast<size_t>(width)) {
            len++;
            ss << " ";
        };
        old_index = index;
        // 转换为默认格式
        ss << "\x1b[0m";
        outputs.push_back(ss.str());
    }
    invalidate();
}

void View::invalidate() {
    // 处理游戏输出
    while(game_outputs.size() > static_cast<size_t>(puts_height)) {
        game_outputs.pop_front();
    }
    std::cout << SAVECUS;
    // 移动到全部游戏输出的首行
    std::cout << MOVHOME;
    std::cout << dLines(TOP_MARGIN + 1);
    std::cout << rCols(LEFT_MARGIN + 1 + logs_width + 1);
    int next_x = 0, next_y = 0;
    get_cursor_position(next_x, next_y);
    // 输出所有游戏输出
    for (auto iter = game_outputs.begin(); iter != game_outputs.end(); ++iter) {
        std::cout << gotoXY(next_x, next_y);
        std::cout << *iter;
        next_x += 1;
    }
    std::cout << LOADCUS;

    // 处理日志输出
    while(logs.size() > static_cast<size_t>(logs_height)) {
        logs.pop_front();
    }
    std::cout << SAVECUS;
    // 移动到日志输出的首行
    std::cout << MOVHOME;
    std::cout << dLines(TOP_MARGIN + 1 + TOP_PADDING + controller->map->getMaxHeight() + BOTTOM_PADDING + 1);
    std::cout << rCols(LEFT_MARGIN + 1);
    get_cursor_position(next_x, next_y);
    for (auto iter = logs.begin(); iter != logs.end(); ++iter) {
        std::cout << gotoXY(next_x, next_y);
        std::cout << *iter;
        next_x += 1;
    }
    std::cout << LOADCUS;
    // 刷新缓冲区
    std::cout << std::flush;
}

std::string View::uLines(const int &lines) const {
    std::stringstream ss;
    ss << "\x1b[" << lines << "A";
    return ss.str();
}
std::string View::dLines(const int& lines) const {
    std::stringstream ss;
    ss << "\x1b[" << lines << "B";
    return ss.str();
}
std::string View::rCols(const int& cols) const {
    std::stringstream ss;
    ss << "\x1b[" << cols << "C";
    return ss.str();
}
std::string View::lCols(const int& cols) const {
    std::stringstream ss;
    ss << "\x1b[" << cols << "D";
    return ss.str();
}

std::string View::gotoXY(const int& x, const int& y) const {
    std::stringstream ss;
    ss << "\x1b[" << x << ";" << y << "f";
    return ss.str();
}

size_t View::cutUTFString(const std::string& utf8_str, size_t& index, const int& width) {
    // 该函数适用于绝大多数常用字符，但对一些偏僻字符可能会出错
    size_t length = 0;
    for (; index < utf8_str.length() && length < static_cast<size_t>(width) - 1; ) {
        unsigned char c = static_cast<unsigned char>(utf8_str[index]);
        if (c < 0x80) {
            // ASCII 字符（英文字母、数字等）
            index += 1;
            ++length;
        } else if ((c & 0xE0) == 0xC0) {
            // 2字节 UTF-8 字符
            index += 2;
            ++length;
        } else if ((c & 0xF0) == 0xE0) {
            // 3字节 UTF-8 字符（包括大部分中文）
            // 字节3 UTF
            index += 3;
            length += 2;
        } else {
            // 不支持的 UTF-8 序列
            return 0xFFFF;
        }
    }

    return length;
}

std::string View::charToSpecial(const int &x, const int &y, int &tx, int &ty) {
    char (*map)[Map::MAX_WIDTH] = controller->map->map;
    tx = x, ty = y;
    int wall_type = 0;
    if (map[x][y] == '#') {
        int bit_1 = 1;
        for (int i = 0, dx, dy; i < 4; ++i) {
            dx = Map::DIRECTIONS[i][0], dy = Map::DIRECTIONS[i][1];
            for (int j = 1; j <= 4; ++j) {
                int nx = x + dx * j, ny = y + dy * j;
                if (nx < 0 || nx > Map::MAX_HEIGHT || ny < 0 || ny > Map::MAX_WIDTH)
                    continue;
                if (map[nx][ny] == '#' || ((map[nx][ny] == 'i' || map[nx][ny] == 'o') && (i == 0 || i == 4))) {
                    wall_type |= bit_1 << i;
                }
            }
        }
        switch (wall_type) {
            case 0b0001:
            case 0b0100:
            case 0b0101:
                return Map::SPECIAL_CHARS[8].special_char;
            case 0b0010:
            case 0b1000:
            case 0b1010:
                return Map::SPECIAL_CHARS[7].special_char;
            case 0b0011:
                return Map::SPECIAL_CHARS[4].special_char;
            case 0b0110:
                return Map::SPECIAL_CHARS[3].special_char;
            case 0b0111:
                return Map::SPECIAL_CHARS[12].special_char;
            case 0b1001:
                return Map::SPECIAL_CHARS[6].special_char;
            case 0b1011:
                return Map::SPECIAL_CHARS[11].special_char;
            case 0b1100:
                return Map::SPECIAL_CHARS[5].special_char;
            case 0b1101:
                return Map::SPECIAL_CHARS[13].special_char;
            case 0b1110:
                return Map::SPECIAL_CHARS[10].special_char;
            case 0b1111:
                return Map::SPECIAL_CHARS[14].special_char;
            default:
                return " ";

        }
    }

    if (map[x][y] == 'o') {
        if (y >= 1 && y + 4 < Map::MAX_WIDTH && map[x][y-1] == '#' && map[x][y + 4] == '#') {
            ty = y + 3;
            return "\x1b[48;2;51;102;255m    \x1b[0m";
        }
        if (x >= 1 && x + 2 < Map::MAX_HEIGHT && map[x-1][y] == '#' && map[x + 2][y] == '#') {
            int oldx, oldy;
            get_cursor_position(oldx, oldy);
            out_positions.push({oldx, oldy});
            return " ";
        }
    } else if (map[x][y] == 'i') {
        if (y >= 1 && y + 4 < Map::MAX_WIDTH && map[x][y-1] == '#' && map[x][y + 4] == '#') {
            ty = y + 3;
            return "\x1b[48;2;126;192;12m    \x1b[0m";
        }
        
        if (x >= 1 && x + 2 < Map::MAX_HEIGHT && map[x-1][y] == '#' && map[x + 2][y] == '#') {
            int oldx, oldy;
            get_cursor_position(oldx, oldy);
            in_positions.push({oldx, oldy});
            return " ";
        }
    }

    int index = Map::char2index(map[x][y]);
    if(index != -1 && Map::SPECIAL_CHARS[index].width != -1) {
        if (!Map::SPECIAL_CHARS[index].need_empty) {
            ty += Map::SPECIAL_CHARS[index].width - 1;
        }
        auto rgb_color = Map::SPECIAL_CHARS[index].rgb_color;
        auto simple_color = Map::SPECIAL_CHARS[index].simple_color;
        std::stringstream ss;
        if (simple_color == "") {
            ss << "\x1b[" << "38;2;" <<  rgb_color.r << ";" <<  rgb_color.g << ";" <<  rgb_color.b << "m";
        } else {
            ss << "\x1b[1;" << simple_colors.at(simple_color) << "m";
        }
        ss << Map::SPECIAL_CHARS[index].special_char;
        ss << "\x1b[0m";
        return ss.str();
    }

    return " ";
}

bool View::gotoMap(const Position& pos) {
    if (!Position::ifInMap(pos, *(controller->map))) {
        controller->log(Controller::LogLevel::DEBUG, "Not in Map");
        return false;
    }
    int bx = TOP_MARGIN + 1 + TOP_PADDING + 1;
    int by = LEFT_MARGIN + 1 + LEFT_PADDING + 1;
    std::cout << gotoXY(pos.x + bx, pos.y + by);
    return true;
}
