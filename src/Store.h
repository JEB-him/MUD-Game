/**
 * @file Store.h
 * @author Jie Jiang
 */

/**
 * @brief 商店类，展示所有可以出售的商品
 * @details 占据特殊字符 s
 */
#pragma once

#include "tools.h"
#include "json.hpp"

// 前向声明视图和输入处理类
class View;
class InputHandler;

class Store {
public:
    Store();
    
    /**
     * @brief 展示商品
     * @param page 页码，-1 为默认值
     * @return msg\n
     *         status: 0 成功, 1 非有效页码
     */
    Message showProducts(int page=-1);
    
    /**
     * @brief 购买商品
     * @return msg 返回是否购买成功
     */
    Message buyProduct(int index);
    
    /**
     * @brief 显示帮助信息和旁白
     * @details 当页码为-1或0的时，显示
     */
    void prompt();

private:
    std::string name="带饭家";
    nlohmann::json items;
    size_t current_page = 0;
    static std::string get_utf_empty(const std::string& text, const int& len);
};
