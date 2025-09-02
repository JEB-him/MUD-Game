/**
 * @file Map.h
 * @author Jie Jiang
 * */
#pragma once
#include <string>
#include "tools.h"

/**
 * @brief Map 类，用于读取 map 文件
 * @details 关于地图文件的一些规定：\n
 *          1. 使用 # 来表示墙壁\n
 *          2. 使用 0-9,a-z,A-Z 共 61 个字符来表示所有特殊对象，如床、主角\n
 *          3. 入口和出口宽度/高度只能为 4\n
 */
class Map {
public:
    /**
     * @brief SPECIAL_CHARS 的最大长度
     */
    constexpr static int CHAR_MAXN = 61;
    /**
     * @brief 所有特殊符号的定义
     * @note TODO 后期考虑把这些配置迁移到一个专门的配置文件中
     */
    inline const static SpecialChar SPECIAL_CHARS[CHAR_MAXN] = {
        SpecialChar("\U000f1302", 2),
        SpecialChar("\U0000c6c3", 2),
        SpecialChar("\U000f0c3c", 2),
        SpecialChar("\U00002554", 1),
        SpecialChar("\U0000255a", 1),
        SpecialChar("\U00002557", 1),
        SpecialChar("\U0000255d", 1),
        SpecialChar("\U00002550", 1),
        SpecialChar("\U00002551", 1)
    };

    Map() = default;
    /**
     * @brief 使用地图文件初始化地图
     * @param map_path String 类型
     * @param pos 设置主角的初始坐标
     * @note 初始坐标默认为从入口内侧的空白位置（取中点）,
     * 若在地图中检测到主角的符号（#），则参数 pos 无效\n
     * 现在在哪一个场景应该由 Controller 统一管理
     */
    Map(const std::string& map_path, const Position& pos=Position());

    /**
     * @brief 析构函数，Map 应当将当前地图下的所有修改保存到文件中
     * @note 只有地图有效才会进行保存
     */
    ~Map() = default;

    /**
     * @brief 返回地图加载是否成功
     * @details 要求地图尺寸不大于 100x50 （宽x高）
     * @return a bool.
     */
    bool valid() const;

    /**
     * @brief 若地图加载不成功，可以调用此函数来获得信息
     * @return 一个string常量
     */
    const std::string& getValidMsg() const;

    /**
     * @brief 获取主角当前坐标
     * @return a Position.
     */
    Position getPos() const;

    /**
     * @brief 移动主角
     * @param[in] direction 方向\n
     *            0: 向上\n
     *            1: 向右\n
     *            2: 向下\n
     *            3: 向左\n
     * @param[out] event_type 事件类型, 一个引用\n
     *             0: 抵达入口/空地 位置变化,无事件发生\n
     *             1: 碰撞NPC，位置不变，但 Controller 应当与 NPC 交互\n
     *             2: 抵达出口，位置变化，Controller 应当向场景类请求下一个场景
     *             的编号\n
     *             3: 碰撞器械，位置不变，Controller 应当与物品类交互\n
     *             4: 碰撞墙壁，位置不变\n
     * @param[out] id 出口/NPC/器械编号\n
     *             若位置在出口，为出口id, 若碰撞NPC，为 NPC id, 器械同理\n
     *             
     * @note 该函数会通过修改 event_type 来告诉 Controller 事件类型
     * @return a Message.
     */
    Message moveProtagonist(const int& direction, int& event_type, int& id);
private:
    bool        is_valid;     // 该地图类是否有效
    std::string valid_msg;    // 关于地图是否有效的消息
    // 方向数组
    inline static int directions[4][2] = {
        { 0, -1},
        { 1,  0},
        { 0,  1},
        {-1,  0}
    };
    // Map path
    std::string map_path;

    // Current position of protagonist.
    int x;
    int y;

    // 入口s、出口

    // 这些函数都不会被放到文档里面，因为私有的不会被放进去，至于原因，显然是因为
    // 这些方法都只有你自己会用，注释只要能看懂就行，不用在意格式
    // 当然还是要写一下注释，否则别人就看不懂你的代码了
    // 特别提醒，一定要注意命名风格，莫名奇妙的pinyin和缩写会让代码的可读
    // 性很低。
    
    // 我这里就还是用 doxygen 语法了，毕竟可读性很高
    /**
     * @brief 加载地图
     * @param map_path 地图文件路径
     */
    static int loadMap(const std::string& map_path);
};
