/**
 * @brief 主角的数据模型实现（Protagonist类）
 * @author Haozhe Tang
 * @date 2024-08-31
 */

#include <iostream>
#include "Protagonist.h"
#include "json.hpp"
#include <fstream>
#include <stdexcept>
#include "tools.cpp"



 Protagonist::Protagonist(const std::string& protagonistId, const std::string& name)
     : m_protagonistId(protagonistId), m_name(name){
        if(jsonReader().status == 0); ///< 读取成功
        else{
            std::cout <<jsonReader().msg<<std::endl;
            // 读取失败，使用默认值
            m_intelSci = 0;
            m_intelArts = 0;
            m_strength = 0;
            m_money = 0;
            m_health = 100;
        }
    }





///使用json文件初始化角色属性
using json = nlohmann::json;

Message Protagonist::jsonReader(){
    try {
        ///打开并读取文件
        std::ifstream JS(".\\..\\tests\\unit\\Protagonist_Test.json");
        if(!JS.is_open()){
            Message msg("打开JSON文件失败", -1);
            return msg;
        }
        json j;
        JS >> j;
        JS.close();
        //检查JSON是否包含主角所需字段
  
      if (!j.contains("protagonist") || !j["protagonist"].is_object()) {
           Message msg("JSON格式错误：缺少protagonist对象", -1);
            return msg;
        }

        auto& protagData = j["protagonist"];

        // 验证必要字段（ID和姓名在构造后不可修改，这里仅做校验）
        if (!protagData.contains("protaogonistId") || !protagData["protaogonistId"].is_string()) {
           Message msg ("JSON格式错误：缺少有效的protaogonistId",-1);
            return msg;
        }
        if (!protagData.contains("name") || !protagData["name"].is_string()) {
            Message msg("JSON格式错误：缺少有效的name", -1);
            return msg;
        }

        // 校验ID一致性（如果需要严格匹配）
        std::string jsonId = protagData["protaogonistId"];
        if (jsonId != m_protagonistId) {
            Message msg("JSON数据与当前主角ID不匹配", -1);
            return msg;
        }

        // 读取并更新可修改的属性
        auto updateFromJson = [&](const std::string& key, auto& member) {
            if (protagData.contains(key) && protagData[key].is_number()) {
                member = protagData[key].get<int>();
            }
        };

        // 更新基础属性
        updateFromJson("intelSci", m_intelSci);
        updateFromJson("intelArts", m_intelArts);
        updateFromJson("strength", m_strength);
        updateFromJson("money", m_money);
        updateFromJson("health", m_health);

        //校验所有属性合法性
        if (!isValidAttr(BasicValue::ProtagonistAttr::INTEL_SCI, m_intelSci) ||
            !isValidAttr(BasicValue::ProtagonistAttr::INTEL_ARTS, m_intelArts) ||
            !isValidAttr(BasicValue::ProtagonistAttr::STRENGTH, m_strength)||
            !isValidAttr(BasicValue::ProtagonistAttr::MONEY, m_money) ||
            !isValidAttr(BasicValue::ProtagonistAttr::HEALTH, m_health)) {
            Message msg("JSON数据包含非法的属性值", -1);
            return msg;
        }

        // 操作成功
        Message msg("JSON读取成功", 0);
        return msg;
    } catch (const std::exception& e) {
        Message msg(e.what(),-1);
        return msg;
    } catch (...) {
        Message msg("JSON读取时发生未知错误", -1);
        return msg;
    }

}




