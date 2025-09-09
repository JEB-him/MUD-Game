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
    MIN_WIN_HEIGHT(controller->map->getMaxHeight() + 10),
    MIN_WIN_WIDTH(controller->map->getMaxWidth() + 40){
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
    // 清屏
    cout << REASE_S;
    // 设置日志和游戏输出的最大行数
    //         窗口高度-顶部与底部还有中部分割线-地图高度
    logs_height = height - 3 - controller->map->getMaxHeight();
    logs_width = controller->map->getMaxWidth();
    puts_width = width - logs_width;
    puts_height = height;
    // 首先绘制所有框架
    // 地图是固定大小的
    // 顶部
    std::cout << BLT;
    for (int i = 0; i < controller->map->getMaxWidth(); ++i)
        std::cout << BH;
    std::cout << BMT;
    for (int i = 0; i < width - controller->map->getMaxWidth() - 3; ++i)
        std::cout << BH;
    std::cout << BRT << std::endl;
    // 中部
    for (int i = 0; i < height - 2; ++i) {
        std::cout << BV;
        cout << R_COLS(controller->map->getMaxWidth());
        std::cout << BV;
        cout << R_COLS(width - controller->map->getMaxWidth() - 3);
        std::cout << BV << std::endl;
    }
    // 中部的分割线
    cout << SAVECUS;
    cout << GOTO_XY(controller->map->getMaxHeight() + 1, 0);
    cout << BLM;
    cout << R_COLS(controller->map->getMaxWidth());
    cout << BMM;
    cout << LOADCUS;
    // 底部
    std::cout << BLB;
    for (int i = 0; i < controller->map->getMaxWidth(); ++i)
        std::cout << BH;
    std::cout << BMB;
    for (int i = 0; i < width - controller->map->getMaxWidth() - 3; ++i)
        std::cout << BH;
    std::cout << BRB << std::endl;
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
    std::string origin_str = text;
    while(origin_str.length()) {
        std::stringstream ss;
        if (simple_color=="") {
            ss << "\x1b[" << "38;2;" << rgb_color.r << ";" << rgb_color.g << ";" << rgb_color.b << "m";
        } else {
            ss << "\x1b[1;" << simple_colors.at(simple_color) << "m";
        }
        // 插入文本
        for (int i = 0;i < width; ++i) {
            ss << origin_str[0];
            origin_str.erase(0, 1);
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
