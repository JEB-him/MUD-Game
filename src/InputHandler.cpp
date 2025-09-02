/**
 * @brief 等待并获取键盘输入
 *
 * 调用 waitKeyDown 函数可获取键盘输入，支持数字、字母、空格、Esc和Enter键。
 * 函数会阻塞直到有符合条件的按键被按下。
 *
 * @return int 返回按键的ASCII码值
 * @retval 27 Esc键
 * @retval 32 空格键
 * @retval 10 回车键(Enter)
 * @retval 48-57 数字键0-9
 * @retval 97-122 字母键a-z(小写)
 * @retval -1 获取按键失败
 *
 * @note 此函数跨平台支持Windows和Linux系统
 * @note Windows使用键盘钩子实现，Linux使用ncurses库实现
 *
 * @see vkToAscii()
 */

#include <iostream>
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
    {VK_SPACE, ' '},
    {VK_RETURN, '\n'},
    {VK_ESCAPE, 27}};

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
            // std::cout << "Key pressed: " << (char)capturedKey << std::endl;
        }
    }

    // 调用下一个钩子处理程序
    return CallNextHookEx(hKeyboardHook, nCode, wParam, lParam);
}

int waitKeyDown()
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
        Sleep(50);
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
    // Linux下直接返回按键值，因为我们已经从getch()获得了ASCII字符
    return keyCode;
}

int waitKeyDown()
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

        if (ch != ERR) // 如果有按键按下
        {
            capturedKey = ch;
            keyCaptured = true;
            // 检查ESC键
            if (ch == 27) // ESC键
            {
                break;
            }
        }

        usleep(10000); // 睡眠10ms，减少CPU占用
    }

    // 清理ncurses
    endwin();

    return capturedKey;
}
#endif

// // 测试代码
// int main()
// {
//     while (1)
//     {
//         int key = waitKeyDown();
//         if (key == 27)
//         {
//             std::cout << " ESC pressed.";
//             break;
//         }
//         // 检查输出
//         if (key != -1)
//         {
//             std::cout << "Captured key code: " << std::dec << key << " " << (char)key << std::endl;
//         }
//     }
//     std::cout << " Exit executnihaoion." << std::endl;
//     return 0;
// }