/**
 * @file Map.h
 * @author Jie Jiang
 * */
#pragma once
#include <string>
#include "tools.h"
class Controller;

class View;
/**
 * @brief Map 类，用于读取 map 文件
 * @details 关于地图文件的一些规定：\n
 *          1. 使用 # 来表示墙壁\n
 *          2. 使用 0-9,a-z,A-Z 共 62 个字符来表示所有特殊对象，如床、主角\n
 *          3. 入口和出口宽度只能为 4，高度只能为 2\n
 *          4. 初始坐标默认为从入口内侧的空白位置（取中点）,
 *          5. 若在地图中检测到主角的符号（由 Special Chars 列表决定）则参数 pos 无效\n
 *          7. 加载地图文件时将会删除上方的空白\n
 *          8. 要求地图尺寸不大于 100x50 （宽x高）
 *          9. 为了避免 Map 检测到地图尺寸异常，请不要在文件末尾添加空行
 *         10. 为了同时兼容 Linux 和 Windows, 地图路径只能是一个文件名，不能有任何 `/` 或 '\' 符号
 * @note 该类的重要原则应当是保证任何状态下 Map 类中的所有成员全部设置正确
 */

class Map
{
public:
    friend class View;
    friend class Controller;
    /**
     * @brief 地图最大宽度
     */
    constexpr static int MAX_WIDTH = 100;
    /**
     * @brief 地图最大高度
     */
    constexpr static int MAX_HEIGHT = 50;
    /**
     * @brief SPECIAL_CHARS 的最大长度
     */
    constexpr static int CHAR_MAXN = 62;
    /**
     * @brief 所有特殊符号的定义
     * @note TODO 后期考虑把这些配置迁移到一个专门的配置文件中\n
     *       请按照顺序填写
     */
    inline const static SpecialChar SPECIAL_CHARS[CHAR_MAXN] = {
        SpecialChar("\U000f1302", 2),
        SpecialChar("\U0000c6c3", 2, false),
        SpecialChar("\U000f0c3c", 2),
        SpecialChar("\U00002554", 1),
        SpecialChar("\U0000255a", 1),
        SpecialChar("\U00002557", 1),
        SpecialChar("\U0000255d", 1),
        SpecialChar("\U00002550", 1),
        SpecialChar("\U00002551", 1),
        SpecialChar("\U0000c6c3", 2, false, "blue"),
        SpecialChar("\U00002566", 1),
        SpecialChar("\U00002569", 1),
        SpecialChar("\U00002560", 1),
        SpecialChar("\U00002563", 1),
        SpecialChar("\U0000256c", 1),
        SpecialChar("\U000f0b59", 2),
        SpecialChar("\U0000f0b1", 2),
        SpecialChar("\U0000ed7c", 2),
        SpecialChar(" ", 4), ///< 入口
        SpecialChar("\U000f05f2", 2),
        SpecialChar("\U000f1a20", 2),
        SpecialChar("", -1),
        SpecialChar("", -1),
        SpecialChar("", -1),
        SpecialChar(" ", 4), ///< 出口
        SpecialChar("\U000f04b8", 2),
        SpecialChar("\U0000c6c3", 2, false),
        SpecialChar("", -1),
        SpecialChar("\U0000c6c3", 2, false),
        SpecialChar("\U000f0851", 2),
        SpecialChar("\U0000f96d", 2)
    };
    /**
     * @brief 主角索引
     */
    constexpr static int PROTAGONIST_INDEX = 1;

    Map() = default;
    /**
     * @brief 使用地图文件初始化地图
     * @param filename String 类型
     * @param pos 设置主角的初始坐标
     */
    Map(const std::string &filename, const Position &pos = {-1, -1});

    /**
     * @brief 析构函数，Map 应当将当前地图下的所有修改保存到文件中
     * @note 只有地图有效且地图被修改时才会进行保存, 通过析构函数来保存地图并不稳定,
     *       如果有任何错误，析构函数都不会反馈任何信息, 因此，Controller
     *       应当确保在释放 Map 之前调用 save 保存地图，以确保游戏的稳定运行。
     */
    ~Map();

    /**
     * @brief 返回地图加载是否成功
     * @return a bool.
     */
    bool valid() const;

    /**
     * @brief 若地图加载不成功，可以调用此函数来获得信息
     * @return 一个string常量引用
     */
    const std::string &getValidMsg() const;

    /**
     * @brief 获取主角当前坐标
     * @return a Position.
     */
    Position getPos() const;

    /**
     * @brief 获取地图最大宽度
     * @return a int
     */
    int getMaxWidth() const;

    /**
     * @brief 获取地图最大高度
     * @return a int
     */
    int getMaxHeight() const;

