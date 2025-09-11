/**
 * @file View.cpp
 * @author Jie Jiang
 */
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
#include <stdio.h>
#include <locale>     // 确保正确计算中英文字符串长度
void View::get_cursor_position(int& x, int& y) {
#ifdef __linux__
    struct termios oldt, newt;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;

    // 关闭回显和缓冲
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);

    // 隐藏光标
    printf("\033[?25l");
    fflush(stdout);
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
    std::cout << "\033[6n" << std::flush; // 请求光标位置
    std::string response;
    char ch;
    while (std::cin.get(ch)) {
        response += ch;
        if (ch == 'R') break;
    }
    int row, col;
    if (sscanf(response.c_str(), "\033[%d;%dR", &row, &col) == 2) {
        x = row, y = col;
    } else {
        x = -1, y = -1;
    }
#ifdef __linux__
    // 恢复状态
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    printf("\033[?25h"); // 显示光标
    fflush(stdout);
#elif defined(_WIN32)
    // 恢复状态
    SetConsoleMode(hIn, oldMode);
    cursorInfo.bVisible = oldVisible;
    SetConsoleCursorInfo(hOut, &cursorInfo);
#endif
}

std::shared_ptr<View> View::getInstance(std::shared_ptr<Controller> controller) {
    static auto instance = std::shared_ptr<View>(new View(controller));
    return instance;
}

View::View(std::shared_ptr<Controller> controller):
    controller(controller),
    MIN_WIN_HEIGHT(controller->map->getMaxHeight() +
                   10 +
                   TOP_MARGIN +
                   BOTTOM_MARGIN +
                   TOP_PADDING +
                   BOTTOM_PADDING),
    MIN_WIN_WIDTH(controller->map->getMaxWidth() +
                  40 +
                  LEFT_MARGIN +
                  RIGHT_MARGIN +
                  LEFT_PADDING +
                  RIGHT_PADDING){
}

bool View::reDraw() {
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
    if (width < MIN_WIN_WIDTH || height < MIN_WIN_HEIGHT) {
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
    puts_height = height - 3 - TOP_MARGIN - BOTTOM_MARGIN;
    // 清屏
    cout << REASE_S;
    // 首先绘制所有框架
    // 为了避免 Windows 和 Linux 的行为差别，统一多加一个 \r
    // 顶部
    cout << gotoXY(TOP_MARGIN + 1, LEFT_MARGIN + 1);
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
    std::cout << BRB << "\r" << std::endl;

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
    // log 和 game output 使用剩下的，需要保证不低于一定的宽度
    return true;
}

void View::colorPrint(
    const std::string& text,
    const std::string& simple_color,
    const RGB& rgb_color,
    std::deque<std::string>& outputs,
    const int& width) {
    // 添加颜色
    size_t index = 0;
    while(index < text.length()) {
        std::stringstream ss;
        if (simple_color=="") {
            ss << "\x1b[" << "38;2;" << rgb_color.r << ";" << rgb_color.g << ";" << rgb_color.b << "m";
        } else {
            ss << "\x1b[1;" << simple_colors.at(simple_color) << "m";
        }
        // 插入文本
        size_t insert_len = cutUTFString(text, index, width);
        if (insert_len == -1) {
            controller->log(Controller::LogLevel::ERR, "消息打印错误");
            return;
        }
        ss << text;
        // 转换为默认格式
        ss << "[0m";
        outputs.push_back(ss.str());
    }
    invalidate();
}

void View::invalidate() {
    // 处理游戏输出
    while(game_outputs.size() > puts_height) {
        game_outputs.pop_front();
    }
    auto puts_iter = game_outputs.rbegin();
    std::cout << SAVECUS;
    for (auto iter = game_outputs.rbegin(); iter != game_outputs.rend(); ++iter) {
        std::cout << *iter;
        std::cout << MOVU;
    }
    std::cout << LOADCUS;

}


std::string View::uLines(const int& lines) const {
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

size_t View::cutUTFString(const string& utf8_str, size_t& index, const int& width) {
    // 该函数适用于绝大多数常用字符，但对一些偏僻字符可能会出错
    size_t length = 0;
    for (; index < utf8_str.length() && length <= width; ) {
        unsigned char c = utf8_str[index];
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
            index += 3;
            length += 2;
        } else {
            // 不支持的 UTF-8 序列
            return -1;
        }
    }
    return length;
}

std::string View::charToSpecial(const int& x, const int& y, int& tx, int& ty) {
    char (*map)[Map::MAX_WIDTH] = controller->map->map;
    tx = x, ty = y;
    uint8_t wall_type = 0;
    if (map[x][y] == '#') {
        uint8_t bit_1 = 1;
        for (int i = 0, dx, dy; i < 4; ++i) {
            dx = Map::DIRECTIONS[i][0], dy = Map::DIRECTIONS[i][1];
            for (int j = 1; j <= 4; ++j) {
                int nx = x + dx * j, ny = y + dy * j;
                if (nx < 0 || nx > Map::MAX_HEIGHT || ny < 0 || ny > Map::MAX_WIDTH) continue;
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
        std::stringstream ss(Map::SPECIAL_CHARS[index].special_char);
        return ss.str();
    }

    return " ";
}

