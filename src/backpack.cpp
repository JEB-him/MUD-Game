/**
* * @file backpack‌.h
* @author Xiaotian Wu(Cheongfan)
* @brief 背包类实现
*/

#pragma once
#include"backpack.h"

/**
 * @brief 背包类构造函数
 * @note item_creator默认打开存储物品数据的json文件
 * @note 初始容量设定为10（，vector的push方法自动扩容）
 */
Backpack::Backpack():item_creator(".config/Item.json"), backpack_items(10){ }

/**
 * @brief 获取背包管理物品智能指针的vector数组
 * @return vector数组的引用
 */
vector<unique_ptr<Item>>& Backpack::getBackpack‌Items() {
    return backpack_items;
}

/**
 * @brief  添加（生成）名为“item_name”的物品
 * @param item_name 物品的名称
 * @details 逻辑：利用ItemCreator的方法创造目标物品对象，存入backpack_items中
 */
void Backpack::addItem(string item_name) {
    cout<< item_name <<endl;
    backpack_items.push_back(item_creator.createItem(item_name));
}
 
/**
 * @brief 使用或装备或卸下物品(三合一功能函数)
 * @param order 操作的物品的序号(从1开始)
 * @param protagonist 传入主角对象名
 * @details 逻辑：区分好物品类型和装备状态，然后调用Item正确的接口(use、equipAndUnequip二选一)
 * @details 逻辑：消耗品在调用此方法后，释放指针，然后序号在此物品之后的物品指针均前移一个位置
 */
void Backpack::useFunctionOfItem(int order, Protagonist& protagonist) {
    if (order > backpack_items.size() || order < 1) {
        throw std::runtime_error("输入了无效序号。");
    }
    else{
        int index = order - 1;
            if (backpack_items[index].get()->getIsConsumable()) {
                backpack_items[index].get()->use(protagonist);
                backpack_items[index].reset();
                backpack_items.erase(backpack_items.begin() + index);
            }
            else {
                backpack_items[index].get()->equipAndUnequip(protagonist);
            }
    }
}