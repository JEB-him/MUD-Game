/**
 * @file Item.h
 * @author Xiaotian Wu(Cheongfan)
 * @brief 物品类及其派生类,涵盖除手机外，需求设计文档中列出的物品
 * @note 概念介绍： \n
 *          物品(或称为道具)分为"消耗品"和"用具"，分别具有use和equip(& unequip)两种逻辑 \n
 *          "消耗品"和"用具"各具有3个细分种类，合计6个细分种类；同种细分种类具有相似的use和equip(& unequip)逻辑 \n
 *          物品的基础信息包括：物品名称、物品描述、物品价值、物品类型(、装备状态)。 \n
 *          物品的作用是提供增减益，其实现逻辑参考群里发的"增减益设计建议——隐藏属性方案.docx" \n
 *          物品的基础信息和增减益数据目前考虑通过json文件存储和后续修改 \n
 * @note 接口使用说明: \n
 *          首先要创建ItemCreator类对象。 \n
 *          通过unique_ptr<Item> item_ptr = item_creator.create(item_name) 动态地创建名字为item_name的物品的对象。 \n
 *          由此创建的对象可以使用其父类的接口，即item、Equippable、Comsumable接口。 \n
 *          任何物品类不提供访问和修改物品增减益效果的任何方法，只能通过use或equipAndUnequip接口使其生效。 \n
 * @note 接口说明： \n
 *          物品item接口: \n
 *              item.getName()     物品名称 \n
 *              item.getDescription() 物品描述 \n
 *              item.getValue()  物品价值(价格) \n
 *              item.getIsConsumable()  物品是否为消耗品(false即为用具) \n
 *          用具Equippable接口: \n
 *              equippable.getEquipState()   装备状态 \n
 *              equippable.equipAndUnquip()   装备/卸下 \n
 *          消耗品Comsumable接口: \n
 *              comsumable.use()  使用物品(调用完记得释放指向该物品的智能指针) \n
 *          管理器ItemCreator接口: \n
 *              createItem(string item_name)  创建名字为item_name的物品(item_name即每个细分类别的类名，可查表"item_name.txt") \n
 * */

#pragma once
#include <iostream>
#include <string>
#include <memory>
#include <fstream>
#include <map>
#include "json.hpp"
#include "Protagonist.h"

using nlohmann::json;
using std::cout;
using std::endl;
using std::ifstream;
using std::make_unique;
using std::map;
using std::move;
using std::string;
using std::unique_ptr;

/**
 * @brief 物品类
 * @param name 物品名称
 * @param description 物品描述
 * @param value 物品价格
 * @param is_consumable 是否为消耗品
 * */
class Item
{
public:
    Item(const string &name, const string &description, float value, bool is_consumable);
    virtual ~Item() = default;

    /**
     * @brief 获取物品信息
     */
    string getName() const;
    string getDescription() const;
    int getValue() const;
    bool getIsConsumable() const;

    virtual void use(Protagonist& protagonist);
    virtual void equipAndUnequip(Protagonist& protagonist);

protected:
    string name;
    string description;
    float value;
    bool is_consumable;
};

/**
 * @brief 消耗品类
 */
class Consumable : public Item
{
public:
    Consumable(const string &name, const string &description, float value);
    virtual ~Consumable() = default;

    /**
     * @brief 使用消耗品
     * @note Controller通过指向物品对象的智能指针调用该方法使用“消耗品”，并改变主角属性
     */
    virtual void use(Protagonist &protagonist) = 0;
};

/**
 * @brief 用具类
 * @param equip_state 装备状态
 */
class Equippable : public Item
{
public:
    Equippable(const string &name, const string &description, float value);
    virtual ~Equippable() = default;

    /**
     * @brief 获取装备状态
     */
    bool getEquipState();

    /**
     * @brief 装备用具
     * @note Controller通过指向物品对象的智能指针调用该方法装备“用品”(，并改变主角属性)
     */
    virtual void equipAndUnequip(Protagonist &protagonist) = 0;

protected:
    bool equip_state;
};

