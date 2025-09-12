/**
 * @brief 主角的数据模型实现（Protagonist类）
 * @author Haozhe Tang
 * @date 2025-08-31
 */

#include <iostream>
#include "Protagonist.h"
#include "json.hpp"
#include <fstream>
#include <stdexcept>

Protagonist::Protagonist(const std::string &protagonistId, const std::string &name)
    : m_protagonistId(protagonistId), m_name(name)
{
    if (!isValidName(name))
    {
        throw std::invalid_argument("姓名包含非法字符");
    }
    if (jsonReader().status == 0)
        ; ///< 读取成功
    else
    {
        std::cout << jsonReader().msg << std::endl;
        // 读取失败，使用默认值
    }
}

/// 使用json文件初始化角色属性
using json = nlohmann::json;

Message Protagonist::jsonReader()
{
    try
    {
        /// 打开并读取文件
        std::ifstream JS("./.config/student.json");
        if (!JS.is_open())
        {
            Message msg("打开JSON文件失败", -1);
            return msg;
        }
        json j;
        JS >> j;
        JS.close();
        // 检查JSON是否包含主角所需字段

        if (!j.contains("protagonist") || !j["protagonist"].is_object())
        {
            Message msg("JSON格式错误：缺少protagonist对象", -1);
            return msg;
        }

        auto &protagData = j["protagonist"];

        // 验证必要字段（ID和姓名在构造后不可修改，这里仅做校验）
        if (!protagData.contains("protaogonistId") || !protagData["protaogonistId"].is_string())
        {
            Message msg("JSON格式错误：缺少有效的protaogonistId", -1);
            return msg;
        }
        if (!protagData.contains("name") || !protagData["name"].is_string())
        {
            Message msg("JSON格式错误：缺少有效的name", -1);
            return msg;
        }

        // 校验ID一致性（如果需要严格匹配）
        std::string jsonId = protagData["protaogonistId"];
        if (jsonId != m_protagonistId)
        {
            Message msg("JSON数据与当前主角ID不匹配", -1);
            return msg;
        }

        // 读取并更新可修改的属性
        auto updateFromJson = [&](const std::string &key, auto &member)
        {
            if (protagData.contains(key) && protagData[key].is_number())
            {
                member = protagData[key].get<int>();
            }
        };

        // 更新基础属性
        updateFromJson("intelSci", m_intelSci);
        updateFromJson("intelArts", m_intelArts);
        updateFromJson("strength", m_strength);
        updateFromJson("money", m_money);
        updateFromJson("health", m_health);

        // 校验所有属性合法性
        if (!isValidAttr(BasicValue::ProtagonistAttr::INTEL_SCI, m_intelSci) ||
            !isValidAttr(BasicValue::ProtagonistAttr::INTEL_ARTS, m_intelArts) ||
            !isValidAttr(BasicValue::ProtagonistAttr::STRENGTH, m_strength) ||
            !isValidAttr(BasicValue::ProtagonistAttr::MONEY, m_money) ||
            !isValidAttr(BasicValue::ProtagonistAttr::HEALTH, m_health))
        {
            Message msg("JSON数据包含非法的属性值", -1);
            return msg;
        }

        // 操作成功
        Message msg("JSON读取成功", 0);
        return msg;
    }
    catch (const std::exception &e)
    {
        Message msg(e.what(), -1);
        return msg;
    }
    catch (...)
    {
        Message msg("JSON读取时发生未知错误", -1);
        return msg;
    }
}

Message Protagonist::testPrint() const
{
    std::cout << "主角ID: " << m_protagonistId << std::endl;
    std::cout << "姓名: " << m_name << std::endl;
    std::cout << "智力-理: " << m_intelSci << std::endl;
    std::cout << "智力-文: " << m_intelArts << std::endl;
    std::cout << "体力: " << m_strength << std::endl;
    std::cout << "金钱: " << m_money << std::endl;
    std::cout << "健康: " << m_health << std::endl;
    Message msg("打印成功", 0);
    return msg;
}
// 获取主角唯一ID
const std::string &Protagonist::getProtagonistId() 
{
    return m_protagonistId;
}

// 获取姓名
const std::string &Protagonist::getName() 
{
    return m_name;
}

