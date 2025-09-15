#ifndef INPUTHANDLER_H
#define INPUTHANDLER_H

#include <atomic>
#include <map>
#include <iostream>

#if defined(_WIN32) || defined(_WIN64)
#include <Windows.h>
#else
#include <termios.h>
#include <unistd.h>
#include <sys/ioctl.h>
#endif

/**
 * @brief 等待并获取键盘输入
 * @author Yang
 *
 * 调用 waitKeyDown 函数可获取键盘输入，支持数字、字母、空格、Esc、Enter、方向、退格键、_ 等按键。
 * 函数会阻塞直到有符合条件的按键被按下。
 *
 * @return int 返回数字的原始值/字母和ESC的ASCII码值
 * @retval '0'-'9' 数字键0-9
 * @retval 10 回车键(Enter)
 * @retval 27 Esc键
 * @retval 8 退格键
 * @retval 95 '_'
 * @retval 'a'-'z' 字母键a-z(小写)
 * @retval "wsad" 对应 "上下左右"
 * @retval -1 获取按键失败
 *
 * @note 此函数跨平台支持Windows和Linux系统
 * @note Windows使用键盘钩子实现，Linux使用termios实现
 */
class InputHandler {
public:
    InputHandler();
    ~InputHandler();
    
    int waitKeyDown();
    
    // 特殊键常量
    static const int KEY_ENTER = 10;
    static const int KEY_ESC = 27;
    static const int KEY_BACKSPACE = 8;
    static const int KEY_UNDERSCORE = 95;
    static const int KEY_UP = 'w';
    static const int KEY_DOWN = 's';
    static const int KEY_LEFT = 'a';
    static const int KEY_RIGHT = 'd';
    
private:
#if defined(_WIN32) || defined(_WIN64)
    static std::atomic<int> capturedKey;
    static std::atomic<bool> keyCaptured;
    static HHOOK hKeyboardHook;
    
    static int vkToAscii(int vkCode);
    static LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam);
#else
    struct termios orig_termios;
    bool raw_mode_enabled;
    
    void enableRawMode();
    void disableRawMode();
    int readEscapeSequence();
#endif
};

#endif // INPUTHANDLER_H
