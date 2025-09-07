/**
 * @file Protagonist.h
 * @author Haozhe Tang
 * @brief MVC架构的主角Model类，仅管理核心数据与属性修改，不承担Controller/View职责
 * @details 职责边界：1. 存储主角属性、状态；2. 提供属性修改/查询接口；3. 支持序列化存档；
 */
#pragma once
#include "tools.h"
#include <string>
#include <unordered_map>


// 前置声明：仅用于交互接口，避免依赖具体实现（Controller负责中转）
class Item;

/**
 * @brief 主角属性枚举及健康状态枚举
 * @details ProtagonistAttr枚举定义了所有可修改的属性，HealthState枚举定义了健康状态
 * 
 */
namespace BasicValue {


    /**
     * @brief 主角属性枚举
     * @details ProtagonistAttr枚举定义了所有可修改的属性
     */
    enum class ProtagonistAttr {
        INTEL_SCI,          ///< 智力-理
        INTEL_ARTS,         ///< 智力-文
        STRENGTH,          ///< 力量
        MONEY,             ///< 金钱
        HEALTH,             ///< 健康
        INTELSCI_BOOST,		///< 理科智力基础增量
        INTELARTS_BOOST,	///< 文科智力基础增量
        INTELSCI_BOOST_RATE,///< 理科智力基础增量百分比
        INTELARTS_BOOST_RATE,///< 文科智力基础增量百分比
        ID,                 ///< 主角唯一ID
        NAME,                ///< 主角姓名
        LEARNING_TIME_REDUCTION_RATE,///< 学习时间消耗减少比率
        LEARNING_HEALTH_PRESERVATION_RATE,///< 学习健康保留率
        IS_INJURED           ///< 受伤状态标志

    };

     /**
     * @brief 主角健康状态枚举（与m_health属性关联）
     * @details 状态触发规则：HEALTHY(health≥70)、SICK(30≤health<70)、DEAD(health≤0)\n
     */
    enum class HealthState {
        HEALTHY,   ///< 健康状态
        SICK,      ///< 生病状态
        INJURED,   ///< 受伤状态
        DEAD       ///< 死亡状态（游戏结束）
    };
}

class Protagonist {
public:

    /**
     * @brief 构造函数：初始化主角基础数据（属性、状态）
     * @param protagonistId 主角唯一ID（如“OUC_2024_001”，供Controller区分角色）
     * @param name 主角姓名
     */
    Protagonist(const std::string& protagonistId, const std::string& name);

    ~Protagonist() = default; // 析构函数：无动态内存，默认即可

    /**
     * @brief 获取主角唯一ID
     * @return const std::string& 不可修改的ID
     */
    const std::string& getProtagonistId() const;

    /**
     * @brief 获取主角姓名
     * @return const std::string& 不可修改的姓名
     */
    const std::string& getName() const;

    /**
     * @brief 获取所有基础属性（供Controller同步UI/存档）
     * @return std::unordered_map 键值对：key=BasicValue::ProtagonistAttr，value=当前值
     */
    std::unordered_map<BasicValue::ProtagonistAttr, float> getBaseAttrs() const;




    /**
     * @brief 获取所有隐藏属性（供Controller同步UI/存档）
     * @return std::unordered_map 键值对：key=BasicValue::ProtagonistAttr，value=当前值
     */
    std::unordered_map<BasicValue::ProtagonistAttr, float> getHiddenAttrs() const ;


        
    /**
     * @brief 获取当前健康状态（含文本描述，供Controller传递给View）
     * @param outStateDesc 输出参数：状态文本描述（如“健康”“生病”）
     * @return HealthState 当前状态枚举（供Controller判断业务逻辑）
     */
    BasicValue::HealthState getHealthState(std::string& outStateDesc) const;


    // -------------------------- 2. 属性修改接口（供Controller/Item类交互） --------------------------
    /**
     * @brief 通用属性修改（供Controller和Item类调用，如学习/打工导致的属性变化）
     * @param attr 属性名（支持：BasicValue::ProtagonistAttr）
     * @param val 变化量（isAdd=true）或目标值（isAdd=false）
     * @param isAdd 是否为增减（true=val为变化量，false=val为目标值）
     * @return Message 操作结果：status=0（成功）/-1（属性名错误）/1（值越界）；msg=结果描述

     * @note 对于受伤状态（IS_INJURED），val=1表示受伤，val=0表示恢复 isAdd参数为false

     */
    Message updateAttr(BasicValue::ProtagonistAttr attr, int val, bool isAdd = true);



    // -------------------------- 3. 序列化/反序列化接口（供存档系统） --------------------------
    /**
     * @brief 序列化主角数据（供Controller写入存档文件）
     * @return std::string 序列化字符串，包含所有基础属性（id/name）和健康状态
     */
    std::string serialize() const;

    /**
     * @brief 反序列化主角数据（供Controller从存档文件恢复）
     * @param data 序列化字符串（需与serialize()输出格式一致）
     * @return Message 操作结果：status=0（成功）/-1（数据无效/格式错误）；msg=结果描述
     */
    Message deserialize(const std::string& data);

    /**
     * @brief 测试打印当前主角数据（供调试）
     */
    Message testPrint() const;

    /// @brief 读取JSON格式的主角数据
    /// @return 操作结果：status=0（成功）/-1（数据无效/格式错误）；msg=结果描述
    Message jsonReader();

    
private:
    // -------------------------- 核心成员变量 --------------------------
    const std::string m_protagonistId;  ///< 主角唯一ID（不可修改）
    const std::string m_name;           ///< 主角姓名（不可修改）
    // 基础属性（需求定义，初始值按需求设定）
    int m_intelSci ;                ///< 智力-理（影响工作效率）
    int m_intelArts ;               ///< 智力-文（影响NPC交流）
    int m_strength ;                ///< 体力（0~100）
    int m_money ;                  ///< 金币（≥0）
    int m_health ;                 ///< 健康状态（0~100，关联HealthState）
    //隐藏属性

    float intelSci_boost = 0.0;		            	///< 理科智力基础增确保大于等于0
    float intelSci_boost_rate = 1.0;		        ///< 理科智力比例增量确保大于等于1
    float intelArts_boost = 0.0;			        ///< 文科智力基础增量确保大于等于0
    float intelArts_boost_rate = 1.0;	             ///< 文科智力比例增量确保大于等于1
    float learning_time_reduction_rate = 1.0;	    ///< 时间消耗减少比率(学)确保大于等于0，小于等于1
    float learning_health_preservation_rate = 1.0;	///< 健康损失保护比率(学)确保大于等于0，小于等于1

    bool isInjured = 0;                             ///< 受伤状态标志
  
  
  
 
    /**
     * @brief 校验属性值是否合法（如health 0~100，money≥0）
     * @param attrKey 属性名
     * @param val 待校验的值
     * @return bool true=合法，false=不合法
     */
    bool isValidAttr(BasicValue::ProtagonistAttr attr, int val) const;

    /**
     * @brief 同步健康状态（m_health变化后自动更新状态，无需外部调用）
     * @return HealthState 同步后的状态
     */
    BasicValue::HealthState syncHealthState() const;


};

