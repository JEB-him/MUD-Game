 /**
  * @file Item.cpp
  * @author Xiaotian Wu(Cheongfan)
  * @brief 物品类及其派生类的实现方法
  * @note 测试说明： \n
  *          由于本文件的类的测试需要背包系统，且一些物品涉及时间系统、健康值系统等未实现的逻辑， \n
  *          其具体的方法难以书写。因此尚未做检验测试。 \n
  * @note 未完成说明： \n 
  *          本文件未完成部分用"TODO:"进行了标识。未完成内容：物品的使用/装备方法 \n
  *          具体来说： \n 
  *          消耗品的use方法未实现、用具equipAndUnequip方法未适配主角类接口的改变 \n
  * */

#pragma once
#include"Item.h"
#include<filesystem>

  /**
   * @brief 六个细分种类的枚举类（学习资料、运动器材、学习辅助工具、食品、学习辅助食品、健康类物品）
   */
 enum class ItemType {
     study_material,
     study_aid,
     food,
     learning_aid,
     health_item,
     unknown
 };

 /**
  * @brief 字符串到枚举的转换函数
  * @param item_class 物品所属细分类别(string类型)
  * @return
  */
 ItemType getItemType(const string& item_class) {
     static const map<string, ItemType> stringToEnum = {
         {"study_material", ItemType::study_material},
         {"study_aid", ItemType::study_aid},
         {"food", ItemType::food},
         {"learning_aid", ItemType::learning_aid},
         {"health_item", ItemType::health_item},
     };
     auto it = stringToEnum.find(item_class);
     if (it != stringToEnum.end()) {
         return it->second;
     }
     return ItemType::unknown;
 }

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
 * @brief Item类的使用虚函数、装备/卸下虚函数，抛出异常
 * TODO 抛出异常
 */
