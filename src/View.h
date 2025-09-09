/**
 * @file View.h
 * @details 存放视图类，该类与 Controller 类紧密耦合
 *          部分渲染实现考虑下发给 Model
 * @author Jie Jiang
 */
#include <vector>
#include <memory>
#include "tools.h"

/**
 * @brief Color 颜色的定义
 * @note 该颜色类型的定义服务于 ANSII 转义
 */
typedef int Color;

class View {
public:
    friend class Controller;
    /**
     * @brief 单例模式获取 View 的函数
     * @return 一个 View 的 shared 智能指针
     */
    std::shared_ptr<View> getInstance() const;
    /**
     * @brief 全局重绘
     * @note 丢失所有已经绘制的图形，仅保留地图，建议在窗口大小发生改变时应用此函数
     * @return Message
     */
    Message reDraw();

    /**
     * @brief 局部重绘
     * @details 重新擦出绘制制定区域，适用于操作框满需要重绘的情况
     * @param area\n
     *        0 - 重绘地图
     *        1 - 重绘日志
     *        2 - 重绘操作面板
     * @return Message
     */
    Message reDrawArea(const int& area);

    /**
     * @brief 地图中的主角移动
     * @note 该函数应被声明为 Map 的友元
     * @param last_pos 主角移动前的位置
     * #param pos 主角移动后的位置
     */
    Message drawPoMove(const Position& last_pos, const Position& pos);

    /**
     * @brief 打印一条日志
     * @param msg 一条 string 文本
     * @param color 一个 Color 类型的颜色参数，Color 定义见上
     * @return bool success
     */
    bool printLog(const std::string& msg, const Color& color);

    /**
     * @brief 输出一个问题格式的消息到操作界面
     * @param 询问的人，置空则不显示
     * @param text 消息
     * @param color 颜色
     * @return Message 消息
     */
    Message printQuestion(const std::string& person, const std::string& msg, const Color& color);

    /**
     * @brief 输出选项
     * @param options 选项列表(vector)
     * @return Message
     */
    Message printOptions(const std::vector<std::string> options);
private:
};
