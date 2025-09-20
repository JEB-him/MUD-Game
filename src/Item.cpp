/**
  * @file Item.cpp
  * @author Xiaotian Wu(Cheongfan)
  * @brief 物品类及其派生类的实现方法
  * @note 测试说明： \n
  *          由于本文件的类的测试还需要view、now()、主角类新增属性相关接口等外部接口， \n
  *          一些功能需要上述完成后测试。
  * @note 未完成说明： \n
  *          未完成时间逻辑相关的功能测试，目前无编译问题。
  * */

#include"Item.h"
#include"View.h"
#include<filesystem>
#include<vector>
#include<cmath>

/**
* @brief ItemBasicInf类构造函数
*/
ItemBasicInf::ItemBasicInf(string name, string description, int value):name(name),description(description),value(value) { }

  /**
    * @brief 物品类型枚举定义
    * @details 包含五种物品细分类型（学习资料、学习辅助工具、食品、学习辅助食品、健康类物品）及未知类型
    */
enum class ItemType {
    STUDY_MATERIAL,
    STUDY_AID,
    FOOD,
    LEARNING_AID,
    HEALTH_ITEM,
    UNKNOWN
};

/**
  * @brief 将字符串转换为物品类型枚举
  * @param item_class 物品类型字符串
  * @return 对应的ItemType枚举值，未找到匹配时返回ItemType::UNKNOWN
  */
ItemType getItemType(const string& item_class) {
    static const map<string, ItemType> stringToEnum = {
        {"study_material", ItemType::STUDY_MATERIAL},
        {"study_aid", ItemType::STUDY_AID},
        {"food", ItemType::FOOD},
        {"learning_aid", ItemType::LEARNING_AID},
        {"health_item", ItemType::HEALTH_ITEM},
    };
    auto it = stringToEnum.find(item_class);
    if (it != stringToEnum.end()) {
        return it->second;
    }
    return ItemType::UNKNOWN;
}

/**
 * @brief Item类构造函数
 * @param name 物品名称
 * @param description 物品描述
 * @param value 物品价值
 * @param is_consumable 是否为消耗品
 */
Item::Item(const string& name, const string& description, float value, bool is_consumable) :
    name(name), description(description), value(value), is_consumable(is_consumable) { }

/**
 * @brief 获取物品名称
 * @return 物品名称字符串
 */
string Item::getName() const {
    return name;
}

/**
 * @brief 获取物品价值
 * @return 物品价值数值
 */
int Item::getValue() const {
    return value;
}

/**
 * @brief 获取物品描述
 * @return 物品描述字符串
 */
string Item::getDescription() const {
    return description;
}

/**
 * @brief 获取物品消耗品状态
 * @return 是否为消耗品的布尔值
 */
bool Item::getIsConsumable() const {
    return is_consumable;
}

/**
 * @brief 检查物品是否处于冷却状态（虚函数）
 * @param protagonist 主角对象引用
 * @return 总是返回false（基类默认实现）
 */
bool Item::isOnCD(Protagonist& protagonist)const {
    return false;
}

/**
 * @brief 使用物品（虚函数）
 * @param protagonist 主角对象引用
 */
 void Item::use(Protagonist& protagonist){ }

/**
 * @brief 装备/卸下物品（虚函数）
 * @param protagonist 主角对象引用
 */
void Item::equipAndUnequip(Protagonist& protagonist) { }

/**
 * @brief Consumable类构造函数
 * @param name 物品名称
 * @param description 物品描述
 * @param value 物品价值
 */
Consumable::Consumable(const string& name, const string& description, float value) :
    Item(name, description, value, true) { }

/**
 * @brief Equippable类构造函数
 * @param name 物品名称
 * @param description 物品描述
 * @param value 物品价值
 */
Equippable::Equippable(const string& name, const string& description, float value) :
    Item(name, description, value, false), equip_state(false) { }

/**
 * @brief 获取装备状态
 * @return 当前装备状态的布尔值
 */
bool Equippable::getEquipState() {
    return equip_state;
}

/**
 * @brief StudyMaterial类构造函数
 * @param name 物品名称
 * @param description 物品描述
 * @param value 物品价值
 * @param is_science 是否为理科学习资料
 * @param intel_boost_rate 智力提升比例
 */
