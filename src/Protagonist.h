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
#include <cereal/types/unordered_map.hpp>
#include <cereal/types/memory.hpp>
#include <cereal/archives/binary.hpp>
#include <cereal/cereal.hpp>
#include   <cereal/types/string.hpp>
// 9月9日更改：添加默认构造函数，类内基础属性初始化，移除const限定符，添加隐藏属性

/**
 * @brief 主角属性枚举及健康状态枚举
 * @details ProtagonistAttr枚举定义了所有可修改的属性，HealthState枚举定义了健康状态
 *
 */
namespace BasicValue
{

    /**
     * @brief 主角属性枚举
     * @details ProtagonistAttr枚举定义了所有可修改的属性
     */
    enum class ProtagonistAttr
    {
        INTEL_SCI,                         ///< 智力-理
        INTEL_ARTS,                        ///< 智力-文
        STRENGTH,                          ///< 力量
        MONEY,                             ///< 金钱
        HEALTH,                            ///< 健康
        INTELSCI_BOOST,                    ///< 理科智力基础增量
        INTELARTS_BOOST,                   ///< 文科智力基础增量
        INTELSCI_BOOST_RATE,               ///< 理科智力基础增量百分比
        INTELARTS_BOOST_RATE,              ///< 文科智力基础增量百分比
        ID,                                ///< 主角唯一ID
        NAME,                              ///< 主角姓名
        LEARNING_TIME_REDUCTION_RATE,      ///< 学习时间消耗减少比率
        LEARNING_HEALTH_PRESERVATION_RATE, ///< 学习健康保留率
        VITMIN_EFFECT_RATE,                ///< 维生素作用系数    
        BUFF_ENERGY_DRINK,                 ///< 能量饮料buff状态
        BUFF_MILK,                         ///< 牛奶buff状态
        BUFF_VITAMINS,                     ///< 维生素buff状态
        T_BUFF_ENERGY_DRINK,               ///< 获得能量饮料buff的时间
        T_BUFF_MILK,                       ///< 获得牛奶buff的时间
        T_BUFF_VITAMINS,                   ///< 获得维生素buff的时间
        T_USED_COMPRESSED_CRACKER,         ///< 上次使用压缩饼干的时间
        T_USED_FIRST_AID_KIT,              ///< 上次使用急救包的时间
        IS_INJURED,                        ///< 受伤状态标志
        GAME_TIME                          ///< 游戏内时间
    };

    /**
     * @brief 主角健康状态枚举（与m_health属性关联）
     * @details 状态触发规则：HEALTHY(health≥70)、SICK(30≤health<70)、DEAD(health≤0)\n
     */
    enum class HealthState
    {
        HEALTHY, ///< 健康状态
        SICK,    ///< 生病状态
        INJURED, ///< 受伤状态
        DEAD     ///< 死亡状态（游戏结束）
    };
    /**
     * @brief 主角所在场景枚举 
     * @details 定义了主角可能所在的场景,现在未完善，需要场景类完善
     */
    enum class Scene{
        DORMITORY, ///< 宿舍
        CLASSROOM, ///< 教室
        CANTEEN,   ///< 食堂
        HOSPITAL,  ///< 医院
    };
}

class Protagonist
{
public:
    /**
     * @brief 构造函数：初始化主角基础数据（属性、状态）
     * @param protagonistId 主角唯一ID（如“OUC_2024_001”，供Controller区分角色）
     * @param name 主角姓名
     */
    Protagonist(const std::string &protagonistId, const std::string &name);

    /**
     * @brief 默认构造函数（仅供序列化/反序列化使用）
     * @note 使用默认构造函数后，需通过反序列化接口恢复数据
     */
    Protagonist() = default;

    /**
     * @brief 默认析构函数
     * @note 无需手动释放资源，使用默认析构函数
     */
    ~Protagonist() = default;

    /**
     * @brief 获取主角唯一ID
     * @return const std::string& 不可修改的ID
     */
    const std::string &getProtagonistId() ;

