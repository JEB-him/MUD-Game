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
    NPC(const std::string& name, int id);
    ~NPC() = default;
    void loadInteractionConfig(NPCType npcType, const std::string& configPath);
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

    std::string name;
    int id;
    std::string currentInteractionId;
    std::unordered_map<std::string, InteractionNode> interactionTree;
    std::unordered_map<std::string, std::function<void()>> interactionCallbacks;
    mutable int playerMoney = 19; // 假设主角当前金钱为10，实际应从Protagonist对象获取
};

