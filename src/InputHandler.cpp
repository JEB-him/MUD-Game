/**
 * @brief 等待并获取键盘输入
 * @author Yang
 *
 * 调用 waitKeyDown 函数可获取键盘输入，支持数字、字母、空格、Esc和Enter键。
 * 函数会阻塞直到有符合条件的按键被按下。
 *
 * @return int 返回数字的原始值/字母和ESC的ASCII码值
 * @retval '0'-'9' 数字键0-9
 * @retval 10 回车键(Enter)
 * @retval 27 Esc键
 * @retval 8 退格键
 * @retval 'a'-'z' 字母键a-z(小写)
 * @retval "wsad" 对应 "上下左右"
 * @retval -1 获取按键失败
 *
 * @note 此函数跨平台支持Windows和Linux系统
 * @note Windows使用键盘钩子实现，Linux使用ncurses库实现
 *
 * @see vkToAscii()
 */

#include "InputHandler.h"
// #include "Controller.h"
#include <iostream>
// #include <functional>

#include <atomic>
#include <iomanip>

std::atomic<int> capturedKey(-1);
std::atomic<bool> keyCaptured(false);

#if defined(_WIN32) || defined(_WIN64)
#include <Windows.h>
#include <map>

std::map<int, char> vkToAsciiMap = {
    {0x30, '0'},
    {0x31, '1'},
    {0x32, '2'},
    {0x33, '3'},
    {0x34, '4'},
    {0x35, '5'},
    {0x36, '6'},
    {0x37, '7'},
    {0x38, '8'},
    {0x39, '9'},
    {0x41, 'a'},
    {0x42, 'b'},
    {0x43, 'c'},
    {0x44, 'd'},
    {0x45, 'e'},
    {0x46, 'f'},
    {0x47, 'g'},
    {0x48, 'h'},
    {0x49, 'i'},
    {0x4A, 'j'},
    {0x4B, 'k'},
    {0x4C, 'l'},
    {0x4D, 'm'},
    {0x4E, 'n'},
    {0x4F, 'o'},
    {0x50, 'p'},
    {0x51, 'q'},
    {0x52, 'r'},
    {0x53, 's'},
    {0x54, 't'},
    {0x55, 'u'},
    {0x56, 'v'},
    {0x57, 'w'},
    {0x58, 'x'},
    {0x59, 'y'},
    {0x5A, 'z'},
    {VK_SPACE, ' '},   // 32
    {VK_RETURN, '\n'}, // 10
    {VK_ESCAPE, 27},
    {VK_BACK, '\b'}, // 8
    {VK_UP, 'w'},
    {VK_DOWN, 's'},
    {VK_LEFT, 'a'},
    {VK_RIGHT, 'd'}};

int vkToAscii(int vkCode)
{
    if (vkToAsciiMap.find(vkCode) != vkToAsciiMap.end())
    {
        char ascii = vkToAsciiMap[vkCode];
        return ascii;
    }
    return '\0';
}

HHOOK hKeyboardHook = nullptr;

/* 钩子回调函数 */
LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam)
{
    if (nCode >= 0)
    {
        if (wParam == WM_KEYDOWN)
        {
            KBDLLHOOKSTRUCT *pKeyBoard = (KBDLLHOOKSTRUCT *)lParam;
            capturedKey = vkToAscii(pKeyBoard->vkCode);
            keyCaptured = true;
        }
    }

    // 调用下一个钩子处理程序
    return CallNextHookEx(hKeyboardHook, nCode, wParam, lParam);
}

int InputHandler::waitKeyDown()
{
    capturedKey = -1;
    keyCaptured = false;

    /* 设置低级键盘钩子 */
    hKeyboardHook = SetWindowsHookEx(WH_KEYBOARD_LL, KeyboardProc, GetModuleHandle(NULL), 0);

    if (hKeyboardHook == NULL)
    {
        std::cerr << "Failed to set keyboard hook. Error: " << GetLastError() << std::endl;
        return -1;
    }

    // 消息循环
    MSG msg;
    while (!keyCaptured)
    {
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        Sleep(10);
    }

    // 清理钩子
    if (hKeyboardHook)
    {
        UnhookWindowsHookEx(hKeyboardHook);
        hKeyboardHook = nullptr;
    }

    return capturedKey;
}

#else
#include <ncurses.h>
#include <unistd.h>
#include <cstdlib>

