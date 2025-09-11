/**
* @brief NPC类实现
* */
#include "NPC.h"
#include "Protagonist.h"
#include <fstream>
#include <algorithm>
#include <string>
#include <iostream>
#include <iomanip>
#include <limits>
#include <sstream>
#include <vector>
#include "json.hpp"

using json = nlohmann::json;

/**
 * @brief 将NPCType枚举转换为字符串
 * @param type NPC类型
 * @return 对应的字符串表示
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
 * @param name NPC名称
 * @param id NPC唯一标识符
 */
NPC::NPC(const std::string& name, int id) 
    : name(name), id(id) {}

/**
 * @brief 加载交互配置
 * @param npcType NPC类型
 * @param configPath 配置文件路径
 */
void NPC::loadInteractionConfig(NPCType npcType, const std::string& configPath) {
    std::string typeStr = NPCTypeToString(npcType);
    
    // 清空旧交互树
    interactionTree.clear();
    currentInteractionId.clear();

    std::ifstream file(configPath);
    if (!file.is_open()) {
        throw std::runtime_error("无法打开配置文件: " + configPath);
    }

    try {
        json config;
        file >> config;

        // 验证角色配置存在
        if (!config.contains(typeStr)) {
            throw std::runtime_error("配置文件缺少" + typeStr + "角色配置");
        }
        auto& roleConfig = config[typeStr];

        // 检查initial字段
        if (!roleConfig.contains("initial")) {
            throw std::runtime_error("角色配置缺少initial字段");
        }
        if (!roleConfig["initial"].is_string()) {
            throw std::runtime_error("initial字段必须是字符串");
        }
        std::string initialNode = roleConfig["initial"].get<std::string>();
        currentInteractionId = typeStr + "_" + initialNode;

        // 加载交互节点
        if (!roleConfig.contains("interactions") || !roleConfig["interactions"].is_object()) {
            throw std::runtime_error("interactions字段缺失或不是对象");
        }
        auto& interactions = roleConfig["interactions"];
        for (auto& [nodeId, nodeData] : interactions.items()) {
            // 检查prompt字段
            if (!nodeData.contains("prompt") || !nodeData["prompt"].is_string()) {
                throw std::runtime_error("节点" + nodeId + "的prompt字段缺失或不是字符串");
            }
            std::string prompt = nodeData["prompt"].get<std::string>();
            InteractionNode node{
                prompt,
                {},
                {}
            };

            // 检查options字段
            if (!nodeData.contains("options") || !nodeData["options"].is_array()) {
                throw std::runtime_error("节点" + nodeId + "的options字段缺失或不是数组");
            }
            auto& options = nodeData["options"];
            for (auto& option : options) {
                // 检查text字段
                if (!option.contains("text") || !option["text"].is_string()) {
                    throw std::runtime_error("选项缺少text字段或不是字符串");
                }
                std::string text = option["text"].get<std::string>();

                std::string nextNode;
                if (option.contains("next")) {
                    if (option["next"].is_string()) {
                        nextNode = option["next"].get<std::string>();
                    } else if (option["next"].is_null()) {
                        nextNode = "END"; // 处理null值
                    } else {
                        throw std::runtime_error("next字段必须是字符串或null");
                    }
                } else {
                    nextNode = "END";
                }

                // 生成带前缀的nextNode
                if (!nextNode.empty()) {
                    nextNode = typeStr + "_" + nextNode;
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

            std::string prefixedId = typeStr + "_" + nodeId;
            interactionTree[prefixedId] = node;
        }

        // 验证初始节点存在
        if (interactionTree.find(currentInteractionId) == interactionTree.end()) {
            throw std::runtime_error("初始节点" + currentInteractionId + "不存在");
        }

    } catch (const json::exception& e) {
        throw std::runtime_error("JSON解析错误: " + std::string(e.what()));
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
        throw std::runtime_error("交互树未初始化");
    }
    
    if (currentInteractionId.empty()) {
        throw std::runtime_error("未设置初始交互节点");
    }
    
    auto it = interactionTree.find(currentInteractionId);
    if (it == interactionTree.end()) {
        throw std::runtime_error("找不到交互节点: " + currentInteractionId);
    }
    auto& node = it->second;

    // 显示交互提示和选项
    if (node.options.empty()) {
        std::cout << "***" << node.prompt << std::endl;
        return;
    }

    std::cout << "**" << node.prompt << std::endl;

    if (node.effect.size() > 0) {
        // 执行效果
        for (auto& [type, value] : node.effect) {
            int changeValue = std::stoi(value);

            if (type == "STRENGTH") {
                std::cout << "体力" << (changeValue > 0 ? "增加" : "减少") << std::abs(changeValue) << "点" << std::endl;
            } else if (type == "MONEY") {
                std::cout << "金钱" << (changeValue > 0 ? "获得" : "消耗") << std::abs(changeValue) << "元" << std::endl;
            }
        }
    }

    for (size_t i = 0; i < node.options.size(); ++i) {
        std::cout << i << ": " << node.options[i].text << std::endl;
    }

    int choice = -1;

    while (true) {
        std::cout << "请输入你的选择：";
        std::string input;
        std::getline(std::cin, input);

        try {
            size_t pos;
            choice = std::stoi(input, &pos);
            
            // 检查是否整个输入都是数字
            if (pos != input.length()) {
                throw std::invalid_argument("包含非数字字符");
            }

            // 检查选项范围
            if (choice >= 0 && choice < static_cast<int>(node.options.size())) {
                if (node.options[choice].conditions.size() > 0) {
                    if (node.options[choice].conditions[0].type == "MONEY") {
                        int requiredMoney = std::stoi(node.options[choice].conditions[0].value);
                        std::cout << "需要 " << requiredMoney << " 金钱, 当前金钱: " << playerMoney << std::endl;
                        if (playerMoney < requiredMoney) {

                            std::cout << "金钱不足，无法完成交易，请重新选择！" << std::endl;
                            continue; // 有效输入但条件不满足，重新选择
                        } else {
                            std::cout << "交易成功，当前金钱: " << playerMoney << std::endl; 
                        }
                    }
                }
                break; // 有效输入，退出循环
            } else {
                std::cout << "选择超出范围，请输入有效选项编号" << std::endl;
            }
        }
        catch (const std::exception&) {
            std::cout << "无效输入，请输入数字选项" << std::endl;
            // 清除错误状态（针对cin直接读取的情况）
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    }

    handleOptionSelection(choice);
}
/**
 * @brief 处理选项选择
 * @param optionIndex 选项索引
 * @details 根据用户选择更新当前交互节点，递归调用startInteraction()处理下一个节点。
 * @throws std::runtime_error 选项索引超出范围、下一个节点不存在
 */
void NPC::handleOptionSelection(int optionIndex) {
    if (!interactionTree[currentInteractionId].options.empty() 
        && interactionTree[currentInteractionId].options.size() > optionIndex) {
        auto& option = interactionTree[currentInteractionId].options[optionIndex];
        currentInteractionId = option.next;
        startInteraction();
    } else {
        throw std::runtime_error("选项索引超出范围");
    }

    
}