StudyMaterial::StudyMaterial(const string& name, const string& description, float value, bool is_science, float intel_boost_rate) :
    Equippable(name, description, value), is_science(is_science), intel_boost_rate(intel_boost_rate) { }

/**
 * @brief 装备/卸下学习资料
 * @details 根据学习资料的文理类型，提高或降低对应的智力比例增量
 * @param protagonist 主角对象引用
 */
void StudyMaterial::equipAndUnequip(Protagonist& protagonist) {
    if (!equip_state) {
        // 装备物品
        equip_state = true;
        if (is_science) {
            protagonist.updateAttr(BasicValue::ProtagonistAttr::INTELSCI_BOOST_RATE, intel_boost_rate, true);
        }
        else {
            protagonist.updateAttr(BasicValue::ProtagonistAttr::INTELARTS_BOOST_RATE, intel_boost_rate, true);
        }
        //  : view
        ss << "\"" << name << "\"" << "已装备。";
        auto view = View::getInstance();
        view->printQuestion("",ss.str(), "white");
        ss.str("");
    }
    else {
        // 卸下物品
        equip_state = false;
        if (is_science) {
            protagonist.updateAttr(BasicValue::ProtagonistAttr::INTELSCI_BOOST_RATE, -intel_boost_rate, true);
        }
        else {
            protagonist.updateAttr(BasicValue::ProtagonistAttr::INTELARTS_BOOST_RATE, -intel_boost_rate, true);
        }
        //  view
        ss << "\"" << name << "\"" << "已取消装备。";
        auto view = View::getInstance();
        view->printQuestion("", ss.str(), "white");
        ss.str("");
    }
}

/**
 * @brief StudyAid类构造函数
 * @param name 物品名称
 * @param description 物品描述
 * @param value 物品价值
 * @param time_reduction_rate 学习时间减少比例
 * @param health_preservation_rate 健康保留比例
 */
StudyAid::StudyAid(const string& name, const string& description, float value, float time_reduction_rate, float health_preservation_rate) :
    Equippable(name, description, value), time_reduction_rate(time_reduction_rate), health_preservation_rate(health_preservation_rate) { }

/**
 * @brief 装备/卸下学习辅助工具
 * @details 调整学习时间减少率和健康保留率
 * @param protagonist 主角对象引用
 */
void StudyAid::equipAndUnequip(Protagonist& protagonist) {
    /**
    * @note  装备物品
    */
    if (!equip_state) {
        equip_state = true;
         /**
          * @note 学习消耗时间和健康，基础消耗量要乘以一个rate = 1,这里把这个rate降低至小于1。
          */
        protagonist.updateAttr(BasicValue::ProtagonistAttr::LEARNING_TIME_REDUCTION_RATE, -time_reduction_rate, true);
        protagonist.updateAttr(BasicValue::ProtagonistAttr::LEARNING_HEALTH_PRESERVATION_RATE, -health_preservation_rate, true);
        // view
        ss << "\"" << name << "\"" << "已装备。";
        auto view = View::getInstance();
        view->printQuestion("", ss.str(), "white");
        ss.str("");
    }
    /**
    * @note  卸下物品
    */
    else {
        equip_state = false;
        protagonist.updateAttr(BasicValue::ProtagonistAttr::LEARNING_TIME_REDUCTION_RATE, time_reduction_rate, true);
        protagonist.updateAttr(BasicValue::ProtagonistAttr::LEARNING_HEALTH_PRESERVATION_RATE, health_preservation_rate, true);
        // view
        ss << "\"" << name << "\"" << "已取消装备。";
        auto view = View::getInstance();
        view->printQuestion("", ss.str(), "white");
        ss.str("");
    }
}

/**
 * @brief Food类构造函数
 * @param name 物品名称
 * @param description 物品描述
 * @param value 物品价值
 * @param strength_restore 体力恢复值
 * @param health_restore 健康恢复值
 * @param time_cooldown 冷却时间
 * @param have_cd 是否有冷却时间
 */