// 获取所有属性
std::unordered_map<BasicValue::ProtagonistAttr, float> Protagonist::getBaseAttrs() const
{
    return {
        {BasicValue::ProtagonistAttr::INTEL_SCI, m_intelSci},
        {BasicValue::ProtagonistAttr::INTEL_ARTS, m_intelArts},
        {BasicValue::ProtagonistAttr::STRENGTH, m_strength},
        {BasicValue::ProtagonistAttr::MONEY, m_money},
        {BasicValue::ProtagonistAttr::HEALTH, m_health},
    };
}

// 以后是隐藏值
std::unordered_map<BasicValue::ProtagonistAttr, float> Protagonist::getHiddenAttrs() const
{
    return {
        {BasicValue::ProtagonistAttr::INTELSCI_BOOST, intelSci_boost},
        {BasicValue::ProtagonistAttr::INTELARTS_BOOST, intelArts_boost},
        {BasicValue::ProtagonistAttr::INTELSCI_BOOST_RATE, intelSci_boost_rate},
        {BasicValue::ProtagonistAttr::INTELARTS_BOOST_RATE, intelArts_boost_rate},
        {BasicValue::ProtagonistAttr::LEARNING_TIME_REDUCTION_RATE, learning_time_reduction_rate},
        {BasicValue::ProtagonistAttr::LEARNING_HEALTH_PRESERVATION_RATE, learning_health_preservation_rate}};
}

// 获取当前健康状态
BasicValue::HealthState Protagonist::getHealthState(std::string &outStateDesc) const
{
    if (isInjured)
    {
        outStateDesc = "受伤";
        return BasicValue::HealthState::INJURED;
    }
    else if (m_health >= 70)
    {
        outStateDesc = "健康";
        return BasicValue::HealthState::HEALTHY;
    }
    else if (m_health >= 30)
    {
        outStateDesc = "生病";
        return BasicValue::HealthState::SICK;
    }
    else
    {
        outStateDesc = "死亡";
        return BasicValue::HealthState::DEAD;
    }
}

// 获取当前场景
BasicValue::Scene Protagonist::getCurrentScene() const
{
    return current_scene;
}

// 获取当前位置
Position Protagonist::getPosition() const
{
    return pos;
}

// 通用属性修改接口
Message Protagonist::updateAttr(BasicValue::ProtagonistAttr attr, int val, bool isAdd)
{
    // 辅助函数：处理int类型属性
    auto processIntAttr = [&](int &member)
    {
        std::unique_ptr<int> newVal = std::make_unique<int>(isAdd ? member + val : val);
        if (!isValidAttr(attr, *newVal))
        {
            return Message("属性值越界", 1);
        }
        member = *newVal;
        return Message("属性更新成功", 0);
    };

    // 辅助函数：处理float类型属性
    auto processFloatAttr = [&](float &member)
    {
        std::unique_ptr<float> newVal = std::make_unique<float>(isAdd ? member + static_cast<float>(val) : static_cast<float>(val));
        if (!isValidAttr(attr, static_cast<int>(*newVal)))
        { // 适配原验证逻辑
            return Message("属性值越界", 1);
        }
        member = *newVal;
        return Message("属性更新成功", 0);
    };

    switch (attr)
    {
    case BasicValue::ProtagonistAttr::INTEL_SCI:
        return processIntAttr(m_intelSci);
    case BasicValue::ProtagonistAttr::INTEL_ARTS:
        return processIntAttr(m_intelArts);
    case BasicValue::ProtagonistAttr::STRENGTH:
        return processIntAttr(m_strength);
    case BasicValue::ProtagonistAttr::MONEY:
        return processIntAttr(m_money);
    case BasicValue::ProtagonistAttr::HEALTH:
        return processIntAttr(m_health);
    case BasicValue::ProtagonistAttr::INTELSCI_BOOST:
        return processFloatAttr(intelSci_boost);
    case BasicValue::ProtagonistAttr::INTELARTS_BOOST:
        return processFloatAttr(intelArts_boost);
    case BasicValue::ProtagonistAttr::INTELSCI_BOOST_RATE:
        return processFloatAttr(intelSci_boost_rate);
    case BasicValue::ProtagonistAttr::INTELARTS_BOOST_RATE:
        return processFloatAttr(intelArts_boost_rate);
    case BasicValue::ProtagonistAttr::LEARNING_TIME_REDUCTION_RATE:
        return processFloatAttr(learning_time_reduction_rate);
    case BasicValue::ProtagonistAttr::LEARNING_HEALTH_PRESERVATION_RATE:
        return processFloatAttr(learning_health_preservation_rate);
    case BasicValue::ProtagonistAttr::IS_INJURED:
        isInjured = (val != 0);
        return Message("受伤状态更新成功", 0);
    default:
        return Message("属性名错误", -1);
    }
}

