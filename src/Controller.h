/**
 * @file Controller.h
 */
#pragma once
#include <iostream>
#include <filesystem>
#include <memory>
#include "tools.h"
#include <set>
#include <ctime>
#include <vector>
#include"Store.h"

#include <cereal/archives/binary.hpp>
#include <cereal/types/string.hpp>
#include <cereal/types/memory.hpp>
#include <cereal/types/vector.hpp>

class View;
class Scene;
class Backpack;
class InputHandler;
class NPC;
class Protagonist;
class Map;
class Store;
class FinalExam;
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
        DEBUG, ///< 消息只应当在调试时被看到
        INFO,  ///< 程序正常运行时可以输出的消息
        WARN,  ///< 消息对应的事件发生时，程序能运行，但仍需引起注意
        ERR    ///< 严重的错误，该事件发生时程序会 Crash
    };

 
    /**
     * @brief 游戏状态
     * @details
     */

    // 禁用拷贝构造函数和赋值运算符
    Controller(const Controller &) = delete;
    Controller &operator=(const Controller &) = delete;

    /**
     * @brief 单例模式创建实例
     * @param level 日志等级，用于确定游戏界面应当输出的日志信息，默认为 INFO
     * @param log_dir 日志目录，默认为项目根目录下的 logs/
     * @param root_dir 根目录，默认为可执行文件所在路径
     */
    static std::shared_ptr<Controller> getInstance(const LogLevel &level=LogLevel::INFO, const std::filesystem::path &log_dir=std::filesystem::path(), const std::filesystem::path &root_dir=std::filesystem::path());

    /**
     * @brief 日志函数
     * @param level 日志等级
     * @param msg 日志消息
     */
    void log(const LogLevel &level, const std::string &msg);

    /**
     * @brief 游戏入口函数
     * @details 游戏入口函数，供 main() 调用\n
     *          游戏执行流程：
     *          1. init
     *          2. 登录/注册
     *          3. 死循环等待按键响应
     * @return 一个状态码，供游戏 exit 使用
     */
    int run();

    /**
     * @brief 获取 root_dir
     * @return std::filesystem::path
     */
    std::filesystem::path getRootDir() const;

    // Model 类的智能指针
    std::shared_ptr<Map>          map         = nullptr;
    std::shared_ptr<Protagonist>  protagonist = nullptr;
    std::shared_ptr<NPC>          npc         = nullptr;
    std::shared_ptr<View>         view        = nullptr;
    std::shared_ptr<InputHandler> input       = nullptr;
    std::shared_ptr<Backpack>     backpack    = nullptr;
    std::shared_ptr<Scene>        scene       = nullptr;
    std::shared_ptr<Store>        store       = nullptr;
    std::shared_ptr<FinalExam>    final_exam  = nullptr;
    template <class Archive>
    void serialize(Archive &archive)
    {
        archive(CEREAL_NVP(protagonist),
                CEREAL_NVP(npc)
                // CEREAL_NVP(backpack),
        );
    }

private:
    // 项目根目录
    std::filesystem::path root_dir;

    // 该文件永远需要被写入，且会写入所有消息，囊括了 info, warn and error
    inline static const std::string DEBUG_FILE = "Debug.log";
    // 只有设置为需要写入才写入
    inline static const std::string INFO_FILE = "Info.log";
    // 只有设置为需要写入才写入
    inline static const std::string WARN_FILE = "Warnings.log";
    // 永远写入
    inline static const std::string ERROR_FILE = "Error.log";
    // 日志目录
    std::filesystem::path log_dir;
    // 日志等级
    LogLevel level;

    // 构造函数
    Controller(const LogLevel &level, const std::filesystem::path &log_dir, const std::filesystem::path &root_dir);

    /**
     * @brief 初始化函数·
     * @details 初始化信息，包括：\n
     *          0. 调用 view 模块中的函数进行界面初始化
     *          1. 初始化必要的智能指针
     *          2. 初始化 角色 和 背包
     * @param username 用户名
     * @param pro 角色信息
     * @param backpack 背包信息
     * @note 使用 Protagonist 和 Backpack 的默认值，主函数需要提前创建 角色 和 背包 对象
     * @return 初始化结果信息
     */
    Message init();

    
    Message load(std::string username);

    /**
     * @brief 保存游戏
     * @details 保存游戏的逻辑，包括：\n
     *          1. 保存所有数据到文件中
     * @param pro 角色信息
     * @param backpack 背包信息
     */
    Message save();

    /**
     * TODO
     * @brief 登录
     * @details 实现登录逻辑
     */
    Message playerLogin(std::string &user_name);

    /**
     * @brief 获得用户操作事件
     * @param[out] event_type 事件类型
     */
    Message getEvent(EventType &event_type);

    /**TODO
     * @brief 与 NPC 碰撞的 handler
     * @param npc_id NPC 编号
     * @return Message
     */
    Message interactNPC();

    /**
     * @brief 与器械交互
     */

    /**
     * @brief 解决用户操作事件
     * @param[out] event_type 事件类型
     */
    Message handleEvent(EventType &event_type);
};
