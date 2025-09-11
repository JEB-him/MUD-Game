#pragma once
#include <iostream>
#include <atomic>
#include <iomanip>
#include <sstream>

#include "tools.h"
// #include <functional>

class InputHandler
{
public:
    friend class Controller;

    /**
     * @brief 获取键盘输入
     * @details 会等待用户按下键盘
     * @note '0'-'9','a'-'z'，ESC(27),'enter'返回对应ascii码
     * @note '上下左右' 转换成 'wsad'
     * @return int ascii码
     */
    int waitKeyDown();

    std::string getCmd();

private:
    std::stringstream ss;
};