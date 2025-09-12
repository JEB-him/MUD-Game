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
 */
 bool Item::isOnCD(Protagonist& protagonist)const {
     return false;
}
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
      /**
         * @note 装备前判断是否未装备
         */
      if (!equip_state) {

          /**
          * @note 装备状态：已装备
          */
          equip_state = true;

          /**
          * @note 根据学习资料的文理类型，提高对应的智力比例增量
          */
          if (is_science) {
              protagonist.updateAttr(BasicValue::ProtagonistAttr::INTELSCI_BOOST_RATE, intel_boost_rate, true);
          }
          else {
              protagonist.updateAttr(BasicValue::ProtagonistAttr::INTELARTS_BOOST_RATE, intel_boost_rate, true);
          }

          /**
          * @note feedback
          * TODO view
          */
          ss << "\"" << name << "\"" << "已装备。";
          //view->gameoutput(ss.str());
          ss.str("");
      }
      else {

          /**
          * @note 与上一个scope相反
          */

          equip_state = false;
          if (is_science) {
              protagonist.updateAttr(BasicValue::ProtagonistAttr::INTELSCI_BOOST_RATE, -intel_boost_rate, true);
          }
          else {
              protagonist.updateAttr(BasicValue::ProtagonistAttr::INTELARTS_BOOST_RATE, -intel_boost_rate, true);
          }

          /**
          * @note feedback
          * TODO view
          */
          ss << "\"" << name << "\"" << "已取消装备。";
          //view->gameoutput(ss.str());
          ss.str("");
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
  void StudyAid::equipAndUnequip(Protagonist& protagonist) {
      if (!equip_state) {

          /**
          * @note 装备状态：已装备
          */
          equip_state = true;

          /**
          * @note 学习消耗时间和健康，基础消耗量要乘以一个rate = 1,这里把这个rate降低至小于1。
          * TODO: 主角类接口发生了修改，未作适配
          */
          protagonist.updateAttr(BasicValue::ProtagonistAttr::LEARNING_TIME_REDUCTION_RATE, -time_reduction_rate, true);
          protagonist.updateAttr(BasicValue::ProtagonistAttr::LEARNING_HEALTH_PRESERVATION_RATE, -health_preservation_rate, true);

          /**
          * @note feedback
          * TODO view
          */
          ss << "\"" << name << "\"" << "已装备。";
          //view->gameoutput(ss.str());
          ss.str("");
      }
      else {

          /**
          * @note 与上一个scope相反 
          * TODO: 主角类接口发生了修改，未作适配
          */

          equip_state = false;

          protagonist.updateAttr(BasicValue::ProtagonistAttr::LEARNING_TIME_REDUCTION_RATE, time_reduction_rate, true);
          protagonist.updateAttr(BasicValue::ProtagonistAttr::LEARNING_HEALTH_PRESERVATION_RATE, health_preservation_rate, true);

          /**
          * @note feedback
          * TODO view
          */
          ss << "\"" << name << "\"" << "已取消装备。";
          //view->gameoutput(ss.str());
          ss.str("");
      }
  }


 /**
 * @brief 食品类构造函数
 */
 Food::Food(const string& name, const string& description, float value, float strength_restore, float health_restore, float time_cooldown, bool have_cd) :
     Consumable(name, description, value), strength_restore(strength_restore), health_restore(health_restore), have_cd(have_cd), time_cooldown(time_cooldown) { }

 bool Food::isOnCD(Protagonist& protagonist)const {
     //if (have_cd && /*now()*/ - protagonist.getHiddenAttrs()[BasicValue::ProtagonistAttr::TUsedCompressedCracker] > time_cooldown)
     //    return true;
     //else
         return false;
 }
 /**
 * @brief 使用“食品类物品”
 * @note  在外部调用item.use接口使用了消耗品后，记得将该消耗品对象释放
 * TODO: 当前为阉割版，物品使用无cd。
 */
  void Food::use(Protagonist& protagonist) {
          /**
          * @note 恢复主角一定体力和健康
          */
          protagonist.updateAttr(BasicValue::ProtagonistAttr::STRENGTH, strength_restore, true);
          protagonist.updateAttr(BasicValue::ProtagonistAttr::HEALTH, health_restore, true);

          /**
          * @note 记录上次使用时间(计算CD) TODO now()
          */
          //protagonist.updateAttr(BasicValue::ProtagonistAttr::TUsedCompressedCracker, /*now()*/, false);

          /**
          * @note feedback
          * TODO view
          */
          ss << "\"" << name << "\"" << "已使用消耗品";
          //view->gameoutput(ss.str());
          ss.str("");
  }

 /**
 * @brief 学习辅助类构造函数
 */
 LearningAid::LearningAid(const string& name, const string& description, float value, float intel_boost, float intel_boost_rate, float duration, bool have_abuse_punish, float health_reduce) :
     Consumable(name, description, value), intel_boost(intel_boost), intel_boost_rate(intel_boost_rate), duration(duration), have_abuse_punish(have_abuse_punish), health_reduce(health_reduce) { }

 /**
 * @brief 使用“学习辅助类物品”：
 * @note  提高主角学习效率
 * TODO: 当前为阉割版，持续时间永久，无滥用惩罚。
 */
  void LearningAid::use(Protagonist& protagonist) {
      if(have_abuse_punish == false){
          //if (protagonist.getHiddenAttrs()[BasicValue::ProtagonistAttr::BuffMilk] == false) {
          //    /**
          //      * @note 提高主角学习效率
          //      */
          //    protagonist.updateAttr(BasicValue::ProtagonistAttr::INTELARTS_BOOST, intel_boost, true);
          //    protagonist.updateAttr(BasicValue::ProtagonistAttr::INTELSCI_BOOST, intel_boost, true);
          //    protagonist.updateAttr(BasicValue::ProtagonistAttr::INTELARTS_BOOST_RATE, intel_boost_rate, true);
          //    protagonist.updateAttr(BasicValue::ProtagonistAttr::INTELSCI_BOOST_RATE, intel_boost_rate, true);
          //    
          //    /**
          //    * @note 记录buff已生效及其生效时间
          //    */
          //    protagonist.updateAttr(BasicValue::ProtagonistAttr::BuffMilk, true, false);
          //    protagonist.updateAttr(BasicValue::ProtagonistAttr::TBuffMilk, /*now()*/, false);
          //} else {
          //    /**
          //    * @note 重新记录buff生效时间
          //    */
          //    protagonist.updateAttr(BasicValue::ProtagonistAttr::TBuffMilk, /*now()*/, false);
          //}
      }else{
          //if (/*now*/ -protagonist.getHiddenAttrs()[BasicValue::ProtagonistAttr::TBuffEnergyDrink] <= duration) {
          //    protagonist.updateAttr(BasicValue::ProtagonistAttr::HEALTH, -health_reduce, true);
          //}
          //if (protagonist.getHiddenAttrs()[BasicValue::ProtagonistAttr::BuffEnergyDrink] == false) {
          //    /**
          //      * @note 提高主角学习效率
          //      */
          //    protagonist.updateAttr(BasicValue::ProtagonistAttr::INTELARTS_BOOST, intel_boost, true);
          //    protagonist.updateAttr(BasicValue::ProtagonistAttr::INTELSCI_BOOST, intel_boost, true);
          //    protagonist.updateAttr(BasicValue::ProtagonistAttr::INTELARTS_BOOST_RATE, intel_boost_rate, true);
          //    protagonist.updateAttr(BasicValue::ProtagonistAttr::INTELSCI_BOOST_RATE, intel_boost_rate, true);

          //    /**
          //    * @note 记录buff已生效及其生效时间
          //    */
          //    protagonist.updateAttr(BasicValue::ProtagonistAttr::BuffEnergyDrink, true, false);
          //    protagonist.updateAttr(BasicValue::ProtagonistAttr::TBuffEnergyDrink, /*now()*/, false);
          //}
          //else {
          //    /**
          //    * @note 重新记录buff生效时间
          //    */
          //    protagonist.updateAttr(BasicValue::ProtagonistAttr::TBuffEnergyDrink, /*now()*/, false);
          //}
      }
      /**
      * @note feedback
      * TODO view
      */
      ss << "\"" << name << "\"" << "已使用消耗品";
      //view->gameoutput(ss.str());
      ss.str("");
  }

 /**
 * @brief 健康类构造函数
 */
 HealthItem::HealthItem(const string& name, const string& description, float value, float health_restore, float duration, float time_cooldown, bool have_cd) :
     Consumable(name, description, value), health_restore(health_restore), duration(duration), have_cd(have_cd), time_cooldown(time_cooldown){ }

 bool HealthItem::isOnCD(Protagonist& protagonist)const {
     //if (have_cd && /*now()*/ - protagonist.getHiddenAttrs()[BasicValue::ProtagonistAttr::TUsedFirstAidKit] > time_cooldown)
     //    return true;
     //else
         return false;
 }

 /**
 * @brief 使用“健康类物品”
 * @note  提高主角学习效率
 * TODO: 当前为阉割版，持续时间永久，无CD。
 */
  void HealthItem::use(Protagonist& protagonist) {
      if (have_cd) {
          /**
          * @note 获得一些健康
          */
          protagonist.updateAttr(BasicValue::ProtagonistAttr::HEALTH, health_restore, true);

          /**
          * @note 记录上次使用的时间(计算CD)
          */
          //protagonist.updateAttr(BasicValue::ProtagonistAttr::TUsedFirstAidKit, /*now()*/, false);
      } else {
          /**
          * @note 维生素buff，一段时间内健康值不下降
          */
          //protagonist.updateAttr(BasicValue::ProtagonistAttr::VitminEffectRate, 0, false);

          /**
          * @note 记录维生素buff生效及其生效时间
          */
          //protagonist.updateAttr(BasicValue::ProtagonistAttr::BuffVitamins, true, false);
          //protagonist.updateAttr(BasicValue::ProtagonistAttr::TBuffVitamins, /*now()*/, false);

      }

      /**
      * @note feedback
      * TODO view
      */
      ss << "\"" << name << "\"" << "已使用消耗品";
      //view->gameoutput(ss.str());
      ss.str("");
  }

 /**
 * @brief 物品管理器类
 */

 /**
  * @brief 初始化json文件读取
  * @param file_name 包含所有物品参数信息的json文件的名字（字符串）
  */
 ItemCreator::ItemCreator() {
     // TODO need update
     std::filesystem::path file_path(ROOT_DIR);
     file_path = file_path / ".config/Item.json";

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
         * @note 读取物品参数
         */
         string name = (string)config_item[item_name]["name"];
         string description = (string)config_item[item_name]["description"];
         float value = (float)config_item[item_name]["value"];
         bool is_science = (bool)config_item[item_name]["is_science"];
         float intel_boost_rate = (float)config_item[item_name]["intel_boost_rate"];

         /**
         * @note 创建物品对象并返回
         */
         unique_ptr<Item> item_ptr = make_unique<StudyMaterial>(name,description, value, is_science, intel_boost_rate);
         return item_ptr;

         break;
     }
     case ItemType::study_aid: {
         /**
         * @note 读取物品参数
         */
         string name = (string)config_item[item_name]["name"];
         string description = (string)config_item[item_name]["description"];
         float value = (float)config_item[item_name]["value"];
         float time_reduction_rate = (float)config_item[item_name]["time_reduction_rate"];
         float health_preservation_rate = (float)config_item[item_name]["health_preservation_rate"];

         /**
         * @note 创建物品对象并返回
         */
         unique_ptr<Item> item_ptr = make_unique<StudyAid>(name,description, value, time_reduction_rate, health_preservation_rate);
         return item_ptr;

         break;
     }
     case ItemType::food: {
         /**
         * @note 读取物品参数
         */
         string name = (string)config_item[item_name]["name"];
         string description = (string)config_item[item_name]["description"];
         float value = (float)config_item[item_name]["value"];
         float strength_restore = (float)config_item[item_name]["strength_restore"];
         float health_restore = (float)config_item[item_name]["health_restore"];
         float time_cooldown = (float)config_item[item_name]["time_cooldown"];
         bool have_cd = (bool)config_item[item_name]["have_cd"];

         /**
         * @note 创建物品对象并返回
         */
         unique_ptr<Item> item_ptr = make_unique<Food>(name, description, value, strength_restore, health_restore, time_cooldown, have_cd);
         return item_ptr;

         break;
     }
     case ItemType::learning_aid: {
         /**
         * @note 读取物品参数
         */
         string name = (string)config_item[item_name]["name"];
         string description = (string)config_item[item_name]["description"];
         float value = (float)config_item[item_name]["value"];
         float intel_boost = (float)config_item[item_name]["intel_boost"];
         float intel_boost_rate = (float)config_item[item_name]["intel_boost_rate"];
         float duration = (float)config_item[item_name]["duration"];
         float health_reduce = (float)config_item[item_name]["health_reduce"];
         bool have_abuse_punish = (bool)config_item[item_name]["have_abuse_punish"];

         /**
         * @note 创建物品对象并返回
         */
         unique_ptr<Item> item_ptr = make_unique<LearningAid>(name,description, value, intel_boost, intel_boost_rate, duration, health_reduce, have_abuse_punish);
         return item_ptr;

         break;
     }
     case ItemType::health_item: {
         /**
         * @note 读取物品参数
         */
         string name = (string)config_item[item_name]["name"];
         string description = (string)config_item[item_name]["description"];
         float value = (float)config_item[item_name]["value"];
         float health_restore = (float)config_item[item_name]["health_restore"];
         float duration = (float)config_item[item_name]["duration"];
         float time_cooldown = (float)config_item[item_name]["time_cooldown"];
         bool have_cd = (bool)config_item[item_name]["have_cd"];

         /**
         * @note 创建物品对象并返回
         */
         unique_ptr<Item> item_ptr = make_unique<HealthItem>(name, description, value, health_restore, duration, time_cooldown, have_cd);
         return item_ptr;

         break;
     }
     default:
         break;
     }
 }
