#include <iostream>
#include <atomic>
#include <iomanip>

std::atomic<int> capturedKey(-1);
std::atomic<bool> keyCaptured(false);

#if defined(_WIN32) || defined(_WIN64)
#include <Windows.h>
#include <map>

std::map<int, char> vkToAsciiMap = {
    {0x30, '0'}, {0x31, '1'}, {0x32, '2'}, {0x33, '3'}, {0x34, '4'}, {0x35, '5'}, {0x36, '6'}, {0x37, '7'}, {0x38, '8'}, {0x39, '9'}, {0x41, 'a'}, {0x42, 'b'}, {0x43, 'c'}, {0x44, 'd'}, {0x45, 'e'}, {0x46, 'f'}, {0x47, 'g'}, {0x48, 'h'}, {0x49, 'i'}, {0x4A, 'j'}, {0x4B, 'k'}, {0x4C, 'l'}, {0x4D, 'm'}, {0x4E, 'n'}, {0x4F, 'o'}, {0x50, 'p'}, {0x51, 'q'}, {0x52, 'r'}, {0x53, 's'}, {0x54, 't'}, {0x55, 'u'}, {0x56, 'v'}, {0x57, 'w'}, {0x58, 'x'}, {0x59, 'y'}, {0x5A, 'z'}, {VK_SPACE, ' '}, {VK_RETURN, '\n'}, {VK_ESCAPE, 27}};

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
            std::cout << "Key pressed: " << (char)capturedKey << std::endl;
        }
    }

    // 调用下一个钩子处理程序
    return CallNextHookEx(hKeyboardHook, nCode, wParam, lParam);
}

int waitKeyDown()
{
    capturedKey = -1;
    keyCaptured = false;

    std::cout << "Press any key (ESC to exit)..." << std::endl;

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

        if (capturedKey == 27) // 27 = ESC
        {
            break;
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

#endif

int main()
{
    while (1)
    {
        int key = waitKeyDown();

        // 检查ESC键退出
        if (key == 27)
        {
            std::cout << " ESC pressed.";
            break;
        }
        if (key != -1)
        {
            std::cout << "\nCaptured key code: " << std::dec << key << " " << (char)key;
        }
    }
    std::cout << "\nExit execution." << std::endl;
    return 0;
}