Food::Food(const string& name, const string& description, float value, float strength_restore, float health_restore, float time_cooldown, bool have_cd) :
    Consumable(name, description, value), strength_restore(strength_restore), health_restore(health_restore), have_cd(have_cd), time_cooldown(time_cooldown) { }

/**
 * @brief 检查食品是否处于冷却状态
 * @param protagonist 主角对象引用
 * @return 是否处于冷却状态的布尔值
 */
bool Food::isOnCD(Protagonist& protagonist)const {
    if (have_cd && abs(protagonist.getGameTime() - protagonist.getHiddenAttrs()[BasicValue::ProtagonistAttr::T_USED_COMPRESSED_CRACKER]) < time_cooldown)
        return true;
    else
        return false;
}

/**
 * @brief 使用食品
 * @details 恢复主角体力和健康值，记录使用时间
 * @param protagonist 主角对象引用
 */
void Food::use(Protagonist& protagonist) {
    
    //恢复主角一定体力和健康
    protagonist.updateAttr(BasicValue::ProtagonistAttr::STRENGTH, strength_restore, true);
    protagonist.updateAttr(BasicValue::ProtagonistAttr::HEALTH, health_restore, true);

    // 记录使用时间
    protagonist.updateAttr(BasicValue::ProtagonistAttr::T_USED_COMPRESSED_CRACKER, protagonist.getGameTime(), false);

    /**
    * @note feedback
    *   view
    */
    ss << "\"" << name << "\"" << "已使用消耗品";
    auto view = View::getInstance();
    view->printQuestion("", ss.str(), "white");
    ss.str("");
}

/**
 * @brief LearningAid类构造函数
 * @param name 物品名称
 * @param description 物品描述
 * @param value 物品价值
 * @param intel_boost 智力提升值
 * @param intel_boost_rate 智力提升比例
 * @param duration 效果持续时间
 * @param have_abuse_punish 是否有滥用惩罚
 * @param health_reduce 健康减少值（滥用惩罚）
 * @param punish_cd 防止滥用的最低时间间隔
 */
LearningAid::LearningAid(const string& name, const string& description, float value, float intel_boost, float intel_boost_rate, float duration, bool have_abuse_punish, float health_reduce, int punish_cd) :
    Consumable(name, description, value), intel_boost(intel_boost), intel_boost_rate(intel_boost_rate), duration(duration), have_abuse_punish(have_abuse_punish), health_reduce(health_reduce), punish_cd(punish_cd){ }

/**
 * @brief 使用学习辅助物品
 * @details 提高主角学习效率，可能带有滥用惩罚
 * @param protagonist 主角对象引用
 */
void LearningAid::use(Protagonist& protagonist) {
      if(have_abuse_punish == false){
          if (protagonist.getHiddenAttrs()[BasicValue::ProtagonistAttr::BUFF_MILK] == false) {
              /**
                * @note 提高主角学习效率
                */
              protagonist.updateAttr(BasicValue::ProtagonistAttr::INTELARTS_BOOST, intel_boost, true);
              protagonist.updateAttr(BasicValue::ProtagonistAttr::INTELSCI_BOOST, intel_boost, true);
              protagonist.updateAttr(BasicValue::ProtagonistAttr::INTELARTS_BOOST_RATE, intel_boost_rate, true);
              protagonist.updateAttr(BasicValue::ProtagonistAttr::INTELSCI_BOOST_RATE, intel_boost_rate, true);
              
              /**
              * @note 记录buff已生效及其生效时间
              */
              protagonist.updateAttr(BasicValue::ProtagonistAttr::BUFF_MILK, true, false);
              protagonist.updateAttr(BasicValue::ProtagonistAttr::T_BUFF_MILK, protagonist.getGameTime(), false);
          } else {
              /**
              * @note 重新记录buff生效时间
              */
              protagonist.updateAttr(BasicValue::ProtagonistAttr::T_BUFF_MILK, protagonist.getGameTime(), false);
          }
      }else{
          if (abs(protagonist.getGameTime() - protagonist.getHiddenAttrs()[BasicValue::ProtagonistAttr::T_BUFF_ENERGY_DRINK]) <= punish_cd) {
              protagonist.updateAttr(BasicValue::ProtagonistAttr::HEALTH, -health_reduce, true);
          }
          if (protagonist.getHiddenAttrs()[BasicValue::ProtagonistAttr::BUFF_ENERGY_DRINK] == false) {
              /**
                * @note 提高主角学习效率
                */
              protagonist.updateAttr(BasicValue::ProtagonistAttr::INTELARTS_BOOST, intel_boost, true);
              protagonist.updateAttr(BasicValue::ProtagonistAttr::INTELSCI_BOOST, intel_boost, true);
              protagonist.updateAttr(BasicValue::ProtagonistAttr::INTELARTS_BOOST_RATE, intel_boost_rate, true);
              protagonist.updateAttr(BasicValue::ProtagonistAttr::INTELSCI_BOOST_RATE, intel_boost_rate, true);

              /**
              * @note 记录buff已生效及其生效时间
              */
              protagonist.updateAttr(BasicValue::ProtagonistAttr::BUFF_ENERGY_DRINK, true, false);
              protagonist.updateAttr(BasicValue::ProtagonistAttr::T_BUFF_ENERGY_DRINK, protagonist.getGameTime(), false);
          }
          else {
              /**
              * @note 重新记录buff生效时间
              */
              protagonist.updateAttr(BasicValue::ProtagonistAttr::T_BUFF_ENERGY_DRINK, protagonist.getGameTime(), false);
          }
    }
      /**
      * @note feedback
      *   view
      */
    ss << "\"" << name << "\"" << "已使用消耗品";
    auto view = View::getInstance();
    view->printQuestion("", ss.str(), "white");
    ss.str("");
}

