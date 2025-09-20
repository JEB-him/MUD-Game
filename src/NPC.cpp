/**
* @brief NPC类实现
* */
#include "NPC.h"
#include "Protagonist.h"
#include "Controller.h"
#include "View.h"
#include "backpack.h"
#include "InputHandler.h"

#include <fstream>
#include <algorithm>
#include <string>
#include <iostream>
#include <iomanip>
#include <limits>
#include <sstream>
#include <vector>
#include <memory>
#include <unordered_map>
#include "json.hpp"

using json = nlohmann::json;

/**
 * @brief 将NPCType枚举转换为字符串
 * @param type NPC类型
 * @return 对应的字符串表示
 * @deprecated Controller 将直接使用字符串创建 NPC
 */
const std::string& NPCTypeToString(NPCType type) {
    static const std::unordered_map<NPCType, std::string> typeMap {
        {NPCType::STUDENT, "Student"},
        {NPCType::PROFESSOR, "Professor"},
        {NPCType::CANTEEN_STAFF, "CanteenStaff"},
        {NPCType::ROOMMATE, "Roommate"},
        {NPCType::DORM_MANAGER, "DormManager"},
        {NPCType::EMPLOYER, "Employer"},
        {NPCType::LIBRARIAN, "Librarian"},
        {NPCType::COACH, "Coach"}
    };
    return typeMap.at(type);
}

/**
 * @brief 构造函数
 * @param name NPC名称\n
 *        1. 配置文件中的姓和名分开
 *        2. 如果配置文件不指定姓名，则必须指定性别，以进行随机分配
 * @param id NPC唯一标识符
 */
NPC::NPC(const std::string& first_name, const std::string& last_name, int id) 
    : first_name(first_name), last_name(last_name), id(id) {
    name = first_name + last_name;
}

/**
 * @brief 加载交互配置
 * @param npc_type NPC (string)
 * @param config_path 配置文件路径
 */
void NPC::loadInteractionConfig(const std::string& npc_type, const std::string& config_path) {
    
    // 清空旧交互树
    interactionTree.clear();
    currentInteractionId.clear();

    std::ifstream file(config_path.c_str());
    if (!file.is_open()) {
        Controller::getInstance()->log(Controller::LogLevel::ERR, "无法打开配置文件: " + config_path);
        Controller::getInstance()->gameExit();
    }

    try {
        json config;
        file >> config;

        // 验证角色配置存在
        if (!config.contains(npc_type)) {
            Controller::getInstance()->log(Controller::LogLevel::ERR, "配置文件缺少" + npc_type + "角色配置");
            Controller::getInstance()->gameExit();
        }
        auto& roleConfig = config[npc_type];

        // 检查initial字段
        if (!roleConfig.contains("initial")) {
            Controller::getInstance()->log(Controller::LogLevel::ERR, "角色配置缺少initial字段");
            Controller::getInstance()->gameExit();
        }
        if (!roleConfig["initial"].is_string()) {
            Controller::getInstance()->log(Controller::LogLevel::ERR, "initial字段必须是字符串");
            Controller::getInstance()->gameExit();
        }
        std::string initialNode = roleConfig["initial"].get<std::string>();
        currentInteractionId = npc_type + "_" + initialNode;

        // 加载交互节点
        if (!roleConfig.contains("interactions") || !roleConfig["interactions"].is_object()) {
            Controller::getInstance()->log(Controller::LogLevel::ERR, "interactions字段缺失或不是对象");
            Controller::getInstance()->gameExit();
        }
        auto& interactions = roleConfig["interactions"];
        for (auto& [nodeId, nodeData] : interactions.items()) {
            // 检查prompt字段
            if (!nodeData.contains("prompt") || !nodeData["prompt"].is_string()) {
                Controller::getInstance()->log(Controller::LogLevel::ERR, "节点" + nodeId + "的prompt字段缺失或不是字符串");
                Controller::getInstance()->gameExit();
            }
            std::string prompt = nodeData["prompt"].get<std::string>();
            replaceText(prompt);
            InteractionNode node{
                prompt,
                {},
                {}
            };

            // 检查options字段
            if (!nodeData.contains("options") || !nodeData["options"].is_array()) {
                Controller::getInstance()->log(Controller::LogLevel::ERR, "节点" + nodeId + "的options字段缺失或不是数组");
                Controller::getInstance()->gameExit();
            }
            auto& options = nodeData["options"];
            for (auto& option : options) {
                // 检查text字段
                if (!option.contains("text") || !option["text"].is_string()) {
                    Controller::getInstance()->log(Controller::LogLevel::ERR, "选项缺少text字段或不是字符串");
                    Controller::getInstance()->gameExit();
                }
                std::string text = option["text"].get<std::string>();
                replaceText(text);

                std::string nextNode;
                if (option.contains("next")) {
                    if (option["next"].is_string()) {
                        nextNode = option["next"].get<std::string>();
                    } else if (option["next"].is_null()) {
                        nextNode = "END"; // 处理null值
                    } else {
                        Controller::getInstance()->log(Controller::LogLevel::ERR, "next字段必须是字符串或null");
                        Controller::getInstance()->gameExit();
                    }
                } else {
                    nextNode = "END";
                }

                // 生成带前缀的nextNode
                if (!nextNode.empty()) {
                    nextNode = npc_type + "_" + nextNode;
                }

                // 解析condition字段
                std::vector<Condition> conditions;
                if (option.contains("condition") && option["condition"].is_object()) {
                    for (auto& [key, value] : option["condition"].items()) {
                        conditions.push_back({key, value.get<std::string>()});
                    }
                }

                node.options.push_back(InteractionOption{text, nextNode, conditions});
            }

            // 解析effect字段
            if (nodeData.contains("effect") && nodeData["effect"].is_object()) {
                for (auto& [key, value] : nodeData["effect"].items()) {
                    node.effect.push_back({key, value.get<std::string>()});
                }
            }

            std::string prefixedId = npc_type + "_" + nodeId;
            interactionTree[prefixedId] = node;
        }

        // 验证初始节点存在
        if (interactionTree.find(currentInteractionId) == interactionTree.end()) {
            Controller::getInstance()->log(Controller::LogLevel::ERR, "初始节点" + currentInteractionId + "不存在");
            Controller::getInstance()->gameExit();
        }

    } catch (const json::exception& e) {
        Controller::getInstance()->log(Controller::LogLevel::ERR, "JSON解析错误: " + std::string(e.what()));
        Controller::getInstance()->gameExit();
    }
}

