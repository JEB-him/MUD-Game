/**
 * @file Protagonist.h
 * @author Haozhe Tang
 * @brief 主角类，管理OUCer主角的属性、行为与交互逻辑，兼容Map类与tools工具类
 * @details 实现需求中学习、打工、锻炼、吃饭、宿舍操作等核心玩法，与NPC/物品/地图协同工作
 */
#pragma once
#include "tools.h"
#include <string>
#include <unordered_map>
#include <vector>

// 前置声明（避免循环引用）
class NPC;

/**
 * @brief 主角类核心枚举（限定行为/物品类型，避免硬编码）
 * @details 定义了主角在游戏中的各种行为和物品类型，包括运动、食物、NPC交互和物品分类等
 *          主要根据群链接中的共享文档进行设计
 *              
 */
namespace ProtagonistDef {
    /// 运动类型（对应需求中运动场4种运动）
    enum class ExerciseType {
        BASKETBALL,  ///< 篮球
        FOOTBALL,    ///< 足球
        RUNNING,     ///< 跑步
        BADMINTON    ///< 羽毛球
    };

    /// 食物类型（对应需求中食堂可点餐品）
    enum class FoodType {
        COMPRESSED_BISCUIT,  ///< 压缩饼干（便宜，中等体力恢复，有CD）
        NUTRITIOUS_MEAL,     ///< 营养餐（昂贵，大量健康+体力恢复）
        MILK,                ///< 牛奶（功能用品，提升学习智力收益）
        COMMON_MEAL          ///< 普通餐（基础恢复，无特殊效果）
    };

    /// NPC交互类型（对应需求中不同NPC交互行为）
    enum class NPCActionType {
        ASK_HELP,       ///< 请教（学生/图书管理员）
        HELP_BUY_FOOD,  ///< 帮舍友带饭（舍友）
        WORK_FOR,       ///< 帮管理员干活（宿舍管理员）
        TALK_TEACHER,   ///< 与教师交流（教师）
        TALK_EMPLOYER   ///< 与招聘人员交流（兼职中介）
    };

    /// 物品类型（对应需求中功能用品/非用品）
    enum class ItemType {
        FUNCTIONAL,  ///< 功能用品（牛奶、急救包、能量饮料等，有CD）
        NON_USABLE   ///< 非用品（篮球、台灯、手机等，可装备）
    };
}

class Protagonist {
private:
    /**
     * @brief 物品数据结构体（管理背包内物品信息，替代复杂容器嵌套）
     */
    struct ItemData {
        std::string name;       ///< 物品名称（如“篮球”“急救包”）
        ProtagonistDef::ItemType type; ///< 物品类型
        int count;              ///< 物品数量（非用品默认为1）
        int remainingCD;        ///< 剩余冷却时间（单位：游戏小时，功能用品有效）
        bool isEquipped;        ///< 是否装备（非用品有效）

        /// 构造函数：初始化物品基础信息
        ItemData(std::string itemName, ProtagonistDef::ItemType itemType)
            : name(std::move(itemName)), type(itemType), count(1), remainingCD(0), isEquipped(false) {}
    };

    /**
     * @brief NPC社交关系结构体（存储与NPC的熟悉度/好感度）
     */
    struct NPCRelation {
        std::string name;       ///< NPC名称
        std::string type;       ///< NPC类型（如“Professor”“Roommate”）
        int familiarity;        ///< 熟悉度（宿舍管理员/舍友交互提升）
        int favorability;       ///< 好感度（通用社交提升）

        /// 构造函数：初始化NPC关系
        NPCRelation(std::string npcName, std::string npcType)
            : name(std::move(npcName)), type(std::move(npcType)), familiarity(0), favorability(0) {}
    };

    // -------------------------- 核心属性（需求定义） --------------------------
    int m_intelSci;          ///< 智力-理（影响工作效率）
    int m_intelArts;         ///< 智力-文（影响NPC交流）
    int m_strength;          ///< 体力（学习/打工/锻炼消耗/恢复）
    int m_money;             ///< 金币（打工收入/物品开销）
    int m_health;            ///< 健康状态（0~100，工伤/吃坏肚子降低）
    int m_currentTime;       ///< 当前游戏时间（初始0，1000结束，100=1周）
    Position m_pos;          ///< 当前坐标（兼容tools.h的Position类）
    std::string m_currentArea; ///< 当前区域（宿舍/教学楼/运动场等，判断存档权限）
    std::string m_currentJobId; ///< 当前工作ID（空串=无工作，打工系统用）
    bool m_isBusy;           ///< 是否忙碌（学习/打工中不可移动）

