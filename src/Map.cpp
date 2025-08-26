/**
 * @file Map.h
 * @author Jie Jiang
 * */

#include <string>

class Position {

};

/**
 * @breif Map 类，用于读取 map 文件
 */
class Map {
public:
    Map() = default;
    /**
     * @brief 使用地图文件初始化地图
     * @param map_path String 类型
     * @param x 主角的初始横坐标
     * @param y 主角的初始纵坐标
     * @note 初始坐标默认为从左到右从上到下第一个空白位置
     */
    Map(std::string map_path);

    /**
     * @brief 获取主角当前坐标
     * @return a Position const reference.
     */
    const Position& getPos() const;
};