/**
 * @brief 开始交互
 * @details 从当前交互节点开始，递归处理用户选择，直到遇到结束节点。
 * @throws std::runtime_error 交互树未初始化、未设置初始交互节点、找不到交互节点
 */
void NPC::startInteraction() {

    // handler->getOption();
    // std::stringstream ss;
    // int playerMoney = 10; //Protagonist::getInstance().getMoney();  假设主角当前金钱为10，实际应从Protagonist对象获取 
    // for (auto& [key, value] : interactionTree) {
    //     std::cout << "node: " << key << std::endl;
    //     for (auto& [key, value] : value.effect) {
    //         ss << "effect: " << key << " " << value << std::endl;
    //         view->gameOutput(ss.str());
    //         ss.str("");
    //     }
    // }
    if (interactionTree.empty()) {
        Controller::getInstance()->log(Controller::LogLevel::ERR, "交互树未初始化");
        Controller::getInstance()->gameExit();
    }
    
    if (currentInteractionId.empty()) {
        Controller::getInstance()->log(Controller::LogLevel::ERR, "未设置初始交互节点");
        Controller::getInstance()->gameExit();
    }
    
    auto it = interactionTree.find(currentInteractionId);
    if (it == interactionTree.end()) {
        Controller::getInstance()->log(Controller::LogLevel::ERR, "找不到交互节点: " + currentInteractionId);
        Controller::getInstance()->gameExit();
    }
    auto& node = it->second;
    auto view = View::getInstance();

    view->printQuestion("", node.prompt, "white");

    auto controller = Controller::getInstance();
    if (!controller) {
        controller->log(Controller::LogLevel::ERR, "悬空指针： controller");
    }

    if (!view) {
        controller->log(Controller::LogLevel::ERR, "悬空指针： view");
    }
    if (!(controller->protagonist)) {
        controller->log(Controller::LogLevel::ERR, "悬空指针： protagonist");
    }
    if (!(controller->input)) {
        controller->log(Controller::LogLevel::ERR, "悬空指针： input");
    }
    if (!(controller->backpack)) {
        controller->log(Controller::LogLevel::ERR, "悬空指针： backpack");
    }
    auto backpack = controller->backpack;
    auto input = controller->input;
    auto protagonist = controller -> protagonist;
    auto base_attrs = controller -> protagonist -> getBaseAttrs();
    // TODO 修复 map 类型
    int current_money = base_attrs[BasicValue::ProtagonistAttr::MONEY];
    ss.str("");
    ss << "当前金钱: $" << current_money;
    view->printQuestion("", ss.str(), "yellow");

    ss.str("");
    ss << "*-*" << node.prompt;
    view->printQuestion("", ss.str(), "white");

    if (node.effect.size() > 0) {
        // 执行效果
        for (auto& [type, value] : node.effect) {
                int changeValue;
            try {
                changeValue = std::stoi(value);
            } catch (const std::invalid_argument& e) {
                ss.str("");
                ss << "无效Effect in NPC process: " << e.what();
                controller->log(Controller::LogLevel::ERR, ss.str());
                continue;
            } catch (const std::out_of_range& e) {
                ss.str("");
                ss << "Effect exceed \'int\' range in NPC process: " << e.what();
                controller->log(Controller::LogLevel::ERR, ss.str());
                continue;
            }

            Message success_msg;
            ss.str("");
            if (type == "STRENGTH") {
                success_msg = controller -> protagonist -> updateAttr(BasicValue::ProtagonistAttr::STRENGTH, changeValue, true);
                ss << "体力" << (changeValue > 0 ? "增加" : "减少") << std::abs(changeValue) << "点";
            } else if (type == "MONEY") {
                controller -> protagonist -> updateAttr(BasicValue::ProtagonistAttr::MONEY, changeValue, true);
                ss << "金钱" << (changeValue > 0 ? "获得" : "消耗") << std::abs(changeValue) << "元";
            } else if (type == "INTEL_ARTS") {
                controller -> protagonist -> updateAttr(BasicValue::ProtagonistAttr::INTEL_ARTS, changeValue, true);
                ss << "文科能力" << (changeValue > 0 ? "提升" : "降低") << std::abs(changeValue) << "点";
            } else if (type == "INTEL_SCI") {
                controller -> protagonist -> updateAttr(BasicValue::ProtagonistAttr::INTEL_SCI, changeValue, true);
                ss << "理科能力" << (changeValue > 0 ? "提升" : "降低") << std::abs(changeValue) << "点";
            } else if (type == "TIME") {
                controller->protagonist->addGameTime(changeValue);
                ss << "时间" << (changeValue > 0 ? "逆流" : "流逝") << std::abs(changeValue) << "小时";
            } else if (type == "OBJ") {
                backpack->addItem(value);
                ss << "获得物品" << backpack->getItemInf(value).name;
            }
                view->printQuestion("", ss.str(), "white");
            
            if (success_msg.status != 0) {
                controller->log(Controller::LogLevel::ERR, success_msg.msg);
            }
        }
    }

    // 输出选项
    std::vector<std::string> outputs_options;
    for (size_t i = 0; i < node.options.size(); ++i) {
        ss.str("");
        ss << i << ": " << node.options[i].text;
        outputs_options.push_back(ss.str());
    }
    view->printOptions(outputs_options);

    int choice = -1;
    controller->log(Controller::LogLevel::DEBUG, "node.options.size()=" + std::to_string(node.options.size()));
    if (node.options.size() == 0)
    {
        controller->log(Controller::LogLevel::DEBUG, "Exit node");
        return;
    }

    while (true) {
        int press_ascii;
        while (1)
        {
            press_ascii = Controller::getInstance()->input->waitKeyDown();
            if (press_ascii >= '0' && (press_ascii - '0') <= node.options.size())
            {
                break;
            }
        }
        view->reDraw();
        press_ascii -= '0';
        choice = press_ascii;
        controller->log(Controller::LogLevel::DEBUG, "用户输入：" + std::to_string(press_ascii));
        if (press_ascii < 0 || press_ascii > 9) {
            controller->log(Controller::LogLevel::ERR, "非法选项输入");
            continue;
        }

        if (choice < 0)
            controller->log(Controller::LogLevel::DEBUG, "choice < 0!");
        if (static_cast<size_t>(choice) >= node.options.size())
            controller->log(Controller::LogLevel::DEBUG, "static_cast<size_t>(choice) >= node.options.size()");

        // 检查选项范围
        if (choice >= 0 && static_cast<size_t>(choice) < node.options.size()) {
            try {
                if (node.options[choice].conditions.size() > 0) {
                    bool conditionsMet = true;
                    for (auto& [type, value] : node.options[choice].conditions) {
                        if (node.options[choice].conditions[0].type == "MONEY") {
                            int required_money = std::stoi(value);
                            if (current_money < required_money) {
                                conditionsMet = false;
                                view->printQuestion("", "金钱不足，无法完成交易，请重新选择！", "white");
                                break; // 有效输入但条件不满足，重新选择
                            }
                        }
                        if (node.options[choice].conditions[0].type == "TIME") {
                            int required_time = std::stoi(value);
                            int current_time = base_attrs[BasicValue::ProtagonistAttr::GAME_TIME];
                            if (current_time < required_time) {
                                view->printQuestion("", "时间不足，请重新选择！", "white");
                                conditionsMet = false;
                                break; // 有效输入但条件不满足，重新选择
                            }                       
                        }
                    }
                    if (!conditionsMet) {
                        continue; // 条件不满足，重新选择
                    }
                }
            } catch (const std::invalid_argument& e) {
                ss.str("");
                ss << "无效条件" << e.what();
                controller->log(Controller::LogLevel::ERR, ss.str());
                continue;
            } catch (const std::out_of_range& e) {
                ss.str("");
                ss << "Effect exceed \'int\' range in NPC condition process: " << e.what();
                controller->log(Controller::LogLevel::ERR, ss.str());
                continue;
            }
            controller->log(Controller::LogLevel::DEBUG, "有效选项已选择：" + std::to_string(choice));
            break; // 有效输入，退出循环
        } else {
            view->printQuestion("", "选择超出范围，请输入有效选项编号", "white");
        }
    }

    view->reDraw();
    handleOptionSelection(choice);
    return;
}
/**
 * @brief 处理选项选择
 * @param optionIndex 选项索引
 * @details 根据用户选择更新当前交互节点，递归调用startInteraction()处理下一个节点。
 * @throws std::runtime_error 选项索引超出范围、下一个节点不存在
 */