    // -------------------------- 资源管理（需求定义） --------------------------
    std::unordered_map<std::string, ItemData> m_backpack; ///< 背包（物品ID→物品数据）
    std::unordered_map<std::string, NPCRelation> m_npcRelations; ///< NPC关系（NPC ID→关系数据）

    // -------------------------- 私有辅助方法（逻辑封装） --------------------------
    /**
     * @brief 检查属性是否足够（体力/金币）
     * @param needStrength 所需体力
     * @param needMoney 所需金币
     * @return true：属性足够；false：属性不足
     */
    bool checkAttrEnough(int needStrength, int needMoney = 0) const;

    /**
     * @brief 查找背包中的物品
     * @param itemId 物品唯一ID
     * @param outItem 输出参数：物品数据（找到则赋值）
     * @return true：找到物品；false：未找到
     */
    bool findItem(const std::string& itemId, ItemData& outItem) const;

    /**
     * @brief 查找NPC关系
     * @param npcId NPC唯一ID
     * @param outRelation 输出参数：NPC关系（找到则赋值）
     * @return true：找到NPC；false：未找到
     */
    bool findNPCRelation(const std::string& npcId, NPCRelation& outRelation) const;

    /**
     * @brief 减少所有功能物品的CD（每游戏小时调用一次）
     */
    void reduceItemCD();

    /**
     * @brief 检查游戏是否结束（时间到1000或健康≤0）
     * @return true：游戏结束；false：游戏继续
     */
    bool isGameOver() const;

public:
    // -------------------------- 构造/析构函数 --------------------------
    /**
     * @brief 构造函数：初始化主角基础状态（出生点=宿舍）
     * @param protagonistId 主角唯一ID（如“OUC_2024_001”）
     * @param name 主角姓名
     * @param startPos 初始坐标（默认(0,0)，Map可覆盖）
     */
    Protagonist(const std::string& protagonistId, const std::string& name, const Position& startPos = Position());

    /**
     * @brief 析构函数：默认（无动态内存申请）
     */
    ~Protagonist() = default;

    // -------------------------- 基础状态接口（供外部查询/更新） --------------------------
    /**
     * @brief 获取所有基础属性（供UI显示/存档）
     * @return 属性映射表（key=属性名，value=属性值）
     */
    std::unordered_map<std::string, int> getBaseAttrs() const;

    /**
     * @brief 更新单个属性（支持增减/覆盖，处理边界值）
     * @param attrKey 属性名（如“intelSci”“strength”）
     * @param val 目标值/变化量
     * @param isAdd 是否为增减（true=增减，false=覆盖）
     * @return Message 操作结果（0=成功，-1=属性名错误，1=值越界）
     */
    Message updateAttr(const std::string& attrKey, int val, bool isAdd = true);

    /**
     * @brief 获取当前位置（供Map类交互）
     * @return Position 当前坐标
     */
    Position getPos() const;

    /**
     * @brief 设置当前位置（Map类移动后调用）
     * @param newPos 新坐标
     * @param area 新区域（如“Dormitory”“Classroom”）
     * @return Message 操作结果（0=成功，1=区域名无效）
     */
    Message setPos(const Position& newPos, const std::string& area);

    /**
     * @brief 检查是否忙碌（供Controller判断是否可移动/交互）
     * @return bool true=忙碌，false=空闲
     */
    bool isBusy() const;

    // -------------------------- 核心行为接口（需求玩法） --------------------------
    /**
     * @brief 学习（文科/理科，对应需求“教学楼学习”）
     * @param subjectType 学科类型（0=文科，1=理科）
     * @param duration 学习时长（单位：游戏小时，默认1）
     * @return Message 操作结果（0=成功，1=体力不足，-1=参数错误）
     */
    Message study(int subjectType, int duration = 1);

