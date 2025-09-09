/**
 * @file View.h
 * @details 存放视图类，该类与 Controller 类紧密耦合
 *          部分渲染实现考虑下发给 Model
 */

class View {
public:
    friend class Controller;
    /**
     * @brief 全局重绘
     * @note 丢失所有已经绘制的图形，仅保留地图，建议在
     */
private:
};
