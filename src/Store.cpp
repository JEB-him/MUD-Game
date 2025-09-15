#include "Store.h"
#include "Controller.h"
#include "View.h"
#include "InputHandler.h"
#include "backpack.h"
#include "Protagonist.h"
#include <vector>
#include <string>
#include <fstream>
#include <filesystem>

using json = nlohmann::json;
Store::Store() {
    // 初始化商品列表
    auto controller = Controller::getInstance();
    std::filesystem::path items_path = controller->getRootDir() / ".config/Item.json";
    std::cout << items_path << std::endl;
    std::ifstream items_file(items_path.c_str());
    if (!items_file.is_open()) {
        controller->log(Controller::LogLevel::ERR, "项目配置异常");
        return;
    }
    items_file >> items;
}

Message Store::showProducts(int page) {
    std::stringstream ss;
    auto view = View::getInstance();
    auto controller = Controller::getInstance();
    controller->log(Controller::LogLevel::DEBUG, std::to_string(items.size()));

    if (page == -1) {
        page = 0;
        prompt();
    }
    size_t itemsPerPage = 10;
    size_t startIndex = (size_t)page * itemsPerPage;
    size_t totalPages = (items.size() + itemsPerPage - 1) / itemsPerPage;

    if (page < -1 || startIndex >= items.size()) {
        controller->log(Controller::LogLevel::ERR, "无效页码");
        return {"无效页码", 1};
    }

    // 显示当前页码信息
    ss << "第 " << page + 1 << " 页，共 " << totalPages << " 页";
    view->printQuestion("", ss.str(), "white");
    
    // 准备当前页的商品列表
    std::vector<std::string> currentPageProducts;
    size_t i = 0;
    for (auto [key, value]: items.items()) {
        if (startIndex <= i && i < startIndex + itemsPerPage) {
            ss.str("");
            ss << i - startIndex << ". " << value["name"].get<std::string>() << get_utf_empty(value["name"], 20) << value["value"];
            currentPageProducts.push_back(ss.str());
        } else if (i > startIndex + itemsPerPage) {
            break;
        }
        ++i;
    }
    
    // 显示商品列表
    view->printOptions(currentPageProducts);
    current_page = page;
    return {"Success", 0};
}

Message Store::buyProduct(int index) {
    size_t actualIndex = (size_t)index + current_page * 10;
    std::stringstream ss;
    auto view = View::getInstance();
    auto controller = Controller::getInstance();

    if (index < 1 || index > 10 || actualIndex < 0 || actualIndex >= items.size()) {
        controller->log(Controller::LogLevel::ERR, "无效的商品索引");
        return {"无效的商品索引", 1};
    }

    view->printQuestion("", "================", "white");
    size_t i = 0;
    std::string key_target;
    for (auto& [key, value] : items.items()) {
        if (i == actualIndex) {
            key_target = key;
            ss << value["name"] << " " << value["description"];
            view->printQuestion("", ss.str(), "white");
            break;
        }
        ++i;
    }
    view->printQuestion("", "================", "white");
    
    // 询问是否确认购买
    std::string question = "确认购买? (Y/N)";
    rechose:view->printQuestion("", question, "white");
    
    // 获取用户输入
    int response = controller->input->waitKeyDown();
    
    if (response == 'Y' || response == 'y') {
        if (controller->protagonist->getBaseAttrs()[BasicValue::ProtagonistAttr::MONEY] < items[key_target]["value"]) {
            view->printQuestion("", "您的余额不足，无法购物。", "white");
            return { "余额不足", 1 };
        }
        controller->protagonist->updateAttr(BasicValue::ProtagonistAttr::MONEY, -(items[key_target]["value"].get<int>()), true);
        controller->backpack->addItem(key_target);
        controller->protagonist->addGameTime(2);

        view->printQuestion("", "购买成功，欢迎下次光临！","white");
        return { "Success", 0 };
    }else if (response == 'N' || response == 'n') {
        view->printQuestion("", "没关系，感谢您的光临！", "white");
        return { "Refuse", 0 };
    }else {
        view->printQuestion("", "你思考了一会儿...", "white");
        goto rechose;
    }
    
}

void Store::prompt() {
    auto view = View::getInstance();
    std::vector<std::string> prompts {
        ""
        "欢迎来到带饭家！想买点什么呢？",
        "============ Help ============",
        "使用 store [页码] 翻页",
        "使用 buy [索引] 购买"
    };
    view->printOptions(prompts);
}

std::string Store::get_utf_empty(const std::string& text, const int& len) {
    size_t length = 0, index = 0;
    for (; index < text.length(); ) {
        unsigned char c = static_cast<unsigned char>(text[index]);
        if (c < 0x80) {
            // ASCII 字符（英文字母、数字等）
            index += 1;
            ++length;
        } else if ((c & 0xE0) == 0xC0) {
            // 2字节 UTF-8 字符
            index += 2;
            ++length;
        } else if ((c & 0xF0) == 0xE0) {
            // 3字节 UTF-8 字符（包括大部分中文）
            index += 3;
            length += 2;
        } else {
            // 不支持的 UTF-8 序列
            return "";
        }
    }
    std::string res;
    for (int i = 0; i < len - (int)length; ++ i) {
        res += " ";
    }
    return res;
}