/**
 * @brief HealthItem类构造函数
 * @param name 物品名称
 * @param description 物品描述
 * @param value 物品价值
 * @param health_restore 健康恢复值
 * @param duration 效果持续时间
 * @param time_cooldown 冷却时间
 * @param have_cd 是否有冷却时间
 */
HealthItem::HealthItem(const string& name, const string& description, float value, float health_restore, float duration, float time_cooldown, bool have_cd) :
    Consumable(name, description, value), health_restore(health_restore), duration(duration), have_cd(have_cd), time_cooldown(time_cooldown) { }

/**
 * @brief 检查健康物品是否处于冷却状态
 * @param protagonist 主角对象引用
 * @return 是否处于冷却状态的布尔值
 */
bool HealthItem::isOnCD(Protagonist& protagonist)const {
    if (have_cd && abs(protagonist.getGameTime() - protagonist.getHiddenAttrs()[BasicValue::ProtagonistAttr::T_USED_FIRST_AID_KIT]) < time_cooldown)
        return true;
    else
        return false;
}

/**
 * @brief 使用健康物品
 * @details 恢复健康值或提供健康保护效果
 * @param protagonist 主角对象引用
 */
void HealthItem::use(Protagonist& protagonist) {
    if (have_cd) {
        // 直接恢复健康值
        protagonist.updateAttr(BasicValue::ProtagonistAttr::HEALTH, health_restore, true);

        // 记录使用时间
        protagonist.updateAttr(BasicValue::ProtagonistAttr::T_USED_FIRST_AID_KIT, protagonist.getGameTime(), false);
    }
    else {
        // 提供健康保护效果
        protagonist.updateAttr(BasicValue::ProtagonistAttr::VITMIN_EFFECT_RATE, 0, false);

        //记录维生素buff生效及其生效时间
        protagonist.updateAttr(BasicValue::ProtagonistAttr::BUFF_VITAMINS, true, false);
        protagonist.updateAttr(BasicValue::ProtagonistAttr::T_BUFF_VITAMINS, protagonist.getGameTime(), false);
    }
    //  view
    ss << "\"" << name << "\"" << "已使用消耗品";
    auto view = View::getInstance();
    view->printQuestion("", ss.str(), "white");
    ss.str("");
}

/**
 * @brief ItemCreator类构造函数
 * @details 初始化物品配置文件的读取
 */

 /**
  * @brief 初始化json文件读取
  * @param file_name 包含所有物品参数信息的json文件的名字（字符串）
  */
