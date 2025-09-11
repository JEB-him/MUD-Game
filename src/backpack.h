/**
 * @file backpack‌.h
 * @author Xiaotian Wu(Cheongfan)
 * @brief 背包类
 * */

#pragma once
#include<iostream>
#include<string>
#include<memory>
#include<vector>
#include"Item.h"

using std::cout;
using std::endl;
using std::make_unique;
using std::move;
using std::vector;

/**
 * @brief 背包类
 * @param item_creator 物品生成器
 * @param backpack‌_space 背包空间，储存所有物品对象的unique智能指针
 * @param num_items 当前背包物品数量
 * @param capacity  背包最大容量
 */
class Backpack {
public:
	Backpack() : capacity(10), num_items(0), backpack_space() {};
	/**
	 * @brief 背包类构造函数
	 * @param capacity 设置最大容量
	 * @details vector初始容量设定为10，容量超限则扩容10倍
	 * @details num_items初始值为0
	 */
	Backpack();

	/**
	 * @brief 背包类析构函数
	 */
	~Backpack() = default;

	vector<unique_ptr<Item>>& getBackpack‌Items();

	/**
	 * @brief  
	* @details 利用ItemCreator的方法创造目标物品对象，存入backpack‌_space中，num_items++
	*/
	void addItem(string item_name);

	/**
	 * @brief 使用或装备或卸下物品(三合一功能函数)
	 * @details 区分好物品类型和装备状态，然后调用Item正确的接口(use、equipAndUnequip二选一)
	 * @details 消耗品在调用此方法后，指针释放，num_items--，然后序号在此物品之后的物品指针均前移一个位置
	 */
	void useFunctionOfItem(int order, Protagonist& protagonist);

	/**
	 * @brief 背包-序列化函数
	 */
	template <class Archive>
	void serialize(Archive &ar)
	{
		ar(CEREAL_NVP(capacity),
		   CEREAL_NVP(num_items),
		   CEREAL_NVP(backpack_space), );
		item_creator = ItemCreator();
	}

private:
	ItemCreator item_creator;
	vector<unique_ptr<Item>> backpack_items;
};