    /**
     * @brief 参加考试（日常/期末，对应需求“考试”）
     * @param isFinal 是否为期末考试（true=期末，false=日常）
     * @return Message 操作结果（0=通过，1=未通过，-1=体力不足）
     * @note 通过则父母给金币（日常+200，期末+500）
     */
    Message takeExam(bool isFinal = false);

    /**
     * @brief 应聘工作（对应需求“打工-应聘公司”）
     * @param jobId 工作ID（如“real_001”=真实工作，“fake_001”=虚假工作）
     * @return Message 操作结果（0=找到工作，1=被骗，-1=金币不足/忙碌）
     */
    Message applyJob(const std::string& jobId);

    /**
     * @brief 执行工作（对应需求“打工-打工场所”）
     * @return Message 操作结果（0=正常收入，1=失误扣钱，2=被裁员，3=工伤）
     */
    Message work();

    /**
     * @brief 锻炼（对应需求“运动场锻炼”）
     * @param exerciseType 运动类型（0=篮球，1=足球，2=跑步，3=羽毛球）
     * @return Message 操作结果（0=成功，1=金币不足，2=缺少道具，-1=参数错误）
     */
    Message doExercise(int exerciseType);

    /**
     * @brief 吃饭（对应需求“食堂点餐”）
     * @param foodType 食物类型（0=压缩饼干，1=营养餐，2=牛奶，3=普通餐）
     * @return Message 操作结果（0=正常恢复，1=吃太多闹肚子，-1=金币不足）
     */
    Message eatFood(int foodType);

    /**
     * @brief 宿舍存档（对应需求“宿舍-保存游戏”）
     * @return Message 操作结果（0=存档成功，1=非宿舍区域，-1=存档失败）
     */
    Message saveGame() const;

    /**
     * @brief 宿舍恢复体力（对应需求“宿舍-恢复体力”）
     * @param duration 休息时长（单位：游戏小时，默认2）
     * @return Message 操作结果（0=成功，1=非宿舍区域，-1=参数错误）
     */
    Message recoverStrengthInDorm(int duration = 2);

    // -------------------------- NPC交互接口（需求社交逻辑） --------------------------
    /**
     * @brief 与NPC交互（对应需求“NPC交互”）
     * @param npcId NPC唯一ID
     * @param npcType NPC类型（如“Professor”“Roommate”“DormManager”）
     * @param actionType 交互类型（0=请教，1=帮带饭，2=帮忙干活，3=教师交流）
     * @return Message 操作结果（0=成功，1=条件不足，-1=NPC不存在）
     */
    Message interactWithNPC(const std::string& npcId, const std::string& npcType, int actionType);

    // -------------------------- 物品交互接口（需求道具逻辑） --------------------------
    /**
     * @brief 使用功能物品（对应需求“功能用品使用”）
     * @param itemId 物品ID（如“energy_drink”“first_aid”）
     * @return Message 操作结果（0=成功，1=CD未到，2=物品不存在，-1=非功能用品）
     */
    Message useFunctionalItem(const std::string& itemId);

    /**
     * @brief 装备非用品（对应需求“非用品装备”）
     * @param itemId 物品ID（如“basketball”“eye_lamp”）
     * @return Message 操作结果（0=装备成功，1=已装备，2=物品不存在，-1=非非用品）
     */
    Message equipNonUsableItem(const std::string& itemId);

    /**
     * @brief 添加物品到背包（NPC赠送/购买）
     * @param itemId 物品ID
     * @param itemName 物品名称
     * @param itemType 物品类型（0=功能用品，1=非用品）
     * @param count 数量（默认1）
     * @return Message 操作结果（0=成功，-1=参数错误）
     */
    Message addItem(const std::string& itemId, const std::string& itemName, int itemType, int count = 1);

    // -------------------------- 持久化接口（存档/读档） --------------------------
    /**
     * @brief 序列化主角状态（供存档系统）
     * @return 序列化字符串（JSON格式，包含属性、物品、NPC关系）
     */
    std::string serialize() const;

    /**
     * @brief 反序列化主角状态（供读档系统）
     * @param data 序列化字符串（JSON格式）
     * @return Message 操作结果（0=成功，-1=数据无效）
     */
    Message deserialize(const std::string& data);
};