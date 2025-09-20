#include "InputHandler.h"

#if defined(_WIN32) || defined(_WIN64)
// Windows 实现
std::atomic<int> InputHandler::capturedKey(-1);
std::atomic<bool> InputHandler::keyCaptured(false);
HHOOK InputHandler::hKeyboardHook = nullptr;

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
    {VK_OEM_MINUS, '_'}, // 95
    {VK_SPACE, ' '},     // 32
    {VK_RETURN, 10},     // 10
    {VK_ESCAPE, 27},
    {VK_BACK, 8}, // 8
    {VK_UP, 'w'},
    {VK_DOWN, 's'},
    {VK_LEFT, 'a'},
    {VK_RIGHT, 'd'}};

int InputHandler::vkToAscii(int vkCode)
{
    if (vkToAsciiMap.find(vkCode) != vkToAsciiMap.end())
    {
        char ascii = vkToAsciiMap[vkCode];
        return ascii;
    }
    return '\0';
}

/* 钩子回调函数 */
LRESULT CALLBACK InputHandler::KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam)
{
    // 只有 nCode >= 0 时才处理消息
    if (nCode == HC_ACTION) {
        if (wParam == WM_KEYDOWN) {
            KBDLLHOOKSTRUCT *pKeyBoard = (KBDLLHOOKSTRUCT *)lParam;
            
            // 1. 捕获我们感兴趣的键
            int key = vkToAscii(pKeyBoard->vkCode);
            if (key != '\0') { // 只处理我们映射表中的键
                capturedKey = key;
                keyCaptured = true;
                
                // 2. 关键步骤：返回 1 以阻止此消息继续传播
                return 1;
            }
            
            // 对于其他未映射的键（如Ctrl, Alt, F1等），可以选择不拦截
            // 这样它们就能正常到达终端/其他程序
        }
    }

    // 对于我们不处理的消息，调用下一个钩子
    return CallNextHookEx(hKeyboardHook, nCode, wParam, lParam);
//    if (nCode >= 0)
//    {
//        if (wParam == WM_KEYDOWN)
//        {
//            KBDLLHOOKSTRUCT *pKeyBoard = (KBDLLHOOKSTRUCT *)lParam;
//            capturedKey = vkToAscii(pKeyBoard->vkCode);
//            keyCaptured = true;
//        }
//    }
//
//    // 调用下一个钩子处理程序
//    return CallNextHookEx(hKeyboardHook, nCode, wParam, lParam);
}

InputHandler::InputHandler()
{
    // Windows 不需要特殊初始化
}

InputHandler::~InputHandler()
{
    // Windows 不需要特殊清理
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

// Linux 实现 (使用 termios)
InputHandler::InputHandler() : raw_mode_enabled(false)
{
    // 保存原始终端设置
    tcgetattr(STDIN_FILENO, &orig_termios);
}

InputHandler::~InputHandler()
{
    // 确保退出时恢复终端设置
    if (raw_mode_enabled)
    {
        disableRawMode();
    }
}

void InputHandler::enableRawMode()
{
    if (raw_mode_enabled)
        return;

    struct termios raw = orig_termios;

    // 修改终端设置
    raw.c_lflag &= ~(ICANON | ECHO | ISIG); // 禁用规范模式、回显和信号处理
    raw.c_iflag &= ~(IXON | ICRNL);         // 禁用软件流控制和CR到NL的转换
    raw.c_oflag &= ~(OPOST);                // 禁用输出处理
    raw.c_cc[VMIN] = 0;                     // 最小字符数为0（非阻塞）
    raw.c_cc[VTIME] = 1;                    // 超时时间为0.1秒

    // 应用新设置
    tcsetattr(STDIN_FILENO, TCSANOW, &raw);
    raw_mode_enabled = true;
}

void InputHandler::disableRawMode()
{
    if (!raw_mode_enabled)
        return;

    // 恢复原始终端设置
    tcsetattr(STDIN_FILENO, TCSANOW, &orig_termios);
    raw_mode_enabled = false;
}

int InputHandler::readEscapeSequence()
{
    char seq[2];

    // 读取转义序列的后续字符
    if (read(STDIN_FILENO, &seq[0], 1) != 1)
        return KEY_ESC;
    if (read(STDIN_FILENO, &seq[1], 1) != 1)
        return KEY_ESC;

    // 解析转义序列
    if (seq[0] == '[')
    {
        switch (seq[1])
        {
        case 'A':
            return KEY_UP; // 上箭头
        case 'B':
            return KEY_DOWN; // 下箭头
        case 'C':
            return KEY_RIGHT; // 右箭头
        case 'D':
            return KEY_LEFT; // 左箭头
        }
    }

    return KEY_ESC; // 未知转义序列
}

int InputHandler::waitKeyDown()
{
    enableRawMode();

    int c = -1;
    fd_set fds;
    struct timeval tv;

    // 等待按键
    while (true)
    {
        FD_ZERO(&fds);
        FD_SET(STDIN_FILENO, &fds);

        // 设置超时时间为100毫秒
        tv.tv_sec = 0;
        tv.tv_usec = 0;

        // 检查是否有输入
        if (select(STDIN_FILENO + 1, &fds, NULL, NULL, &tv) > 0)
        {
            // 读取一个字符
            char ch;
            if (read(STDIN_FILENO, &ch, 1) == 1)
            {
                // 处理转义序列（功能键）
                if (ch == '\033')
                { // ESC字符
                    c = readEscapeSequence();
                    break;
                }
                // 处理Backspace键
                else if (ch == 127 || ch == 8)
                {
                    c = KEY_BACKSPACE;
                    break;
                }
                // 处理Enter键
                else if (ch == '\r' || ch == '\n')
                {
                    c = KEY_ENTER;
                    break;
                }
                // 处理普通字符
                else if (ch >= 32 && ch <= 126)
                {
                    // 转换为小写字母
                    if (ch >= 'A' && ch <= 'Z')
                    {
                        ch += 32;
                    }
                    c = ch;
                    break;
                }
            }
        }

        // 可以在这里添加其他处理，如检查退出条件等
    }

    disableRawMode();
    return c;
}

#endif
