/**
 * @file Controller.h
 */

#include <iostream>
#include <memory>
#include "tools.h"
#include "Map.h"
#include "Item.h"
#include "Protagonist.h"
#include "keylockerintrin.h"
#include "NPC.h"

/**
 * @brief MVC 模式中的 Controller
 * @details 程序的总控制器\n
 */
class Controller {
public:
    friend class View;

    /**
     * @brief 消息等级，DEBUG 最详细，ERROR 最简洁
     * @note 日志消息会按照等级被输出到日志文件中，同时控制台会打印设定等级
     * 的消息，详见 README 日志输出
     */
    enum class LogLevel {
        DEBUG,      ///< 消息只应当在调试时被看到
        INFO,       ///< 程序正常运行时可以输出的消息
        WARN,       ///< 消息对应的事件发生时，程序能运行，但仍需引起注意
        ERROR       ///< 严重的错误，该事件发生时程序会 Crash
    };

    /**
     * @brief 游戏状态
     * @details
     */

    // 禁用拷贝构造函数和赋值运算符
    Controller(const Controller&) = delete;
    Controller& operator=(const Controller&) = delete;

    /**
     * @brief 单例模式创建实例
     * @param level 日志等级，用于确定游戏界面应当输出的日志信息，默认为 INFO
     * @param log_dir 日志目录，默认为项目根目录下的 logs/
     * @param root_dir 根目录，默认为可执行文件所在路径
     */
    static std::shared_ptr<Controller> getInstance(const LogLevel& level=LogLevel::INFO, const std::string& log_dir="logs/", const std::string root_dir="") {
        static std::shared_ptr<Controller> instance(new Controller(level, log_dir, root_dir));
        return instance;
    }

    /**
     * @brief 日志函数
     * @param level 日志等级
     * @param msg 日志消息
     */
    void log(const LogLevel& level, const std::string& msg);

    /**
     * @brief 游戏入口函数
     * @details 游戏入口函数，供 main() 调用
     * @return 一个状态码，供游戏 exit 使用
     */
    int run();
private:
    // 项目根目录
    std::string root_dir;

    // 该文件永远需要被写入，且会写入所有消息，囊括了 info, warn and error
    inline static const std::string DEBUG_FILE = "Debug.log";
    // 只有设置为需要写入才写入
    inline static const std::string INFO_FILE = "Info.log";
    // 只有设置为需要写入才写入
    inline static const std::string WARN_FILE = "Warnings.log";
    // 永远写入
    inline static const std::string ERROR_FILE = "Error.log";
    // 日志目录
    std::string log_dir;
    // 日志等级
    LogLevel level;

    // Model 类的智能指针
    std::shared_ptr<Map> map;
    // TODO 补充自己的智能指针
    // 要求： 若该类唯一，则可使用智能指针管理

    // 构造函数
    Controller(const LogLevel& level, const std::string& log_dir, const std::string root_dir);

    /**
     * @brief 初始化函数
     * @details 这个函数应当：\n
     *          0. 调用 view 模块中的函数进行界面初始化
     *          1. 初始化必要的智能指针
     * @param create 是否注册账号
     * @note 初始化应该有默认参数，用于为新的玩家创建存档
     * @return 一条消息
     */
    Message init(const bool& create);

    /**
     * @brief 登录
     * @details 实现登录逻辑
     */
    
    /**
     * @brief 注册
     * @note 要求注册完立即调用保存游戏
     */

    /**
     * @brief 保存游戏
     * @details 保存地图更改，序列化所有 Model
     */

    /**
     * @brief 获取按键响应，并设置当前游戏状态
     */
};
