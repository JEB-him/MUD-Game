/**
* * @file backpack‌.h
* @author Xiaotian Wu(Cheongfan)
* @brief 背包类实现
*/

#include"backpack.h"
#include "Controller.h"

/**
 * @brief 背包类构造函数
 * @note item_creator默认打开存储物品数据的json文件
 * @note 初始容量设定为0（，vector的push方法自动扩容）
 */
Backpack::Backpack():item_creator(), backpack_items(0){ }

/**
 * @brief 获取背包管理物品智能指针的vector数组
 * @return vector数组的引用
 */
vector<std::shared_ptr<Item>>& Backpack::getBackpackItems() {
    return backpack_items;
}

/**
 * @brief  获取物品的基础信息（名字、描述、价值）
 * @param item_name 一个物品的特异标识符，如"advanced_mathematics"。
 */
ItemBasicInf Backpack::getItemInf(string& item_name)const {
    return item_creator.getItemInf(item_name);
}

/**
 * @brief  添加（生成）名为“item_name”的物品
 * @param item_name 物品的名称
 * @details 逻辑：利用ItemCreator的方法创造目标物品对象，存入backpack_items中
 */
void Backpack::addItem(string item_name) {
    backpack_items.push_back(item_creator.createItem(item_name));
}
 
/**
 * @brief 使用或装备或卸下物品(三合一功能函数)
 * @param order 操作的物品的序号(从1开始)
 * @param protagonist 传入主角对象名
 */
void Backpack::useFunctionOfItem(int order, Protagonist& protagonist) {
    //排除小于1和大于最大物品序号的输入值
    Controller::getInstance()->log(Controller::LogLevel::INFO, "进入使用");
    if (order > backpack_items.size() || order < 1) {
        throw std::runtime_error("输入了无效序号。");
    }
    else{
        Controller::getInstance()->log(Controller::LogLevel::INFO, "序号有效");
        int index = order - 1;
            //消耗品类
            if (backpack_items[index].get()->getIsConsumable()) {
                //冷却判定
                Controller::getInstance()->log(Controller::LogLevel::INFO, "是消耗品");
                if (backpack_items[index].get()->isOnCD(protagonist)) {
                    /**
                    * @note feedback    TODO view
                    */
                    ss << "冷却中,暂时无法使用该道具";
                    //view->gameoutput(ss.str());
                    ss.str("");
                } else {
                    /**
                    * @note feedback    TODO view
                    */
                    Controller::getInstance()->log(Controller::LogLevel::INFO, "不在CD");
                    ss << "消耗品" << "\"" << backpack_items[index].get()->getName() << "\"" << "已被使用。";
                    //view->gameoutput(ss.str());
                    ss.str("");

                    //使用该消耗品
                    backpack_items[index].get()->use(protagonist);
                    backpack_items[index].reset();//释放消耗品对象
                    backpack_items.erase(backpack_items.begin() + index);//从背包的智能指针vector中删除
                }
            }
            //用具类
            else {
                /**
                * @note feedback    TODO view
                */
                ss << "用具" << "\"" << backpack_items[index].get()->getName() << "\"" << "已装备。";
                //view->gameoutput(ss.str());
                ss.str("");

                //装备该用具
                backpack_items[index].get()->equipAndUnequip(protagonist);
                Controller::getInstance()->log(Controller::LogLevel::INFO, "装备成功");
            }
    }
}

/**
 * @brief 获取item_creator的引用
 */
ItemCreator& Backpack::getItemCreator() {
    return item_creator;
}