Message Protagonist::testPrint () const {
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
//获取主角唯一ID
const std::string& Protagonist::getProtagonistId() const {
    return m_protagonistId;
}


//获取姓名
const std::string& Protagonist::getName() const {
    return m_name;
}

//获取所有属性
std::unordered_map<BasicValue::ProtagonistAttr, float> Protagonist::getBaseAttrs() const {
    return {
        { BasicValue::ProtagonistAttr::INTEL_SCI, m_intelSci },
        { BasicValue::ProtagonistAttr::INTEL_ARTS, m_intelArts },
        { BasicValue::ProtagonistAttr::STRENGTH, m_strength },
        { BasicValue::ProtagonistAttr::MONEY, m_money },
        { BasicValue::ProtagonistAttr::HEALTH, m_health },
    };
}

//以后是隐藏值
std::unordered_map<BasicValue::ProtagonistAttr, float> Protagonist::getHiddenAttrs() const {
    return {
        { BasicValue::ProtagonistAttr::INTELSCI_BOOST, intelSci_boost },
        { BasicValue::ProtagonistAttr::INTELARTS_BOOST, intelArts_boost },
        { BasicValue::ProtagonistAttr::INTELSCI_BOOST_RATE, intelSci_boost_rate },
        { BasicValue::ProtagonistAttr::INTELARTS_BOOST_RATE, intelArts_boost_rate },
        { BasicValue::ProtagonistAttr::LEARNING_TIME_REDUCTION_RATE, learning_time_reduction_rate },
        { BasicValue::ProtagonistAttr::LEARNING_HEALTH_PRESERVATION_RATE, learning_health_preservation_rate }
    };
}

//获取当前健康状态
BasicValue::HealthState Protagonist::getHealthState(std::string& outStateDesc) const {
    if (isInjured) {
        outStateDesc = "受伤";
        return BasicValue::HealthState::INJURED;
    } else if (m_health >= 70) {
        outStateDesc = "健康";
        return BasicValue::HealthState::HEALTHY;
    } else if (m_health >= 30) {
        outStateDesc = "生病";
        return BasicValue::HealthState::SICK;
    } else {
        outStateDesc = "死亡";
        return BasicValue::HealthState::DEAD;
    }
}


//通用属性修改接口
Message Protagonist::updateAttr(BasicValue::ProtagonistAttr attr, int val, bool isAdd) {
    int* targetAttr = nullptr;
    float* targetFloatAttr = nullptr; // 用于浮点数属性

    switch (attr) {
        case BasicValue::ProtagonistAttr::INTEL_SCI:
            targetAttr = &m_intelSci;
            break;
        case BasicValue::ProtagonistAttr::INTEL_ARTS:
            targetAttr = &m_intelArts;
            break;
        case BasicValue::ProtagonistAttr::STRENGTH:
            targetAttr = &m_strength;
            break;
        case BasicValue::ProtagonistAttr::MONEY:
            targetAttr = &m_money;
            break;
        case BasicValue::ProtagonistAttr::HEALTH:
            targetAttr = &m_health;
            break;
        case BasicValue::ProtagonistAttr::INTELSCI_BOOST:
            targetFloatAttr = &intelSci_boost;
            break;
        case BasicValue::ProtagonistAttr::INTELARTS_BOOST:
            targetFloatAttr = &intelArts_boost;
            break;
        case BasicValue::ProtagonistAttr::INTELSCI_BOOST_RATE:
            targetFloatAttr = &intelSci_boost_rate;
            break;
        case BasicValue::ProtagonistAttr::INTELARTS_BOOST_RATE:
            targetFloatAttr = &intelArts_boost_rate;
            break;
        case BasicValue::ProtagonistAttr::LEARNING_TIME_REDUCTION_RATE:
            targetFloatAttr = &learning_time_reduction_rate;
            break;
        case BasicValue::ProtagonistAttr::LEARNING_HEALTH_PRESERVATION_RATE:
            targetFloatAttr = &learning_health_preservation_rate;
            break;
        case BasicValue::ProtagonistAttr::IS_INJURED:
            if (!isAdd) {
                isInjured = (val != 0);
                return Message("受伤状态更新成功", 0);
            } else {
                isInjured = (val != 0);
                return Message("受伤状态设置成功", 0);
            }
        default:
            return Message("属性名错误", -1);
    }

    if (targetAttr) {
        int newValue = isAdd ? (*targetAttr + val) : val;

        // 属性合法性校验
        if (!isValidAttr(attr, newValue)) {
            return Message("属性值越界", 1);
        }

        *targetAttr = newValue;
        return Message("属性更新成功", 0);
    }

    if (targetFloatAttr) {
        float newValue = isAdd ? (*targetFloatAttr + val) : val;

        // 属性合法性校验
        if (!isValidAttr(attr, newValue)) {
            return Message("属性值越界", 1);
        }

        *targetFloatAttr = newValue;
        return Message("属性更新成功", 0);
    }

    return Message("未知错误", -1);
}

// 序列化主角数据
//尚未完成
std::string Protagonist::serialize() const {
    std::string data;
    data += "ID:" + m_protagonistId + ";";
    data += "NAME:" + m_name + ";";
    data += "INTEL_SCI:" + std::to_string(m_intelSci) + ";";
    data += "INTEL_ARTS:" + std::to_string(m_intelArts) + ";";
    data += "STRENGTH:" + std::to_string(m_strength) + ";";
    data += "MONEY:" + std::to_string(m_money) + ";";
    data += "HEALTH:" + std::to_string(m_health) + ";";
    data += "INTELSCI_BOOST:" + std::to_string(intelSci_boost) + ";";
    data += "INTELARTS_BOOST:" + std::to_string(intelArts_boost) + ";";
    data += "INTELSCI_BOOST_RATE:" + std::to_string(intelSci_boost_rate) + ";";
    data += "INTELARTS_BOOST_RATE:" + std::to_string(intelArts_boost_rate) + ";";
    data += "LEARNING_TIME_REDUCTION_RATE:" + std::to_string(learning_time_reduction_rate) + ";";
    data += "LEARNING_HEALTH_PRESERVATION_RATE:" + std::to_string(learning_health_preservation_rate) + ";";
    data += "IS_INJURED:" + std::to_string(isInjured ? 1 : 0) + ";";
    return data;
}


//反序列化主角数据 
//尚未完成
Message Protagonist::deserialize(const std::string& data) {
    std::istringstream ss(data);
    std::string token;

    while (std::getline(ss, token, ';')) {
        auto delimiterPos = token.find(':');
        if (delimiterPos == std::string::npos) continue;

        std::string key = token.substr(0, delimiterPos);
        std::string valueStr = token.substr(delimiterPos + 1);
        int value = std::stoi(valueStr);

        if (key == "INTEL_SCI") {
            if (!isValidAttr(BasicValue::ProtagonistAttr::INTEL_SCI, value)) {
                return Message("属性值越界", 1);
            }
            m_intelSci = value;
        } else if (key == "INTEL_ARTS") {
            if (!isValidAttr(BasicValue::ProtagonistAttr::INTEL_ARTS, value)) {
                return Message("属性值越界", 1);
            }
            m_intelArts = value;
        } else if (key == "STRENGTH") {
            if (!isValidAttr(BasicValue::ProtagonistAttr::STRENGTH, value)) {
                return Message("属性值越界", 1);
            }
            m_strength = value;
        } else if (key == "MONEY") {
            if (!isValidAttr(BasicValue::ProtagonistAttr::MONEY, value)) {
                return Message("属性值越界", 1);
            }
            m_money = value;
        } else if (key == "HEALTH") {
            if (!isValidAttr(BasicValue::ProtagonistAttr::HEALTH, value)) {
                return Message("属性值越界", 1);
            }
            m_health = value;
        } else if (key == "INTELSCI_BOOST") {
            intelSci_boost = static_cast<float>(value);
        } else if (key == "INTELARTS_BOOST") {
            intelArts_boost = static_cast<float>(value);
        } else if (key == "INTELSCI_BOOST_RATE") {
            intelSci_boost_rate = static_cast<float>(value);
        } else if (key == "INTELARTS_BOOST_RATE") {
            intelArts_boost_rate = static_cast<float>(value);
        } else if (key == "LEARNING_TIME_REDUCTION_RATE") {
            learning_time_reduction_rate = static_cast<float>(value);
        } else if (key == "LEARNING_HEALTH_PRESERVATION_RATE") {
            learning_health_preservation_rate = static_cast<float>(value);
        } else if (key == "IS_INJURED") {
            isInjured = (value != 0);
        }
    }

    return Message("反序列化成功", 0);
}


bool Protagonist::isValidAttr(BasicValue::ProtagonistAttr attr, int val) const {
    switch (attr) {
        case BasicValue::ProtagonistAttr::INTEL_SCI:
        case BasicValue::ProtagonistAttr::INTEL_ARTS:
        case BasicValue::ProtagonistAttr::STRENGTH:
            return val >= 0 && val <= 100; // 假设智力和体力范围为0~100
        case BasicValue::ProtagonistAttr::MONEY:
            return val >= 0; // 金钱不能为负
        case BasicValue::ProtagonistAttr::HEALTH:
            return val >= 0 && val <= 100; // 健康范围0~100
        default:
            return false; // 非法属性
    }
}

BasicValue::HealthState Protagonist::syncHealthState() const {
    if (m_health >= 70) {
        return BasicValue::HealthState::HEALTHY;
    } else if (m_health >= 30) {
        return BasicValue::HealthState::SICK;
    } else {
        return BasicValue::HealthState::DEAD;
    }
}
