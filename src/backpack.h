/**
* @file backpack‌.h
* @author Xiaotian Wu(Cheongfan)
* @brief 背包类
* @details 实现功能：物品的获得、读取与使用。 \n
* 对应接口：addItem、getBackpack‌Items、useFunctionOfItem
* @note 物品个数、使用物品的方法， \n
*		通过vector.size()和getBackpack‌Items()[order - 1].get()-> <物品方法>  获取 ， \n
*		其中order为物品序号。
* @note 与物品相关的一切操作需要先与背包类进行交互。
* */

#pragma once
#include<iostream>
#include<string>
#include<memory>
#include<vector>
#include"Item.h"

using std::string;
using std::cout;
using std::endl;
using std::unique_ptr;
using std::make_unique;
using std::move;
using std::vector;

/**
 * @brief 背包类
 * @param item_creator 物品生成器
 * @param backpack_items 背包空间，储存所有物品对象的unique智能指针
 */
class Backpack {
public:

	/**
	 * @brief 背包类构造函数
	 * @details vector初始容量设定为0，容量超限则扩容10倍
	 */
	Backpack();

	/**
	 * @brief 背包类析构函数
	 */
	~Backpack() = default;

	/**
	 * @brief 获取背包管理物品智能指针的vector数组
	 * @return vector数组的引用
	 */
	vector<unique_ptr<Item>>& getBackpack‌Items();

	/**
	 * @brief  添加（生成）名为“item_name”的物品
	 * @param item_name 物品的名称
	 * @details 逻辑：利用ItemCreator的方法创造目标物品对象，存入backpack_items中
	 */
	void addItem(string item_name);

	/**
	 * @brief 使用或装备或卸下物品(三合一功能函数)
	 * @param order 操作的物品的序号(从1开始)
	 * @param protagonist 传入主角对象名
	 * @details 逻辑：区分好物品类型和装备状态，然后调用Item正确的接口(use、equipAndUnequip二选一)
	 * @details 逻辑：消耗品在调用此方法后，释放指针，然后序号在此物品之后的物品指针均前移一个位置
	 */
	void useFunctionOfItem(int order, Protagonist& protagonist);

private:
	ItemCreator item_creator;
	vector<unique_ptr<Item>> backpack_items;
	stringstream ss;
};