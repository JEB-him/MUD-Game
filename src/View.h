/**
 * @file View.h
 * @details 存放视图类，该类与 Controller 类紧密耦合
 *          部分渲染实现考虑下发给 Model
 * @author Jie Jiang
 */
#pragma once
#include <vector>
#include <deque>
#include <queue>
#include <memory>
#include <unordered_map>
#include "json.hpp"
#include "tools.h"
#include "Controller.h"
/**
 * @brief 光标移动到指定坐标
 * @param line 指定行
 * @param col 指定列
 * @return a ANSI Escape string
 */

/**
 * @brief 渲染类
 * @note 该类的重要原则应当时刻保证绘制完成之后光标处于控制页面输入处
 */
class View
{
public:
    friend class Controller;
    const std::unordered_map<std::string, std::string> simple_colors{
        {"black", "30"},
        {"red", "31"},
        {"green", "32"},
        {"yellow", "33"},
        {"blue", "34"},
        {"magenta", "35"},
        {"cyan", "36"},
        {"white", "37"},
        {"default", "39"}};
    /**
     * @brief 单例模式获取 View 的函数
     * @return 一个 View 的 shared 智能指针
     */
    static std::shared_ptr<View> getInstance(std::shared_ptr<Controller> controller); /**
                                                                                       * @brief 全局重绘
                                                                                       * @note 丢失所有已经绘制的图形，仅保留地图，建议在窗口大小发生改变时应用此函数
                                                                                       * @return bool
                                                                                       */
    bool reDraw();

    /**
     * @brief 局部重绘
     * @details 重新擦出绘制制定区域，适用于操作框满需要重绘的情况
     * @param area\n
     *        0 - 重绘地图
     *        1 - 重绘日志
     *        2 - 重绘操作面板
     * @return Message
     */
    Message reDrawArea(const int &area);

    /**
     * @brief 地图中的主角移动
     * @note 该函数应被声明为 Map 的友元
     * @param last_pos 主角移动前的位置
     * #param pos 主角移动后的位置
     */
    Message drawPoMove(const Position &last_pos, const Position &pos);

    /**
     * @brief 打印一条日志
     * @details 提供一个[速查表](https://gist.github.com/fnky/458719343aabd01cfb17a3a4f7296797#color-codes)\n
     * @param msg 一条 string 文本
     * @param rgb_color 一个 RGB 类型的颜色参数，RGB 定义见上
     * @param simple_color 颜色，参见 ANSI Escape 表, rgb_color 不为空时该参数被忽略
     * @return bool success
     */
    bool printLog(const std::string &msg, const std::string &simple_color, const RGB &rgb_color = RGB(-1, -1, -1));

    /**
     * @brief 输出一个问题格式的消息到操作界面
     * @param 询问的人，置空则不显示
     * @param text 消息
     * @param rgb_color 颜色
     * @param simple_color 颜色，参见 ANSI Escape 表, 该参数传递空值时使用 rgb_color
     * @return Message 消息
     */
    Message printQuestion(const std::string &person, const std::string &msg, const std::string &simple_color, const RGB &rgb_color = RGB(-1, -1, -1));

    /**
     * @brief 输出选项
     * @param options 选项列表(vector)
     * @return Message
     */
    Message printOptions(const std::vector<std::string> &options);

    /**
     * @brief 清空屏幕
     */
    static void clear() { std::cout << "\x1b[2J"; }

private:
    const int MIN_WIN_WIDTH;
    const int MIN_WIN_HEIGHT;

    // 设置日志和游戏输出的最大行数
    static constexpr int TOP_MARGIN = 0;
    static constexpr int BOTTOM_MARGIN = 3;
    static constexpr int LEFT_MARGIN = 1;
    static constexpr int RIGHT_MARGIN = 1;
    // PADDING 仅对地图窗口生效
    static constexpr int TOP_PADDING = 0;
    static constexpr int BOTTOM_PADDING = 0;
    static constexpr int LEFT_PADDING = 1;
    static constexpr int RIGHT_PADDING = 1;

    constexpr static std::string BV = "\U00002502";
    constexpr static std::string BH = "\U00002500";
    constexpr static std::string BLM = "\U0000251C";
    constexpr static std::string BMM = "\U00002524";
    constexpr static std::string BLT = "\U0000256D";
    constexpr static std::string BMT = "\U0000252C";
    constexpr static std::string BRT = "\U0000256E";
    constexpr static std::string BLB = "\U00002570";
    constexpr static std::string BMB = "\U00002534";
    constexpr static std::string BRB = "\U0000256F";

    constexpr static std::string MOVHOME = "\x1b[H";
    constexpr static std::string MOVU = "\x1bM";
    constexpr static std::string MOVD = "\n";
    constexpr static std::string SAVECUS = "\x1b[s";
    constexpr static std::string LOADCUS = "\x1b[u";
    constexpr static std::string REASE_S = "\x1b[2J";

    std::string uLines(const int &lines) const;
    std::string dLines(const int &lines) const;
    std::string rCols(const int &cols) const;
    std::string lCols(const int &cols) const;
    std::string gotoXY(const int &x, const int &y) const;

    // 日志队列
    std::deque<std::string> logs;
    int logs_height = 0;
    int logs_width = 0;
    // 游戏输出队列
    std::deque<std::string> game_outputs;
    int puts_height = 0;
    int puts_width = 0;

    // 出口和入口队列
    std::queue<Position> in_positions;
    std::queue<Position> out_positions;

    // 控制器智能指针
    std::shared_ptr<Controller> controller;

    // 构造函数，要求控制器指针
    View(std::shared_ptr<Controller> controller);

    // 获得光标位置
    void get_cursor_position(int &x, int &y);

    void colorPrint(
        const std::string &text,
        const std::string &simple_color,
        const RGB &rgb_color,
        std::deque<std::string> &outputs,
        const int &width);

    // 更新输出
    void invalidate();

    // 中英字符串截断
    size_t cutUTFString(const std::string &utf8_str, size_t &index, const int &width);

    // 特殊字符输出
    std::string charToSpecial(const int &x, const int &y, int &tx, int &ty);
};

/*
┌──────────────────────────────────┐
│                                  │
├──────────────────────────────────┤
│            内容                  │
└──────────────────────────────────┘
*/
