#include "Store.h"
#include "json.hpp"
#include "Controller.h"
#include "View.h"
#include <vector>
#include <string>
#include <fstream>
#include <filesystem>

Store::Store() {
    // 初始化商品列表
    auto controller = Controller::getInstance();
    std::filesystem::path items_path = controller->root_dir / 
    std::ifstream items_file()
}

void Store::showProducts(int page) {
    int itemsPerPage = 10;
    int startIndex = page * itemsPerPage;
    int totalPages = (products.size() + itemsPerPage - 1) / itemsPerPage;
    
    if (page < 0 || startIndex >= products.size()) {
        view->printQuestion("无效的页码!", {"white"});
        return;
    }
    
    // 显示当前页码信息
    std::string pageInfo = "第 " + std::to_string(page + 1) + " 页，共 " + 
                          std::to_string(totalPages) + " 页";
    view->printQuestion(pageInfo, {"white"});
    
    // 准备当前页的商品列表
    std::vector<std::string> currentPageProducts;
    for (int i = startIndex; i < startIndex + itemsPerPage && i < products.size(); i++) {
        currentPageProducts.push_back(std::to_string(i - startIndex) + ". " + products[i]);
    }
    
    // 显示商品列表
    view->printOptions(currentPageProducts);
    
    // 显示帮助信息
    prompt();
}

void Store::buyProduct(int index) {
    int actualIndex = index;
    
    if (actualIndex < 0 || actualIndex >= products.size()) {
        view->printQuestion("无效的商品索引!", {"white"});
        return;
    }
    
    // 询问是否确认购买
    std::string question = "是否确认购买 '" + products[actualIndex] + "'? (Y/N)";
    view->printQuestion(question, {"white"});
    
    // 获取用户输入
    char response = inputHandler->waitKeyDown();
    
    if (response == 'Y' || response == 'y') {
        view->printQuestion("购买成功! 感谢您的光临。", {"white"});
    } else {
        view->printQuestion("已取消购买。", {"white"});
    }
}

void Store::showHelp() {
    view->printQuestion("使用命令: page [num] 来翻页, buy [index] 来购买", {"white"});
}