    /**
     * @brief 移动主角
     * @param[in] direction 方向\n
     *            0: 向上\n
     *            1: 向右\n
     *            2: 向下\n
     *            3: 向左\n
     * @param[out] event_type 事件类型, 一个引用\n
     *             0: 抵达入口/空地 位置变化,无事件发生(入口位置不变)\n
     *             1: 碰撞NPC，位置不变，但 Controller 应当与 NPC 交互\n
     *             2: 抵达出口，位置变化，Controller 应当向场景类请求下一个场景
     *             的编号\n
     *             3: 碰撞器械，位置不变，Controller 应当与物品类交互\n
     *             4: 碰撞墙壁，位置不变\n
     * @param[out] id 出口/NPC/器械编号\n
     *             若位置在出口，为出口id, 若碰撞NPC，为 NPC id\n
     *             器械为 SPECIAL_CHARS 中的索引\n
     * @note 该函数会通过修改 event_type 来告诉 Controller 事件类型
     * @return a Message.
     *
     */
    Message moveProtagonist(const int &direction, EventType &event_type, int &id);

    /**
     * @brief 传送主角到某一个地点
     * @details 支持地图内传送到某个 NPC 的附近\n
     *          一旦成功传送，应当建立一个坐标索引，减少搜索耗时\n
     *          默认从该对象的下、右、上、左的顺序寻找空位\n
     *          若无法找到空位，寻找从左到右，从上到下的第一个空位
     *          TODO 尚未实现空位寻找
     * @param ind 传送目的地（对象）的 ID 或索引
     * @param type 传送对象的类别，器械旁/NPC旁/出口入口\n
     *        0: NPC
     *        1: 器械
     *        2: 出口
     *        3: 入口
     */
    Message goTo(const int &ind, const int &type);

    /**
     * @brief 保存当前地图到文件中
     * @note 要求当前地图有效，即 is_valid 为 True\n
     *       TODO: save() 在程序退出时应当被 Controller 调用，在接收到终止信号时,
     *       程序 Controller 也应当调用 save()，这要求程序手动管理信号！！！
     */
    Message save() const;
    /**
     * @brief 检查这个坐标处是否有器械、NPC、出口
     * @param pos Position，这个位置的坐标
     * @return a char index of SPECIAL_CHARS value, -1 表示未碰撞 -2 表示空间狭小/墙壁
     */
    char detectCollision(const Position &pos) const;

    /**
     * @brief 获取字符的 index
     * @param ch 字符
     * @return 若在列表中返回列表索引，否则返回 -1
     */
    static int char2index(const char &ch);

private:

    // 新增：存储出口、入口、NPC位置（ID为索引）
    //haozhe Tang
    std::vector<Position> exits;    // 出口位置列表
    std::vector<Position> entries;  // 入口位置列表
    std::vector<Position> npcs;     // NPC位置列表
    std::vector<Position> instruments;  // 器械位置列表（按SPECIAL_CHARS索引）
    enum class LineType
    {
        WALL,        // 墙壁
        EMPTY_LINE,  // 空行
        OVER_SIZE,   // 尺寸超限
        INVAILD_LINE // 非法的行
    };

    /**
     * @brief 一个结构体，用于储存读取时是否已经读到了某些必要的行
     * @note 此处缩写了，因为与 LineType 前 3 个一一对应
     */
    bool is_empty;                   // 地图读取到目前位置是否为空
    bool modified;                   // 地图是否被修改过
    bool is_valid;                   // 该地图类是否有效
    std::string valid_msg;           // 关于地图是否有效的消息
    char map[MAX_HEIGHT][MAX_WIDTH]; // 地图数组
    int max_width;                   // 地图最大宽度
    int max_height;                  // 地图最大高度
    // 方向数组
    inline static int DIRECTIONS[4][2] = {
        {-1, 0},
        {0, 1},
        {1, 0},
        {0, -1}};
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
    Message loadMap(const std::string &filename);

    /**
     * @brief 设置 NPC 和出口等的 ID
     * @param rows 扫描的行数
     * @return 返回索引建立是否成功
     */
    bool indexInit(const int &rows);

    /**
     * @brief 对行的类型进行辨别
     * @return 返回一个 enum class LineType
     */
    LineType classifyLine(const std::string &line);

    /**
     * @brief 处理所有 NPC 和出入口，并进行封闭性和宽字符放置检查
     * @return bool 地图是否封闭, 宽字符放置是否合理
     */
    bool processMap();

    /**
     * @brief 在沿地图边界检查地图时，修改参数以进行转向
     */
    bool calcDir(int &x, int &y, const bool &reset = false);

    /**
     * @brief 检查宽字符
     */
    bool checkWideChar(const int &x, const int &y);

    /**
     * @brief 获取出口 ID
     */
    int getExitId(const Position &pos);

    /**
     * @brief 获取NPC ID
     */
    int getNPCId(const Position& pos);

    /**
     * @brief 储存行到地图中，并检查是否含有非法字符
     */
    static bool line_copy(char map_line[], const std::string &line);
};