ItemCreator::ItemCreator() {
    // 更新配置文件路径
    std::filesystem::path file_path(ROOT_DIR);
    file_path = file_path / ".config/Item.json";

    config_file_item.open(file_path);
    if (!config_file_item.is_open()) {
        Controller::getInstance()->log(Controller::LogLevel::ERR, "无法打开物品配置文件");
        Controller::getInstance()->gameExit();
    }
    config_file_item >> config_item;
    config_file_item.close();
}

/**
 * @brief 获取物品基础信息
 * @param item_name 物品唯一标识符
 * */
ItemBasicInf ItemCreator::getItemInf(string& item_name)const {
    ItemBasicInf item_inf(config_item[item_name]["name"], config_item[item_name]["description"], config_item[item_name]["value"]);
    return item_inf;
}

/**
 * @brief 创建物品对象
 * @param item_name 物品名称
 * @return 物品对象的shared_ptr
 */
std::shared_ptr<Item> ItemCreator::createItem(string &item_name)
{
    // 读取该物品所属类，并将string类型的类名转化为enum类型
    ItemType item_class = getItemType((string)config_item[item_name]["class"]);

    //根据物品所属类获取物品参数，动态创建对象
    switch (item_class) {
    case ItemType::STUDY_MATERIAL: {
        //读取物品参数
        string name = (string)config_item[item_name]["name"];
        string description = (string)config_item[item_name]["description"];
        float value = (float)config_item[item_name]["value"];
        bool is_science = (bool)config_item[item_name]["is_science"];
        float intel_boost_rate = (float)config_item[item_name]["intel_boost_rate"];

        //创建物品对象并返回
        return make_shared<StudyMaterial>(name, description, value, is_science, intel_boost_rate);

        break;
    }
    case ItemType::STUDY_AID: {
        //读取物品参数
        string name = (string)config_item[item_name]["name"];
        string description = (string)config_item[item_name]["description"];
        float value = (float)config_item[item_name]["value"];
        float time_reduction_rate = (float)config_item[item_name]["time_reduction_rate"];
        float health_preservation_rate = (float)config_item[item_name]["health_preservation_rate"];

        //创建物品对象并返回
        return make_shared<StudyAid>(name, description, value, time_reduction_rate, health_preservation_rate);

         break;
    }
    case ItemType::FOOD: {
        //读取物品参数
        string name = (string)config_item[item_name]["name"];
        string description = (string)config_item[item_name]["description"];
        float value = (float)config_item[item_name]["value"];
        float strength_restore = (float)config_item[item_name]["strength_restore"];
        float health_restore = (float)config_item[item_name]["health_restore"];
        float time_cooldown = (float)config_item[item_name]["time_cooldown"];
        bool have_cd = (bool)config_item[item_name]["have_cd"];

        //创建物品对象并返回
        return make_shared<Food>(name, description, value, strength_restore, health_restore, time_cooldown, have_cd);

        break;
    }
    case ItemType::LEARNING_AID: {
        //读取物品参数
        string name = (string)config_item[item_name]["name"];
        string description = (string)config_item[item_name]["description"];
        float value = (float)config_item[item_name]["value"];
        float intel_boost = (float)config_item[item_name]["intel_boost"];
        float intel_boost_rate = (float)config_item[item_name]["intel_boost_rate"];
        float duration = (float)config_item[item_name]["duration"];
        float health_reduce = (float)config_item[item_name]["health_reduce"];
        int punish_cd = (int)config_item[item_name]["punish_cd"];
        bool have_abuse_punish = (bool)config_item[item_name]["have_abuse_punish"];

        //创建物品对象并返回
        return make_shared<LearningAid>(name, description, value, intel_boost, intel_boost_rate, duration, have_abuse_punish, health_reduce, punish_cd);

        break;
    }
    case ItemType::HEALTH_ITEM: {
        //读取物品参数
        string name = (string)config_item[item_name]["name"];
        string description = (string)config_item[item_name]["description"];
        float value = (float)config_item[item_name]["value"];
        float health_restore = (float)config_item[item_name]["health_restore"];
        float duration = (float)config_item[item_name]["duration"];
        float time_cooldown = (float)config_item[item_name]["time_cooldown"];
        bool have_cd = (bool)config_item[item_name]["have_cd"];

        //创建物品对象并返回
        return make_shared<HealthItem>(name, description, value, health_restore, duration, time_cooldown, have_cd);

        break;
    }
    default:
        Controller::getInstance()->log(Controller::LogLevel::ERR, "Error:Item.cpp 找不到目标物品细分类");
        Controller::getInstance()->gameExit();
        break;
    }
    return nullptr;
}

