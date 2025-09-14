#pragma once
/**
 * @brief 非玩家角色类
 * @details   身份一览：学生Student、舍友Roommate(继承于学生)、宿舍管理员DormManager 、
          招聘人员Employer or 兼职中介PartTimeAgent 、食堂工作人员CanteenStaff 、
          教师Professor 、教练员Coach、图书管理员Librarian 等
 * */
#include <string>
#include <vector>
#include <functional>
#include "json.hpp"
#include "NPCType.h"
#include <unordered_map>
#include "Protagonist.h"

class NPC {
    
public:
    // Protagonist& m_player; // 引用主角对象，便于访问和修改主角属性
    NPC(const std::string& first_name="", const std::string& last_name="", int id=0);
    ~NPC() = default;
    void loadInteractionConfig(const std::string& npc_type, const std::string& configPath);
    void startInteraction();
    void handleOptionSelection(int optionIndex);

private:

    struct Effect {
        std::string type;
        std::string value;
    };

    struct Condition {
        std::string type;
        std::string value;
    };
    
    struct InteractionOption {
        std::string text;
        std::string next;
        std::vector<Condition> conditions;
    };

    struct InteractionNode {
        std::string prompt;
        std::vector<InteractionOption> options;
        std::vector<Effect> effect;
    };

    // 采取中文姓名，name 由 last_name 和 first_name 无空格拼接
    std::string first_name;
    std::string last_name;
    std::string name;

    int id;
    std::string currentInteractionId;
    std::unordered_map<std::string, InteractionNode> interactionTree;
    std::unordered_map<std::string, std::function<void()>> interactionCallbacks;

    // 一个流用来处理输出
    std::stringstream ss;
    bool replaceText(std::string& text);
};