void NPC::handleOptionSelection(int optionIndex) {
    if (!interactionTree[currentInteractionId].options.empty() && interactionTree[currentInteractionId].options.size() > optionIndex)
    {
        auto &option = interactionTree[currentInteractionId].options[optionIndex];
        currentInteractionId = option.next;

        Controller::getInstance()->log(Controller::LogLevel::DEBUG, "选项已完成！即将进入下一个节点：");
        Controller::getInstance()->log(Controller::LogLevel::DEBUG, "optionIndex: " + std::to_string(optionIndex) + " next: " + currentInteractionId);
        startInteraction();
    }
    else
    {
        Controller::getInstance()->log(Controller::LogLevel::ERR, "选项索引超出范围");
        Controller::getInstance()->gameExit();
    }
}

// void NPC::handleOptionSelection(int optionIndex) {
//     if (!interactionTree[currentInteractionId].options.empty()
//         && interactionTree[currentInteractionId].options.size() > optionIndex) {
//         auto& option = interactionTree[currentInteractionId].options[optionIndex];

//         // 检查是否是退出选项
//         if (option.next == "END")
//         {
//             auto view = View::getInstance();
//             view->printQuestion("", "对话结束，再见！", "white");
//             Controller::getInstance()->log(Controller::LogLevel::DEBUG, "对话结束，NPC离开...");
//             currentInteractionId = "END"; // 设置为结束状态
//             return;
//         }

//         currentInteractionId = option.next;
//         startInteraction(); // 继续下一个节点
//         // ... 日志输出 ...
//     } else {
//         throw std::runtime_error("选项索引超出范围");
//     }
// }

bool NPC::replaceText(std::string& text) {
    // 定义占位符和对应的替换值
    const std::unordered_map<std::string_view, std::string> replacements = {
        {"{{name}}", name},
        {"{{first_name}}", first_name},
        {"{{last_name}}", last_name}
    };
    
    bool replaced = false;
    
    // 遍历所有占位符并进行替换
    for (const auto& [placeholder, value] : replacements) {
        size_t pos = 0;
        while ((pos = text.find(placeholder, pos)) != std::string::npos) {
            text.replace(pos, placeholder.length(), value);
            pos += value.length();
            replaced = true;
        }
    }

    return replaced;
}