/**
 * @brief 学习资料类:高等数学、大学英语、线性代数、思想道德、大学物理、军事概论、重点笔记-文、重点笔记-理、程序设计基础实践（作弊码兑换，学习效率999+）
 * @note  “用具”，确保外部参考equip_state合理调用equipAndUnequip
 * @param is_science true为理科，false为文科
 * @param intel_boost_rate 使用者提高的智力(文or理)比例增量
 * */
class StudyMaterial : public Equippable
{
public:
    StudyMaterial(const string &name, const string &description, float value, bool is_science, float intel_boost_rate);
    void equipAndUnequip(Protagonist &protagonist) override;

private:
    bool is_science;
    float intel_boost_rate;
};

/**
 * @brief 体育器材类（用具）：篮球、足球、羽毛球拍
 * @note 该类物品不作增益设计，直接将参与体育运动的收益写在活动的基础获得量里。
 * */
class SportsEquipment : public Equippable
{
public:
    SportsEquipment(const string &name, const string &description, float value);
    void equipAndUnequip(Protagonist &protagonist) override;
};

/**
 * @brief 学习辅助工具类(用具)：闹钟、护眼台灯
 * @note 作用：降低学习损耗
 * @param time_reduction_rate 时间消耗减少比率(闹钟)
 * @param health_preservation_rate 健康保护比率(护眼台灯)
 * */
class StudyAid : public Equippable
{
public:
    StudyAid(const string &name, const string &description, float value, float time_reduction_rate, float health_preservation_rate);
    void equipAndUnequip(Protagonist &protagonist) override;

private:
    float time_reduction_rate;
    float health_preservation_rate;
};

/**
 * @brief 食物类（消耗品）：压缩饼干、营养餐
 * @param strength_restore 体力恢复值（压缩饼干、营养餐）
 * @param health_restore 健康恢复值（营养餐）
 * @param time_cooldown 冷却时间（压缩饼干）
 * @param time_last_used 上次使用时间（压缩饼干）
 * @note time_last_used的变量类型需要和游戏的时间流逝设计对齐
 * */
class Food : public Consumable
{
public:
    Food(const string &name, const string &description, float value, float strength_restore, float health_restore, float time_cooldown, float time_last_used);
    void use(Protagonist &protagonist) override;

    /**
     * @brief 检查是否在冷却中
     */
    bool isOnCooldown() const;

private:
    float strength_restore;
    float health_restore;
    float time_cooldown;
    float time_last_used;
};

/**
 * @brief 学习辅助类（消耗品）：能量饮料、牛奶
 * @param intel_boost 智力基础增量（能量饮料、牛奶）
 * @param intel_boost_rate  智力比例增量（牛奶）
 * @param duration 持续时间（能量饮料、牛奶）
 * @param have_abuse_punish 是否有滥用惩罚(能量饮料)
 * @param is_used_today 今天是否使用过（能量饮料）
 * @param health_reduce 健康降低值（能量饮料）
 * */
class LearningAid : public Consumable
{
public:
    LearningAid(const string &name, const string &description, float value, float intel_boost, float intel_boost_rate, float duration, bool have_abuse_punish, bool is_used_today, float health_reduce);
    void use(Protagonist &protagonist) override;

private:
    float intel_boost;
    float intel_boost_rate;
    float duration;
    bool have_abuse_punish;
    bool is_used_today;
    float health_reduce;
};

/**
 * @brief 健康类（消耗品）：急救包、维生素
 * @param health_restore 健康恢复值(急救包)
 * @param duration 保护持续时间(维生素)
 * */
class HealthItem : public Consumable
{
public:
    HealthItem(const string &name, const string &description, float value, float health_restore, float duration);
    void use(Protagonist &protagonist) override;

private:
    float health_restore;
    float duration;
};

/**
 * @brief 用于管理物品对象的创建
 * @note 推荐用法：主角获得物品时，用智能指针创建对应的物品对象（格式见开头），并让背包的vector管理这个指针。
 * @param config_file_item 存储所有物品参数的json文件，方便物品参数读取和修改
 * @param config_item 读取json具体数据的对象
 */
class ItemCreator
{
public:
    ItemCreator(std::string file_name);
    ~ItemCreator();
    unique_ptr<Item> createItem(string item_name);

private:
    ifstream config_file_item;
    json config_item;
};
