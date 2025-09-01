/**
 * @file Item.h
 * @author Xiaotian Wu
 * @brief 物品类及其派生类,涵盖除手机外，需求设计文档中列出的物品
 * @note 测试说明：
 *          由于本文件的类的测试需要背包系统，且一些物品涉及时间系统、健康值系统等未实现的逻辑，
 *          其具体的方法难以书写。因此尚未做检验测试。
 * @note 未完成说明：
 *          本文件未完成部分用"to do:"进行了标识。未完成内容：消耗品的使用(use)逻辑、物品对象的创建(create)
 * @note 概念介绍：
 *          物品(或称为道具)分为"消耗品"和"用具"，分别具有use和equip(& unequip)两种逻辑
 *          物品的基础信息包括：物品名称、物品描述、物品价值、物品类型(、装备状态)。
 *          物品的作用是提供增减益，其实现逻辑参考群里发的"增减益设计建议——隐藏属性方案.docx"
 *          物品的基础信息和增减益数据目前考虑通过json文件存储和后续修改
 * @note 接口使用说明:
 *          首先要创建ItemManager类对象。
 *          通过unique_ptr<Item> item_ptr = item_manager.createXXX()动态创建物品“XXX”的对象。
 *          由此创建的对象可以使用其父类的接口，即item、Equippable、Comsumable接口。
 *          任何物品类不提供访问和修改物品增减益效果的任何方法，只能通过use或equipAndUnequip接口使其生效。
 * @note 接口说明：
 *          物品item接口:
 *              item.getName()     物品名称
 *              item.getDescription() 物品描述
 *              item.getValue()  物品价值(价格)
 *              item.getIsConsumable()  物品是否为消耗品(false即为用具)
 *          用具Equippable接口:
 *              equippable.getEquipState()   装备状态
 *              equippable.equipAndUnquip()   装备/卸下
 *          消耗品Comsumable接口:
 *              comsumable.use()  使用物品(调用完记得释放指向该物品的智能指针)
 * */
#pragma once
#include<string>
#include"Protagonist.h"
#include<memory>
#include <fstream>
#include "json.hpp"

using std::string;
using std::cout, std::endl;
using std::unique_ptr, std::make_unique;
using std::move;
using nlohmann::json;
using std::ifstream

/**
 * @brief 物品类
 * @param name 物品名称
 * @param description 物品描述
 * @param value 物品价格
 * @param is_consumable 是否为消耗品
 * */
class Item {
public:
    Item(const string& name, const string& description, float value, bool is_consumable);
    virtual ~Item() = default;

    /**
    * @brief 获取物品信息
    */
    string getName() const;
    string getDescription() const;
    int getValue() const;
    bool getIsConsumable() const;

protected:
    string name;
    string description;
    float value;
    bool is_consumable;
};

/**
 * @brief 消耗品类
 */
class Consumable:public Item {
public:
    Consumable(const string& name, const string& description, float value);
    virtual ~Consumable() = default;

    /**
    * @brief 使用消耗品
    * @note Controller通过指向物品对象的智能指针调用该方法使用“消耗品”，并改变主角属性
    */
    virtual void use(Protagonist& protagonist);
};

/**
 * @brief 用具类
 * @param equip_state 装备状态
 */
class Equippable {
public:
    Equippable(const string& name, const string& description, float value);
    virtual ~Equippable() = default;

    /**
     * @brief 获取装备状态
     */
    bool getEquipState();

    /**
    * @brief 装备用具
    * @note Controller通过指向物品对象的智能指针调用该方法装备“用品”(，并改变主角属性)
    */
    virtual void equipAndUnquip(Protagonist& protagonist);
protected:
    bool equip_state;
};

/**
 * @brief 学习资料类:高等数学、大学英语、线性代数、思想道德、大学物理、军事概论、重点笔记、程序设计基础实践（作弊码兑换，学习效率999+）
 * @note  “用具”，确保外部参考equip_state合理调用equip和unequip
 * @param is_science true为理科，false为文科
 * @param intel_boost_rate 使用者提高的智力(文or理)比例增量
 * */
