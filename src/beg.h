/**
* @file beg.h
* @author Xiaotian Wu(Cheongfan)
* @brief 背包类
* */

#pragma once
#include<iostream>
#include<string>
#include<memory>
#include<vector>
#include"Item.h"
#include"Item.cpp"

using std::string;
using std::cout;
using std::endl;
using std::unique_ptr;
using std::make_unique;
using std::move;
using std::vector

/**
 * @brief 背包类
 * @param item_creator 物品生成器
 * @param beg_space 背包空间，储存所有物品对象的unique智能指针
 * @param num_items 当前背包物品数量
 * @param capacity  背包最大容量
 */
class Beg {
public:

	/**
	 * @brief 背包类构造函数
	 * @param capacity 设置最大容量
	 * @details vector初始容量设定为10，容量超限则扩容10倍
	 * @details num_items初始值为0
	 */
	Beg(int capacity);

	/**
	 * @brief 背包类构造函数
	 * @details 循环释放vector的所有unique智能指针
	 */
	~Beg();

	/**
	 * @brief 打印序号+背包的所有物品名称
	 * @details 1.高等数学 2.篮球 3.压缩饼干 4....
	 * @details 通过item类getName()获取物品名字
	 */
	void printItems()const;

	/**
	 * @brief 打印物品选项
	 * @details 1.详情
	 * @details 2.使用/装备/卸下(只打印其中之一)
	 * @details 通过item.getIsComsumable()区分消耗品和用具
	 * @details 用具根据item.getEquipState()区分装备还是卸下
	 */
	void printOptionsOfItem()const;

	/**
	 * @brief 打印物品详细介绍
	 * @note  上一个界面选"1.详情"后使用该接口反馈信息
	 * @details 输出item.getDescription
	 */
	void printDetails()const;

	/**
	 * @brief  
	* @details 利用ItemCreator的方法创造目标物品对象，存入beg_space中，num_items++
	*/
	void addItem(string item_name)

	/**
	 * @brief 使用或装备或卸下物品(三合一功能函数)
	 * @details 区分好物品类型和装备状态，然后调用Item正确的接口(use、equipAndUnequip二选一)
	 * @details 消耗品在调用此方法后，指针释放，num_items--，然后序号在此物品之后的物品指针均前移一个位置
	 */
	void useFunctionOfItem();

private:
	ItemCreator item_creator;
	vector<unique_ptr<Item>> beg_space;
	int num_items;
	int capacity;
};