    /**
     * @brief 获取主角姓名
     * @return const std::string& 不可修改的姓名
     */
    const std::string &getName() ;

    /**
     * @brief 获取所有基础属性（供Controller同步UI/存档）
     * @return std::unordered_map 键值对：key=BasicValue::ProtagonistAttr，value=当前值
     */
    std::unordered_map<BasicValue::ProtagonistAttr, float> getBaseAttrs() const;

    /**
     * @brief 获取所有隐藏属性（供Controller同步UI/存档）
     * @return std::unordered_map 键值对：key=BasicValue::ProtagonistAttr，value=当前值
     */
    std::unordered_map<BasicValue::ProtagonistAttr, float> getHiddenAttrs() const;

    /**
     * @brief 获取当前游戏内时间
     * @return int 当前游戏内时间
     *
     */
    int getGameTime() const;

    /**
     * @brief 获取当前健康状态（含文本描述，供Controller传递给View）
     * @param outStateDesc 输出参数：状态文本描述（如“健康”“生病”）
     * @return HealthState 当前状态枚举（供Controller判断业务逻辑）
     */
    BasicValue::HealthState getHealthState(std::string &outStateDesc) const;


    /**
     * @brief 获取当前场景
     * @return BasicValue::Scene 当前场景枚举
     */
    BasicValue::Scene getCurrentScene() const;

    /**
     * @brief 获取当前位置
     * @return Position 当前坐标
     */
    Position getPosition() const;

    // -------------------------- 2. 属性修改接口（供Controller/Item类交互） --------------------------
    /**
     * @brief 通用属性修改（供Controller和Item类调用，如学习/打工导致的属性变化）
     * @param attr 属性名（支持：BasicValue::ProtagonistAttr）
     * @param val 变化量（isAdd=true）或目标值（isAdd=false）
     * @param isAdd 是否为增减（true=val为变化量，false=val为目标值）
     * @return Message 操作结果：status=0（成功）/-1（属性名错误）/1（值越界）；msg=结果描述
     * @note 对于受伤状态（IS_INJURED），val=1表示受伤，val=0表示恢复 isAdd参数为false
     */
    Message updateAttr(BasicValue::ProtagonistAttr attr, float val, bool isAdd = true);

    /**
     * @brief 设置主角姓名
     * @param name 新姓名
     */
    Message setName( std::string &name);

    /**
     * @brief 设置主角游戏内时间（仅供初始化）
     * @param time 新游戏内时间
     */
    Message setGameTime(int time);

    /**
     * @brief 增加游戏内时间
     * @param time 增加的时间，必须为正数
     * @return Message 操作结果：status=0（成功）/-1（time非正数）；msg=结果描述
     */
    Message addGameTime(int time);

    /**
     * @brief 设置主角的当前位置
     * @param position 新的位置坐标
     */
    Message setPosition(const Position &position);

    /**
     * @brief 设置主角的当前场景
     * @param scene 新的场景
     */
    Message setCurrentScene(BasicValue::Scene scene);

    // -------------------------- 3. 序列化/反序列化接口（供存档系统） --------------------------
    /**
     * @brief 序列化主角数据（供Controller写入存档文件）
     * @return std::string 序列化字符串，包含所有基础属性（id/name）和健康状态
    //  */
    // std::string serialize() const;

    // /**
    //  * @brief 反序列化主角数据（供Controller从存档文件恢复）
    //  * @param data 序列化字符串（需与serialize()输出格式一致）
    //  * @return Message 操作结果：status=0（成功）/-1（数据无效/格式错误）；msg=结果描述
    //  */
    // Message deserialize(const std::string &data);