class StudyMaterial : public Equippable {
public:
    StudyMaterial(const string& name, const string& description, float value, bool is_science, float intel_boost_rate);
    void equipAndUnequip(Protagonist& protagonist) override;
private:
    bool is_science;
    float intel_boost_rate;
};

/**
 * @brief 体育器材类（用具）：篮球、足球、羽毛球拍
 * @note 该类物品不作增益设计，直接将参与体育运动的收益写在活动的基础获得量里。
 * */
class SportsEquipment : public Equippable {
public:
    SportsEquipment(const string& name, const string& description, float value);
    void equipAndUnequip(Protagonist& protagonist) override;
};

/**
 * @brief 学习辅助工具类(用具)：闹钟、护眼台灯
 * @note 作用：降低学习损耗
 * @param time_reduction_rate 时间消耗减少比率(闹钟)
 * @param health_preservation_rate 健康保护比率(护眼台灯)
 * */
class StudyAid : public Equippable {
public:
    StudyAid(const string& name, const string& description, float value, float time_reduction_rate, float health_preservation_rate);
    void equipAndUnequip(Protagonist& protagonist) override;
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
class Food : public Consumable {
public:
    Food(const string& name, const string& description, float value, float strength_restore, float health_restore, float time_cooldown, float time_last_used);
    void use(Protagonist& protagonist) override;

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
class LearningAid : public Consumable {
public:
    LearningAid(const string& name, const string& description, float value, float intel_boost, float intel_boost_rate, float duration, bool have_abuse_punish, bool is_used_today, float health_reduce);
    void use(Protagonist& protagonist) override;
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
class HealthItem : public Item {
public:
    HealthItem(const string& name, const string& description, float value, float health_restore, float duration);
    void use(Protagonist& protagonist) override;
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
class ItemManager {
public:
    ItemManager(char[] file_name);
    ~ItemManager();
    unique_ptr<Item> createAdvancedMathematics();
    unique_ptr<Item> createCollegeEnglish();
    unique_ptr<Item> createLinearAlgebra();
    unique_ptr<Item> createEthics();
    unique_ptr<Item> createCollegePhysics();
    unique_ptr<Item> createIntroductionOfMilitary();
    unique_ptr<Item> createKeyNotes();
    unique_ptr<Item> createBasicProgrammingPractice();

    unique_ptr<Item> createBasketball();
    unique_ptr<Item> createSoccer();
    unique_ptr<Item> createBadmintonRacket();

    unique_ptr<Item> createAlarmClock();
    unique_ptr<Item> createEyeProtectorLamps();

    unique_ptr<Item> createCompressedCrackers();
    unique_ptr<Item> createNutritiousMeals();

    unique_ptr<Item> createEnergyDrink();
    unique_ptr<Item> createMilk();

