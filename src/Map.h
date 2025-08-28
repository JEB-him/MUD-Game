/**
 * @file Map.h
 * @author Jie Jiang
 * */

#include <string>

/**
 * @brief 一个通用的坐标类，建议都是用该类来储存坐标
 */
class Position {
public:
    int x;    /// 横坐标, 该成员 public
    int y;    /// 纵坐标，该成员 public

    /**
     * @param x 横坐标，默认为 0
     * @param y 纵坐标，默认为 0
     */
    Position(const int& x=0, const int& y=0);
};

/**
 * @brief Map 类，用于读取 map 文件
 */
class Map {
public:
    Map() = default;
    /**
     * @brief 使用地图文件初始化地图
     * @param map_path String 类型
     * @param x 主角的初始横坐标
     * @param y 主角的初始纵坐标
     * @note 初始坐标默认为从左到右从上到下第一个空白位置,
     * 若在地图中检测到主角的符号（#），则参数 x, y 无效
     */
    Map(std::string map_path, const int& x=0, const int& y=0);

    /**
     * @brief 获取主角当前坐标
     * @return a Position const reference.
     */
    Position getPos() const;
private:
    // Map path
    std::string map_path;

    // Current position of 
    int x;
    int y;
};
