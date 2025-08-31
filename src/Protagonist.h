
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

class Protagonist {
public:
    /**
     * @brief 主角健康状态枚举（与m_health属性关联）
     * @details 状态触发规则：HEALTHY(health≥70)、SICK(30≤health<70)、DEAD(health≤0)\n
     *              说明：受伤状态（INJURED）每一个游戏时下降一点健康
     */
    enum class HealthState {
        HEALTHY,   ///< 健康状态
        SICK,      ///< 生病状态
        INJURED,   ///< 受伤状态
        DEAD       ///< 死亡状态（游戏结束）
    };

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
     * @return std::unordered_map 键值对：key=属性名（"intelSci"/"intelArts"/"strength"/"money"/"health"），value=当前值
     */
    std::unordered_map<std::string, int> getBaseAttrs() const;

    /**
     * @brief 获取当前健康状态（含文本描述，供Controller传递给View）
     * @param outStateDesc 输出参数：状态文本描述（如“健康”“生病”）
     * @return HealthState 当前状态枚举（供Controller判断业务逻辑）
     */
    HealthState getHealthState(std::string& outStateDesc) const;

    // -------------------------- 2. 属性修改接口（供Controller/Item类交互） --------------------------
    /**
     * @brief 通用属性修改（供Controller调用，如学习/打工导致的属性变化）
     * @param attrKey 属性名（支持："intelSci"/"intelArts"/"strength"/"money"/"health"）
     * @param val 变化量（isAdd=true）或目标值（isAdd=false）
     * @param isAdd 是否为增减（true=val为变化量，false=val为目标值）
     * @return Message 操作结果：status=0（成功）/-1（属性名错误）/1（值越界）；msg=结果描述
     */
    Message updateAttr(const std::string& attrKey, int val, bool isAdd = true);

    /**
     * @brief 按物品效果修改属性（供Item类通调用，如牛奶/急救包效果）
     * @param item 物品实例（Item类需提供getEffect()接口返回属性变化）
     * @return Message 操作结果：status=0（成功）/-1（物品无效果）；msg=结果描述
     * @note 如果不想写getEffect()也可以调用updateAttr（const std::string& attrKey, int val, bool isAdd = true）函数
     */
    Message updateAttrByItem(const Item& item);

  

    // -------------------------- 3. 序列化/反序列化接口（供存档系统） --------------------------
    /**
     * @brief 序列化主角数据（供Controller写入存档文件）
     * @return std::string JSON格式字符串，包含：
     *         - 基础信息（id/name）属性（intelSci等）、健康状态
     */
    std::string serialize() const;

    /**
     * @brief 反序列化主角数据（供Controller从存档文件恢复）
     * @param data JSON格式字符串（需与serialize()输出格式一致）
     * @return Message 操作结果：status=0（成功）/-1（数据无效/格式错误）；msg=结果描述
     */
    Message deserialize(const std::string& data);

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

    // -------------------------- 私有辅助方法（仅内部数据校验/同步） --------------------------
    /**
     * @brief 校验属性值是否合法（如health 0~100，money≥0）
     * @param attrKey 属性名
     * @param val 待校验的值
     * @return bool true=合法，false=不合法
     */
    bool isValidAttr(const std::string& attrKey, int val) const;

    /**
     * @brief 同步健康状态（m_health变化后自动更新状态，无需外部调用）
     * @return HealthState 同步后的状态
     */
    HealthState syncHealthState() const;
};

