#include <iostream>
#include <atomic>
#include <iomanip>
#include "Controller.h"

class InputHandler
{
public:
    /**
     * @brief 获取键盘输入
     * @details 会等待用户按下键盘
     * @note 0-9的数字返回原始值（不需要转化），'a'-'z'，ESC(27)返回对应ascii码
     * @return int 数字原始值/ascii码
     */
    int waitKeyDown();
    void quitGame();
};