/**
 * @brief 清理主角身上的某个buff
 * @param buff_name buff对应键值
 * @return 操作结果：status=0（成功）/-1（数据无效/格式错误）；msg=结果描述
 */
Message ItemCreator::clearBuff(BasicValue::Buff buff_name, Protagonist& protagonist) {
    switch (buff_name) {
    case BasicValue::Buff::BUFF_ENERGY_DRINK: {
        float intel_boost = config_item["energy_drink"]["intel_boost"];
        float intel_boost_rate = config_item["energy_drink"]["intel_boost_rate"];
        protagonist.updateAttr(BasicValue::ProtagonistAttr::INTELARTS_BOOST, -intel_boost, true);
        protagonist.updateAttr(BasicValue::ProtagonistAttr::INTELSCI_BOOST, -intel_boost, true);
        protagonist.updateAttr(BasicValue::ProtagonistAttr::INTELARTS_BOOST_RATE, -intel_boost_rate, true);
        protagonist.updateAttr(BasicValue::ProtagonistAttr::INTELSCI_BOOST_RATE, -intel_boost_rate, true);
        protagonist.updateAttr(BasicValue::ProtagonistAttr::BUFF_ENERGY_DRINK, false, false);
        return Message("能量饮料buff已解除", 0);
    }
    case BasicValue::Buff::BUFF_MILK: {
        float intel_boost = config_item["milk"]["intel_boost"];
        float intel_boost_rate = config_item["milk"]["intel_boost_rate"];
        protagonist.updateAttr(BasicValue::ProtagonistAttr::INTELARTS_BOOST, -intel_boost, true);
        protagonist.updateAttr(BasicValue::ProtagonistAttr::INTELSCI_BOOST, -intel_boost, true);
        protagonist.updateAttr(BasicValue::ProtagonistAttr::INTELARTS_BOOST_RATE, -intel_boost_rate, true);
        protagonist.updateAttr(BasicValue::ProtagonistAttr::INTELSCI_BOOST_RATE, -intel_boost_rate, true);
        protagonist.updateAttr(BasicValue::ProtagonistAttr::BUFF_MILK, false, false);
        return Message("牛奶buff已解除", 0);
    }
    case BasicValue::Buff::BUFF_VITAMINS: {
        protagonist.updateAttr(BasicValue::ProtagonistAttr::VITMIN_EFFECT_RATE, 1, false);
        protagonist.updateAttr(BasicValue::ProtagonistAttr::BUFF_VITAMINS, false, false);
        return Message("维生素buff已解除", 0);
    }
    default:
        return Message("错误的buff键值", -1);
    }
}

Message ItemCreator::updateBuff(Protagonist& protagonist) {
    std::vector<BasicValue::Buff> buff(3);
    std::vector<int> duration(3), time(3);
    buff[0] = BasicValue::Buff::BUFF_ENERGY_DRINK;
    buff[1] = BasicValue::Buff::BUFF_MILK;
    buff[2] = BasicValue::Buff::BUFF_VITAMINS;
    duration[0] = config_item["energy_drink"]["duration"];
    duration[1] = config_item["milk"]["duration"];
    duration[2] = config_item["vitamins"]["duration"];
    time[0] = protagonist.getHiddenAttrs()[BasicValue::ProtagonistAttr::T_BUFF_ENERGY_DRINK];
    time[1] = protagonist.getHiddenAttrs()[BasicValue::ProtagonistAttr::T_BUFF_MILK];
    time[2] = protagonist.getHiddenAttrs()[BasicValue::ProtagonistAttr::T_BUFF_VITAMINS];
    for (int i = 0; i < 3; i++) {
        if (abs(protagonist.getGameTime() - time[i]) > duration[i]) {
            clearBuff(buff[i],protagonist);
        }
    }
    return Message("buff已更新");
}
