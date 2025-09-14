/**
 * @file Item.h
 * @author Xiaotian Wu(Cheongfan)
 * @brief 物品类及其派生类,涵盖除手机外，需求设计文档中列出的物品
 * @note 概念介绍： \n
 *          物品(或称为道具)分为"消耗品"和"用具"，分别具有use和equip(& unequip)两种逻辑 \n
 *          "消耗品"和"用具"各具有2和3个细分种类，合计5个细分种类；同种细分种类具有相似的use和equip(& unequip)逻辑 \n
 *          物品的基础信息包括：物品名称、物品描述、物品价值、物品类型(、装备状态)。 \n
 *          物品的作用是提供基础数值的提升或buff(隐藏数值的提升)
 *          物品的基础信息和增减益数据目前考虑通过json文件存储和后续修改 \n
 * */

#pragma once
#include <iostream>
#include <string>
#include <memory>
#include <fstream>
#include <map>
#include<sstream>
#include "json.hpp"
#include "Protagonist.h"

using std::string;
using std::cout;
using std::endl;
using std::unique_ptr;
using std::make_unique;
using std::move;
using nlohmann::json;
using std::ifstream;
using std::map;
using std::stringstream;

/**
 * @brief 物品类
 * @param name 物品名称
 * @param description 物品描述
 * @param value 物品价值
 * @param is_consumable 是否为消耗品
 * */
class Item
{
public:
    Item(const string &name, const string &description, float value, bool is_consumable);
    virtual ~Item() = default;

    //获取物品信息
    string getName() const;//物品名字
    string getDescription() const;//物品描述
    int getValue() const;//物品价值
    bool getIsConsumable() const;//是否为消耗品
    virtual bool isOnCD(Protagonist& protagonist)const;//该物品是否处于冷却阶段

    virtual void use(Protagonist& protagonist);//使用物品
    virtual void equipAndUnequip(Protagonist& protagonist);//装备/卸下物品

protected:
    string name;
    string description;
    float value;
    bool is_consumable;
    stringstream ss;
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
     * @param protagonist 主角类引用
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
     * @param 主角类引用
     */
    virtual void equipAndUnequip(Protagonist &protagonist) = 0;

protected:
    bool equip_state;
};

/**
 * @brief 学习资料类:高等数学、大学英语、线性代数、思想道德、大学物理、军事概论、重点笔记-文、重点笔记-理、程序设计基础实践（作弊码兑换，学习效率999+）
 * @param is_science 是否为理科资料（true为理科，false为文科）
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
 * @param have_cd   物品是否有CD(压缩饼干、营养餐)
 * */
class Food : public Consumable
{
public:
    Food(const string& name, const string& description, float value, float strength_restore, float health_restore, float time_cooldown, bool have_cd);
    bool isOnCD(Protagonist& protagonist)const override;
    void use(Protagonist& protagonist) override;
private:
    float strength_restore;
    float health_restore;
    float time_cooldown;
    bool have_cd;
};

/**
 * @brief 学习辅助类（消耗品）：能量饮料、牛奶
 * @param intel_boost 智力基础增量（能量饮料、牛奶）
 * @param intel_boost_rate  智力比例增量（牛奶）
 * @param duration 持续时间（能量饮料、牛奶）
 * @param have_abuse_punish 是否有滥用惩罚(能量饮料)
 * @param health_reduce 健康降低值（能量饮料）
 * @param punish_cd 防止滥用的最低时间间隔（能量饮料）
 * */
class LearningAid : public Consumable
{
public:
    LearningAid(const string& name, const string& description, float value, float intel_boost, float intel_boost_rate, float duration, bool have_abuse_punish, float health_reduce, int punish_cd);
    void use(Protagonist& protagonist) override;
private:
    float intel_boost;
    float intel_boost_rate;
    float duration;
    bool have_abuse_punish;
    float health_reduce;
    int punish_cd;
};

/**
 * @brief 健康类（消耗品）：急救包、维生素
 * @param health_restore 健康恢复值(急救包)
 * @param duration 保护持续时间(维生素)
 * @param time_cooldown 物品使用冷却时间(急救包)
 * @param have_cd   物品是否有CD(急救包、维生素)
 * */
class HealthItem : public Consumable
{
public:
    HealthItem(const string& name, const string& description, float value, float health_restore, float duration, float time_cooldown, bool have_cd);
    bool isOnCD(Protagonist& protagonist)const override;
    void use(Protagonist& protagonist) override;
private:
    float health_restore;
    float duration;
    float time_cooldown;
    bool have_cd;
};

/**
 * @brief 用于管理物品对象的创建
 * @param config_file_item 存储所有物品参数的json文件，方便物品参数读取和修改
 * @param config_item 读取json具体数据的对象
 */
class ItemCreator
{
public:
    ItemCreator();
    ~ItemCreator() = default;
    unique_ptr<Item> createItem(string item_name);

private:
    ifstream config_file_item;
    json config_item;
};