void Item::use(Protagonist& protagonist){ }
void Item::equipAndUnequip(Protagonist& protagonist){ }

 /**
 * @brief Consumable类构造函数
 */
 Consumable::Consumable(const string& name, const string& description, float value) :
     Item(name, description, value, true) { }

 /**
 * @brief Equippable类构造函数
 */
 Equippable::Equippable(const string& name, const string& description, float value) :
     Item(name, description, value, false), equip_state(false) { }

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
  void StudyMaterial::equipAndUnequip(Protagonist& protagonist) {

      ///**
      //* @brief 装备前判断是否未装备
      //*/
      //if (!equip_state) {

      //    /**
      //    * @brief 装备状态：已装备
      //    */
      //    equip_state = true;

      //    /**
      //    * @brief 根据学习资料的文理类型，提高对应的智力比例增量
      //    * TODO: 主角类接口发生了修改，未作适配
      //    */
      //    if (is_science) {
      //        protagonist.resetIntelSciBoostRate(protagonist.getIntelSciBoostRate() + intel_boost_rate);
      //    }
      //    else {
      //        protagonist.resetIntelArtsBoostRate(protagonist.getIntelArtsBoostRate() + intel_boost_rate);
      //    }

      //    /**
      //    * @brief feedback，后续可用View的方法替换
      //    */
      //    cout << "\"" << name << "\"" << "已装备。" << endl;
      //}
      //else {
      //    /**
      //    * @brief 与上一个scope相反
      //    * TODO: 主角类接口发生了修改，未作适配
      //    */
      //    equip_state = false;
      //    if (is_science) {
      //        protagonist.resetIntelSciBoostRate(protagonist.getIntelSciBoostRate() - intel_boost_rate);
      //    }
      //    else {
      //        protagonist.resetIntelArtsBoostRate(protagonist.getIntelArtsBoostRate() - intel_boost_rate);
      //    }
      //    cout << "\"" << name << "\"" << "已取消装备。" << endl;
      //}
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
  void StudyAid::equipAndUnequip(Protagonist& protagonist) {
      //if (!equip_state) {

      //    /**
      //    * @brief 装备状态：已装备
      //    */
      //    equip_state = true;

      //    /**
      //    * @brief 学习消耗时间和健康，基础消耗量要乘以一个rate = 1,这里把这个rate降低至小于1。
      //    * TODO: 主角类接口发生了修改，未作适配
      //    */
      //    protagonist.resetLearningTimeReductionRate(protagonist.getLearningTimeReductionRate() - time_reduction_rate);
      //    protagonist.resetLearningHealthPreservationRate(protagonist.getLearningHealthPreservationRate() - health_preservation_rate);

      //    /**
      //    * @brief feedback，后续可用View的方法替换
      //    */
      //    cout << "\"" << name << "\"" << "已装备。" << endl;
      //}
      //else {

      //    /**
      //    * @brief 与上一个scope相反 
      //    * TODO: 主角类接口发生了修改，未作适配
      //    */
      //    equip_state = false;

      //    protagonist.resetLearningTimeReductionRate(protagonist.getLearningTimeReductionRate() + time_reduction_rate);
      //    protagonist.resetLearningHealthPreservationRate(protagonist.getLearningHealthPreservationRate() + health_preservation_rate);

      //    cout << "\"" << name << "\"" << "已取消装备。" << endl;
      //}
  }


 /**
 * @brief 食品类构造函数
 */
 Food::Food(const string& name, const string& description, float value, float strength_restore, float health_restore, float time_cooldown, float time_last_used) :
     Consumable(name, description, value), strength_restore(strength_restore), health_restore(health_restore), time_cooldown(time_cooldown), time_last_used(time_last_used) { }

 /*TODO*/
 bool Food::isOnCooldown()const {
     return true;
 }
 /**
 * @brief 使用“食品类物品”
 * @note  在外部调用item.use接口使用了消耗品后，记得将该消耗品对象释放
 * TODO: 这里需要获取当前时间的接口now(),需要等时间(计时)的具体逻辑出来后再具体修改
 */
  void Food::use(Protagonist& protagonist) {

      /**
      * @brief 等CD
      */
      //if (/*now()*/ -time_last_used > time_cooldown) {

      //    /**
      //    * @brief 恢复主角一定体力和健康，建议reset接口自行判断是否超出MAX_STRENGTH和MAX_HEALTH
      //    */
      //    protagonist.resetStrength(protagonist.getStrength() + strength_restore);
      //    protagonist.resetHealth(protagonist.getHealth() + health_restore);

      //    cout << "\"" << name << "\"" << "已使用消耗品" << endl;
      //}
      //else {
      //    cout << "冷却中,暂时无法使用该道具" << endl;
      //}
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
  void LearningAid::use(Protagonist& protagonist) {

      /**
      * TODO:此处需要设置定时器，在一定时间(duration)后增益解除(执行解除增益的函数)
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
  void HealthItem::use(Protagonist& protagonist) {
     // protagonist.resetHealth(protagonist.getHealth() + health_restore);
      /**
      * TODO:此处需要暂停健康值损耗一段时间，具体实现需要考虑健康值损耗逻辑和时间（计时）逻辑 \n
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
 ItemCreator::ItemCreator(string file_name) {
     // TODO need update
     std::filesystem::path file_path(ROOT_DIR);
     file_path = file_path / file_name;

     config_file_item.open(file_path);
     if (!config_file_item.is_open()) {
         throw std::runtime_error("无法打开物品配置文件");
     }
     config_file_item >> config_item;
     config_file_item.close();
 }

 /**
  * @brief 创建物品对象
  * @param item_name 
  * @return unique智能指针
  */
 unique_ptr<Item> ItemCreator::createItem(string item_name) {
     /**
     * @note 读取该物品所属类，并将string类型的类名转化为enum类型
     */
     item_name;
     ItemType item_class = getItemType((string)config_item[item_name]["class"]);

     /**
     * @note 根据物品所属类获取物品参数，动态创建对象
     */
     switch (item_class) {
     case ItemType::study_material: {
         /**
         * @brief 读取物品参数
         */
         string name = (string)config_item[item_name]["name"];
         string description = (string)config_item[item_name]["description"];
         float value = (float)config_item[item_name]["value"];
         bool is_science = (bool)config_item[item_name]["is_science"];
         float intel_boost_rate = (float)config_item[item_name]["intel_boost_rate"];

         /**
         * @brief 创建物品对象并返回
         */
         unique_ptr<Item> item_ptr = make_unique<StudyMaterial>(name,description, value, is_science, intel_boost_rate);
         return item_ptr;

         break;
     }
     case ItemType::study_aid: {
         /**
         * @brief 读取物品参数
         */
         string name = (string)config_item[item_name]["name"];
         string description = (string)config_item[item_name]["description"];
         float value = (float)config_item[item_name]["value"];
         float time_reduction_rate = (float)config_item[item_name]["time_reduction_rate"];
         float health_preservation_rate = (float)config_item[item_name]["health_preservation_rate"];

         /**
         * @brief 创建物品对象并返回
         */
         unique_ptr<Item> item_ptr = make_unique<StudyAid>(name,description, value, time_reduction_rate, health_preservation_rate);
         return item_ptr;

         break;
     }
     case ItemType::food: {
         /**
         * @brief 读取物品参数
         */
         string name = (string)config_item[item_name]["name"];
         string description = (string)config_item[item_name]["description"];
         float value = (float)config_item[item_name]["value"];
         float strength_restore = (float)config_item[item_name]["strength_restore"];
         float health_restore = (float)config_item[item_name]["health_restore"];
         float time_cooldown = (float)config_item[item_name]["time_cooldown"];
         float time_last_used = (float)config_item[item_name]["time_last_used"];

         /**
         * @brief 创建物品对象并返回
         */
         unique_ptr<Item> item_ptr = make_unique<Food>(name, description, value, strength_restore, health_restore, time_cooldown, time_last_used);
         return item_ptr;

         break;
     }
     case ItemType::learning_aid: {
         /**
         * @brief 读取物品参数
         */
         string name = (string)config_item[item_name]["name"];
         string description = (string)config_item[item_name]["description"];
         float value = (float)config_item[item_name]["value"];
         float intel_boost = (float)config_item[item_name]["intel_boost"];
         float intel_boost_rate = (float)config_item[item_name]["intel_boost_rate"];
         float duration = (float)config_item[item_name]["duration"];
         bool have_abuse_punish = (bool)config_item[item_name]["have_abuse_punish"];
         bool is_used_today = (bool)config_item[item_name]["is_used_today"];
         float health_reduce = (float)config_item[item_name]["health_reduce"];

         /**
         * @brief 创建物品对象并返回
         */
         unique_ptr<Item> item_ptr = make_unique<LearningAid>(name,description, value, intel_boost, intel_boost_rate, duration, have_abuse_punish, is_used_today, health_reduce);
         return item_ptr;

         break;
     }
     case ItemType::health_item: {
         /**
         * @brief 读取物品参数
         */
         string name = (string)config_item[item_name]["name"];
         string description = (string)config_item[item_name]["description"];
         float value = (float)config_item[item_name]["value"];
         float health_restore = (float)config_item[item_name]["health_restore"];
         float duration = (float)config_item[item_name]["duration"];

         /**
         * @brief 创建物品对象并返回
         */
         unique_ptr<Item> item_ptr = make_unique<HealthItem>(name,description, value, health_restore, duration);
         return item_ptr;

         break;
     }
     default:
         break;
     }
 }