    /**
     * @brief Cereal序列化支持
     * @tparam Archive Cereal归档类型 以binary形式存储
     */
    template <class Archive>
    void serialize(Archive &arch)
    {
        arch(CEREAL_NVP(m_protagonistId),
             CEREAL_NVP(m_name),
             CEREAL_NVP(m_intelSci),
             CEREAL_NVP(m_intelArts),
             CEREAL_NVP(m_strength),
             CEREAL_NVP(m_money),
             CEREAL_NVP(m_health),
             CEREAL_NVP(intelSci_boost),
             CEREAL_NVP(intelArts_boost),
             CEREAL_NVP(intelSci_boost_rate),
             CEREAL_NVP(intelArts_boost_rate),
             CEREAL_NVP(learning_time_reduction_rate),
             CEREAL_NVP(learning_health_preservation_rate),
             CEREAL_NVP(vitamins_effect_rate),
             CEREAL_NVP(buff_energy_drink),
             CEREAL_NVP(buff_milk),
             CEREAL_NVP(buff_vitamins),
             CEREAL_NVP(t_buff_energy_drink),
             CEREAL_NVP(t_buff_milk),
             CEREAL_NVP(t_buff_vitamins),
             CEREAL_NVP(t_used_compressed_cracker),
             CEREAL_NVP(t_used_first_aid_kit),
             CEREAL_NVP(isInjured),
             CEREAL_NVP(game_time),
             CEREAL_NVP(pos),
             CEREAL_NVP(current_scene));
    }
  



    /**
     * @brief 测试打印当前主角数据（供调试）
     */
    Message testPrint() const;

    /// @brief 读取JSON格式的主角数据
    /// @return 操作结果：status=0（成功）/-1（数据无效/格式错误）；msg=结果描述
    Message jsonReader();

private:
   
    // -------------------------- 核心成员变量 --------------------------
    std::string m_protagonistId = ""; ///< 主角唯一ID（不可修改）已经移除const
    std::string m_name = "";          ///< 主角姓名（不可修改） 已经移除const
    // 基础属性（需求定义，初始值按需求设定）
    int m_intelSci = 0;   ///< 智力-理（影响工作效率）
    int m_intelArts = 0;  ///< 智力-文（影响NPC交流）
    int m_strength = 100; ///< 体力（0~100）
    int m_money = 0;      ///< 金币（≥0）
    int m_health = 100;   ///< 健康状态（0~100，关联HealthState）
    // 隐藏属性
    float intelSci_boost = 0.0;                    ///< 理科智力基础增量	确保大于等于0
    float intelSci_boost_rate = 1.0;               ///< 理科智力比例增量	确保大于等于1
    float intelArts_boost = 0.0;                   ///< 文科智力基础增量	确保大于等于0
    float intelArts_boost_rate = 1.0;              ///< 文科智力比例增量	确保大于等于1
    float learning_time_reduction_rate = 1.0;      ///< 时间消耗减少比率(学)	确保大于等于0，小于等于1
    float learning_health_preservation_rate = 1.0; ///< 健康损失保护比率(学)	确保大于等于0，小于等于1
    float vitamins_effect_rate = 1.0;	           ///< 维生素作用系数    确保等于0或1
    bool buff_energy_drink = false;	               ///< 能量饮料buff状态
    bool buff_milk = false;			               ///< 牛奶buff状态
    bool buff_vitamins = false;		               ///< 维生素buff状态
    int t_buff_energy_drink = -10000;			   ///< 获得能量饮料buff的时间    确保大于等于0
    int t_buff_milk = -10000;				       ///< 获得牛奶buff的时间    确保大于等于0
    int t_buff_vitamins = -10000;			       ///< 获得维生素buff的时间    确保大于等于0
    int t_used_compressed_cracker = -10000;	       ///< 上次使用压缩饼干的时间    确保大于等于0
    int t_used_first_aid_kit = -10000;			   ///< 上次使用急救包的时间    确保大于等于0

    // 隐藏属性

    bool isInjured = 0; ///< 受伤状态标志

    int game_time = 0; ///< 游戏内时间

    Position pos; ///< 主角当前位置

    BasicValue::Scene current_scene = BasicValue::Scene::DORMITORY; ///< 主角当前所在场景，默认宿舍

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

    /**
     * @brief 校验姓名是否包含非法字符
     * @param name 待校验的姓名
     * @return bool true=合法，false=包含非法字符
     */
    bool isValidName(const std::string &name);
};