    unique_ptr<Item> createFirstAidKit();
    unique_ptr<Item> createVitamins();
private:
    ifstream config_file_item;
    json config_item;
};

/**
* @brief 方法实现
*/

/**
* @brief Item类构造函数
*/
Item::Item(const string& name, const string& description, float value, bool is_consumable) :
    name(name), description(description), value(value), is_consumable(is_consumable) { }

/**
* @brief Item类的几个返回函数，不作赘述
*/
string Item::getName() const {
    return name;
}
int Item::getValue() const {
    return value;
}
string Item::getDescription() const {
    return description;
}
bool Item::getIsConsumable() const {
    return is_consumable;
}

/**
* @brief Consumable类构造函数
*/
Consumable::Consumable(const string& name, const string& description, float value):
    Item(name, description, value, true) { }

/**
* @brief Equippable类构造函数
*/
Equippable::Equippable(const string& name, const string& description, float value):
    Item(name, description, value, false) { }

/**
* @brief 用具装备状态返回函数
*/
bool Equippable::getEquipState() {
    return equip_state;
}

/**
* @brief 学习资料类构造函数
*/
StudyMaterial::StudyMaterial(const string& name, const string& description, float value, bool is_science, float intel_boost_rate) :
    Equippable(name, description, value), is_science(is_science), intel_boost_rate(intel_boost_rate) { }

/**
* @brief 装备学习资料，提高（文或理）智力比例增量
* @note  item.getIsConsumable() == false时可调用该接口
*/
void StudyMaterial::equipAndUnequip(Protagonist& protagonist) override {

    /**
    * @brief 装备前判断是否未装备
    */
    if (!equip_state) {

        /**
        * @brief 装备状态：已装备
        */
        equip_state = true;

        /**
        * @brief 根据学习资料的文理类型，提高对应的智力比例增量
        */
        switch (is_science) {
        case true:
            protagonist.resetIntelSciBoostRate(protagonist.getIntelSciBoostRate() + intel_boost_rate);
        case false:
            protagonist.resetIntelArtsBoostRate(protagonist.getIntelArtsBoostRate() + intel_boost_rate);
        default:
            break;
        }

        /**
        * @brief feedback，后续可用View的方法替换
        */
        cout << "\"" << name << "\"" << "已装备。" << endl;
    }
    else {
        /**
        * @brief 与上一个scope相反
        */
        equip_state = false;
        switch (is_science) {
        case true:
            protagonist.resetIntelSciBoostRate(protagonist.getIntelSciBoostRate() - intel_boost_rate);
        case false:
            protagonist.resetIntelArtsBoostRate(protagonist.getIntelArtsBoostRate() - intel_boost_rate);
        default:
            break;
        }
        cout << "\"" << name << "\"" << "已取消装备。" << endl;
    }
}

/**
* @brief 体育器材类构造函数
*/
SportsEquipment::SportsEquipment(const string& name, const string& description, float value) :
    Equippable(name, description, value) { }

/**
* @brief 装备体育器材，允许参加特定需要器材的运动
* @note  item.getIsConsumable() == false时存在该接口
*/
void SportsEquipment::equipAndUnequip(Protagonist& protagonist) override {

    /**
    * @brief 装备前判断是否未装备
    */
    if (!equip_state) {

        /**
        * @brief 装备状态：已装备
        */
        equip_state = true;

        /**
        * @brief feedback，后续可用View的方法替换
        */
        cout << "\"" << name << "\"" << "已装备。" << endl;
    }
    else {

        /**
        * @brief 与上一个scope相反
        */
        equip_state = false;
        cout << "\"" << name << "\"" << "已取消装备。" << endl;
    }
}

/**
* @brief 学习辅助工具类构造函数
*/
StudyAid::StudyAid(const string& name, const string& description, float value, float time_reduction_rate, float health_preservation_rate) :
    Equippable(name, description, value), time_reduction_rate(time_reduction_rate), health_preservation_rate(health_preservation_rate) { }

/**
* @brief 装备学习辅助工具，降低学习损耗
* @note  item.getIsConsumable() == false时存在该接口
*/
void StudyAid::equip(Protagonist& protagonist) override {
    if (!equip_state) {
        
        /**
        * @brief 装备状态：已装备
        */
        equip_state = true;

        /**
        * @brief 学习消耗时间和健康，基础消耗量要乘以一个rate = 1,这里把这个rate降低至小于1。
        */
        protagonist.resetLearningTimeReductionRate(protagonist.getLearningTimeReductionRate() - time_reduction_rate);
        protagonist.resetLearningHealthPreservationRate(protagonist.getLearningHealthPreservationRate() - health_preservation_rate);

        /**
        * @brief feedback，后续可用View的方法替换
        */
        cout << "\"" << name << "\"" << "已装备。" << endl;
    }
    else {

        /**
        * @brief 与上一个scope相反
        */
        equip_state = false;

        protagonist.resetLearningTimeReductionRate(protagonist.getLearningTimeReductionRate() + time_reduction_rate);
        protagonist.resetLearningHealthPreservationRate(protagonist.getLearningHealthPreservationRate() + health_preservation_rate);

        cout << "\"" << name << "\"" << "已取消装备。" << endl;
    }
}


/**
* @brief 食品类构造函数
*/
Food::Food(const string& name, const string& description, float value, float strength_restore, float health_restore, float time_cooldown, float time_last_used) :
    Consumable(name, description, value), strength_restore(strength_restore), health_restore(health_restore), time_cooldown(time_cooldown), time_last_used(time_last_used) { }

/**
* @brief 使用“食品类物品”
* @note  在外部调用item.use接口使用了消耗品后，记得将该消耗品对象释放
* to do: 这里需要获取当前时间的接口now(),需要等时间(计时)的具体逻辑出来后再具体修改
*/
void Food::use(Protagonist& protagonist) override {

    /**
    * @brief 等CD
    */
    if (/*now()*/ -time_last_used > time_cooldown) {

        /**
        * @brief 恢复主角一定体力和健康，建议reset接口自行判断是否超出MAX_STRENGTH和MAX_HEALTH
        */
        protagonist.resetStrength(protagonist.getStrength() + strength_restore);
        protagonist.resetHealth(protagonist.getHealth() + health_restore);

        cout << "已使用消耗品" << "\"" << name << "\"" <<  << endl;
    }
    else {
        cout << "冷却中,暂时无法使用该道具" << endl;
    }
}

/**
* @brief 学习辅助类构造函数
*/
LearningAid::LearningAid(const string& name, const string& description, float value, float intel_boost, float intel_boost_rate, float duration, bool have_abuse_punish, bool is_used_today, float health_reduce) :
    Consumable(name, description, value), intel_boost(intel_boost), intel_boost_rate(intel_boost_rate), duration(duration), have_abuse_punish(have_abuse_punish), is_used_today(is_used_today), health_reduce(health_reduce) { }

/**
* @brief 使用“学习辅助类物品”
* @note  在外部调用item.use接口使用了消耗品后，记得将该消耗品对象释放
*/
void Food::use(Protagonist& protagonist) override {

    /**
    * to do:此处需要设置定时器，在一定时间(duration)后增益解除(执行解除增益的函数)
    *       具体逻辑在完善时间(计时)逻辑后写
    */
}

/**
* @brief 健康类构造函数
*/
HealthItem::HealthItem(const string& name, const string& description, float value, float health_restore, float duration) :
    Consumable(name, description, value), health_restore(health_restore), duration(duration) { }

/**
* @brief 使用“健康类物品”
* @note  在外部调用item.use接口使用了消耗品后，记得将该消耗品对象释放
*/
void HealthItem::use(Protagonist& protagonist) override {
    protagonist.resetHealth(protagonist.getMealth() + health_restore);
    /**
    * to do:此处需要暂停健康值损耗一段时间，具体实现需要考虑健康值损耗逻辑和时间（计时）逻辑
    *       待上述逻辑完备后再写
    */
}

/**
* @brief 物品管理器类
*/

/**
 * @brief 初始化json文件读取
 * @param file_name 包含所有物品参数信息的json文件的名字（字符串）
 */
ItemManager::ItemManager(char[] file_name) {
    config_file_item(file_name);
    config_file_item >> config_item;
    config_file.close();
}

unique_ptr<Item> ItemManager::createAdvancedMathematics() {
    /**
    * @brief 读取物品参数
    */
    string name = (string)config_item["advanced_mathematics"]["name"];
    string description = (string)config_item["advanced_mathematics"]["description"];
    float value = (float)config_item["advanced_mathematics"]["value"];
    bool is_science = (bool)config_item["advanced_mathematics"]["is_science"];
    float intel_boost_rate = (float)config_item["advanced_mathematics"]["intel_boost_rate"];

    /**
    * @brief 创建物品对象并返回
    */
    unique_ptr<Item> item_ptr = make_unique<StudyMaterial>(name，description, value, is_science, intel_boost_rate);
    return item_prt;
}

/**
* to do: 其他物品的创建同理。目前json的数据结构及其读取效果还有待检验，暂时不写下面的内容。
*/