// 设置主角的当前位置
Message Protagonist::setPosition(const Position &position)
{
    pos = position;
    return Message("位置设置成功", 0);
}

// 设置主角的当前场景
Message Protagonist::setCurrentScene(BasicValue::Scene scene)
{
    current_scene = scene;
    return Message("场景设置成功", 0);
}



bool Protagonist::isValidAttr(BasicValue::ProtagonistAttr attr, int val) const
{
    switch (attr)
    {
    case BasicValue::ProtagonistAttr::INTEL_SCI:
    case BasicValue::ProtagonistAttr::INTEL_ARTS:
    case BasicValue::ProtagonistAttr::STRENGTH:
        return val >= 0 && val <= 100; // 假设智力和体力范围为0~100
    case BasicValue::ProtagonistAttr::MONEY:
        return val >= 0; // 金钱不能为负
    case BasicValue::ProtagonistAttr::HEALTH:
        return val >= 0 && val <= 100; // 健康范围0~100
    case BasicValue::ProtagonistAttr::IS_INJURED:
        return val == 0 || val == 1; // 受伤状态只能是0或1
    case BasicValue::ProtagonistAttr::INTELSCI_BOOST:
        return val >= 0; // 智力科学提升只能为正
    case BasicValue::ProtagonistAttr::INTELARTS_BOOST:
        return val >= 0; // 智力艺术提升只能为正
    case BasicValue::ProtagonistAttr::INTELSCI_BOOST_RATE:
        return val >= 1; // 智力科学提升比例只能大于等于1
    case BasicValue::ProtagonistAttr::INTELARTS_BOOST_RATE:
        return val >= 1; // 智力艺术提升比例只能大于等于1
    case BasicValue::ProtagonistAttr::LEARNING_TIME_REDUCTION_RATE:
        return val >= 0 && val <= 1; // 时间消耗减少比率只能在0~1之间
    case BasicValue::ProtagonistAttr::LEARNING_HEALTH_PRESERVATION_RATE:
        return val >= 0 && val <= 1; // 健康损失保护比率只能在0~1之间
    default:
        return false; // 非法属性
    }
}

BasicValue::HealthState Protagonist::syncHealthState() const
{
    if (m_health >= 70)
    {
        return BasicValue::HealthState::HEALTHY;
    }
    else if (m_health >= 30)
    {
        return BasicValue::HealthState::SICK;
    }
    else
    {
        return BasicValue::HealthState::DEAD;
    }
}
// 校验姓名是否合法
bool Protagonist::isValidName(const std::string &name)
{
    // 定义不允许的特殊字符集合
    const std::string invalidChars = ";:\"'\\/<>*?|";
    // 检查是否包含任何非法字符
    return name.find_first_of(invalidChars) == std::string::npos;
};
// 设置主角的姓名，仅供初始化
Message Protagonist::setName(std::string &name)
{
    if (!isValidName(name))
    {
        return Message("姓名包含非法字符", -1);
    }
    m_name = name;
    return Message("姓名修改成功", 0);
}

// 获取当前游戏内时间
int Protagonist::getGameTime() const
{
    return game_time;
}

// 增加游戏内时间,仅用于初始化
Message Protagonist::setGameTime(int time)
{
    if (time < 0)
    {
        return Message("游戏内时间不能为负", -1);
    }
    game_time = time;
    return Message("游戏内时间设置成功", 0);
}

// 增加游戏内时间
Message Protagonist::addGameTime(int time)
{
    if (time <= 0)
    {
        return Message("增加的时间必须为正数", -1);
    }
    game_time += time;
    return Message("游戏内时间增加成功", 0);
}