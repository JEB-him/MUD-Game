/**
 * @file tools.h
 * @details 这是一个工具文件，用于存放常用的工具类
 * @author Jie Jiang
 */
 
#pragma once

#pragma once

#include <string>
#include <cstdint>
#include <cereal/archives/binary.hpp>
#include <cereal/cereal.hpp>
#include <regex>

// 定义  Rgb 颜色类
class  Rgb {
public:
    std::uint8_t r;  // 红色分量
    std::uint8_t g;  // 绿色分量
    std::uint8_t b;  // 蓝色分量

     Rgb(std::uint8_t r=0, std::uint8_t g=0, std::uint8_t b=0);
};

/**
 * @brief 一个通用的坐标类，建议都使用该类来储存坐标
 */
class Position 
{
public:
    // `<` 表示该注释作用的代码在注释的前面而不是后面
    /* 上面这种只有2个`/`,或者现在使用的只有1个`*`的注释不会被 Doxygen 识别到 */
    int x=0;      ///< 横坐标, 该成员 public
    int y=0;      ///< 纵坐标，该成员 public

    /**
     * @brief 使用一对 int 值来构造一个坐标类
     * @param x 横坐标，默认为 0
     * @param y 纵坐标，默认为 0
     */
    Position(const int& x, const int& y);
    Position()=default;
    template <class Archive>
    void serialize(Archive &arch)
    {
        arch(CEREAL_NVP(x),
        CEREAL_NVP(y));
    };
};

/**
 * @brief 一个通用的消息类，用于在调用中返回消息,
 * @details 建议使用该类来返回调用是否成功
 * @note 只有初始化的时候能够对 Message 赋值，以后就不允许更改了\n
 *       **remove the const specifier**
 */
class Message {
public:
    std::string msg;    ///< 消息体
    int status;         ///< 状态码

    /**
     * @brief 初始化一条消息
     * @details 将状态码和消息内容封装成一条 Message
     * @param msg 消息内容，缺省为空
     * @param status 消息状态码 required.\n
     *        -1 异常退出\n
     *         0 正常退出\n
     *        >0 正常退出，但有部分警告或不恰当参数，不影响程序正常运行
     */
    Message(const std::string& msg="", const int& status=0);
private:
    // private 块中的注释也不会被识别到，无论使用什么形式的注释
};

/**
 * @brief 用于储存特殊符号的类
 * @details 对于一个特殊符号，我们储存一下信息：\n
 *          1. 符号本身：\U 的形式，必须有八位，不足八位高位补0\n
 *          2. 符号宽度：有些字符相当于一个中文汉字，有些只有一个英文字母宽\n
 * @note 请严格按照 README 中的对照表顺序在 Map 中添加特殊符号
 */
class SpecialChar {
public:
    const std::string special_char;    ///< 因为这些符号一般都占 3 字节，所以需要使用string
    const int width;                   ///< 符号宽度, 这个宽度和 strlen 并不一致，请注意！！！
    const std::string simple_color;    ///< ANSI 普通颜色
    const  Rgb  Rgb_color;               ///< ANSI  Rgb 颜色
    const bool need_empty;             ///< 是否需有手动添加空格
    /**
     * @brief 构造函数，有特殊意义的参数见下
     * @param simple_color 默认为 "white"
     * @param  Rgb_color 当 simple_color 为空时启用
     */
    SpecialChar(
        const std::string& special_char,
        const int& width,
        const bool& need_empty=true,
        const std::string& simple_color="white",
        const  Rgb&  Rgb_color= Rgb(0, 0, 0)
    );

    /**
     * @brief 默认构造函数
     * @note width 置为 -1 以保证该实例无效
     */
    SpecialChar();
};

/**
 * @brief 检查用户名是否合法
 * @details 只允许字母、数字、下划线和中文字符
 * @param username 待检查的用户名
 * @return 合法返回 true，否则返回 false
 */
bool isValidUsername(const std::string& username);