int vkToAscii(int keyCode)
{
    // 处理特殊功能键
    switch (keyCode)
    {
    case KEY_UP:
        return 'w';
    case KEY_DOWN:
        return 's';
    case KEY_LEFT:
        return 'a';
    case KEY_RIGHT:
        return 'd';
    case 127: // 在某些终端中Backspace是127
        return 8;
    default:
        return keyCode;
    }
}

int InputHandler::waitKeyDown()
{
    capturedKey = -1;
    keyCaptured = false;

    // 初始化ncurses
    initscr();             // 初始化ncurses模式
    cbreak();              // 禁用行缓冲，使按键立即可用
    noecho();              // 不显示输入的字符
    nodelay(stdscr, TRUE); // 非阻塞输入
    keypad(stdscr, TRUE);  // 启用功能键

    int ch = -1;

    // 等待按键
    while (!keyCaptured)
    {
        ch = getch(); // 获取按键

        // 处理所有支持的按键类型
        if (ch == KEY_UP || ch == KEY_DOWN ||
            ch == KEY_LEFT || ch == KEY_RIGHT || ch == 127)
        {
            capturedKey = vkToAscii(ch);
            keyCaptured = true;
            break;
        }

        if (ch == 27 || (ch >= '0' && ch <= '9') || (ch >= 'a' && ch <= 'z')) // 如果有按键按下
        {
            capturedKey = ch;
            keyCaptured = true;
            break;
        }

        usleep(10000); // 睡10ms，减少CPU占用
    }

    // 清理ncurses
    endwin();

    // 返回小写字母
    if (capturedKey >= 'A' && capturedKey <= 'Z')
    {
        return capturedKey - 'A' + 'a';
    }

    return capturedKey;
}
#endif

// std::string InputHandler::getCmd()
// {
//     std::stringstream ss;
//     ss.str("");
//     ss.clear();

//     while (1)
//     {
//         int ch = waitKeyDown();
//         // Enter
//         if (ch == 10)
//         {
//             std::string cmd = ss.str();
//             return cmd;
//         }

//         // Backspace
//         else if (ch == 8)
//         {
//             std::string content = ss.str();
//             content.pop_back();
//             ss.str(content);
//             ss.clear();
//             ss << content;
//             continue;
//         }

//         // Unexpect input
//         else if (ch == 0)
//         {
//             continue;
//         }

//         else
//             // 将ss实时显示在屏幕上
//             // 需要确定接口...
//             ss << char(ch);
//     }
// }

std::string InputHandler::getCmd()
{
    std::string currentInput;

    std::cout << "Enter command: ";
    std::cout.flush();

    while (true)
    {
        int ch = waitKeyDown();

        // Enter - 确认输入
        if (ch == 10)
        {
            std::cout << std::endl; // 换行
            return currentInput;
        }
        // Backspace
        else if (ch == 8)
        {
            if (!currentInput.empty())
            {
                currentInput.pop_back();
                // 退格：移动光标，输出空格，再退格
                std::cout << "\b \b";
                std::cout.flush();
            }
        }
        // ESC - 取消输入
        else if (ch == 27)
        {
            std::cout << "^[Cancelled" << std::endl;
            return "";
        }
        // 普通可打印字符
        else if (ch >= 32 && ch <= 126)
        {
            char character = static_cast<char>(ch);
            currentInput += character;
            std::cout << character;
            std::cout.flush();
        }
        // 方向键（wsad）也作为可输入字符
        else if (ch == 'w' || ch == 'a' || ch == 's' || ch == 'd')
        {
            currentInput += static_cast<char>(ch);
            std::cout << static_cast<char>(ch);
            std::cout.flush();
        }
    }
}

int main()
{
    InputHandler inputHandler;
    int key = -1;
    std::cout << "Press a key (0-9, a-z, Esc to quit): ";
    // while (1)
    // {
    //     key = inputHandler.waitKeyDown();
    //     if (key != -1)
    //     {
    //         std::cout << "\nYou pressed: " << std::setw(2) << key << " (ASCII: " << std::setw(3) << key << ")\n";
    //     }
    //     else if (key == 27)
    //     {
    //         std::cout << "\nExiting...\n";
    //         break;
    //     }
    //     else
    //     {
    //         std::cout << "\nFailed to get key press.\n";
    //     }
    // }
    std::string cmd = inputHandler.getCmd();
    std::cout << "You entered: " << cmd << std::endl;
    return 0;
}