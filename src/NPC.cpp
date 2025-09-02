/**
* @brief NPC类实现
* */

#include "NPC.h"
#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include <random>
#include <fstream>
#include "json.hpp"

using json = nlohmann::json;

/// @brief 树状交互逻辑入口
void NPC::startInteraction() const {
    std::cout << interaction_text << "\n";
        
    if (interaction_options.empty()) {
        std::cout << "暂时没有可交互的选项。\n";
        return;
    }
       
    // 显示一级交互选项
    std::cout << "请选择交互选项：\n";
    for (size_t i = 0; i < static_cast<int>(interaction_options.size()); ++i) {
        std::cout << i + 1 << ". " << interaction_options[i] << "\n";
    }
    std::cout << "0. 退出交互\n";
        
    int choice;
    while (true) {
        std::cout << "请输入你的选择：";
        std::cin >> choice;
            
        /**
        * @brief 处理用户输入
        * @details 检查用户输入是否为整数，若不是则提示用户重新输入
        */
        if (std::cin.fail()) {
            // 清除错误状态
            std::cin.clear();
            // 忽略错误的输入
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "输入无效，请输入数字选项：\n";
            continue;
        }
            
        if (choice == 0) {
            std::cout << "退出交互。\n";
            return;
        }
            
        if (choice > 0 && choice <= static_cast<int>(interaction_options.size())) {
            handleInteraction(choice - 1);
            return;
        } else {
            std::cout << "无效的选择，请重新选择：\n";
        }
    }
}

/// @brief 处理具体交互选项（可被子类重写）
void NPC::handleInteraction(int option_index) const {
    /// @brief 处理具体交互选项（可被子类重写）
    std::cout << "你选择了：" << interaction_options[option_index] << "\n";
    // 基类默认处理，子类可以重写实现具体逻辑
    std::cout << "这个功能还在开发中。\n";
}

/// @brief 获取称呼
std::string NPC::getTitle() const { return title; }
/// @brief 获取身份
std::string NPC::getIdentity() const { return job; }
/// @brief 获取交互文本
std::string NPC::getInteractionText() const { return interaction_text; }
/// @brief 获取交互选项
std::vector<std::string> NPC::getInteractionOptions() const { return interaction_options; }

/// @brief 设置称呼
void NPC::setTitle(const std::string& new_title) { title = new_title; }
/// @brief 设置身份
void NPC::setIdentity(const std::string& new_identity) { job = new_identity; }
/// @brief 设置交互文本
void NPC::setInteractionText(const std::string& text) { interaction_text = text; }
/// @brief 添加交互选项
void NPC::addInteractionOption(const std::string& option) { interaction_options.push_back(option); }

/**
*@brief 学生类构造函数
* */
Student::Student(const std::string& title, int intelligence = 50)
    : NPC(title, "学生"), intelligence(intelligence) {
    setInteractionText("你好，我是学生" + title + "，我们一起学习吧！");
    addInteractionOption("请教问题");
}

/**
 * @brief 处理具体交互选项
 * @details 处理学生类的具体交互选项，如请教问题
 * @param option_index 交互选项索引
 * */
void Student::handleInteraction(int option_index) const {
        if (option_index == 0) { // 请教问题
            std::cout << "你向" << getTitle() << "请教问题。\n";
            // 需增加提升智力处理代码，需向主角类获取接口
            
        }
    }

int Student::getIntelligence() const { return intelligence; } ///< 获取智力
void Student::setIntelligence(int value) { intelligence = value; } ///< 设置智力

/**
*@brief 学长类构造函数
* */
SeniorStudent::SeniorStudent(const std::string& title, int intelligence = 60, 
             const std::string& major = "计算机")
    : Student(title, intelligence), major(major) {
    setIdentity("学长");
    setInteractionText("你好，我是" + major + "专业的学长" + title + "，最近有点忙，能帮帮我吗？");
    addInteractionOption("帮助处理问题");
}

/**
*@brief 处理具体交互选项
*@details 处理学长类的具体交互选项，如帮助处理问题
* */
void SeniorStudent::handleInteraction(int option_index) const {
    if (option_index == 0) { // 帮助处理问题
        SeniorStudent::handleHelpRequest();
    }
}

/**
 * @brief 处理帮助请求
 * @details 模拟小游戏
 * */
void SeniorStudent::handleHelpRequest() const {
    std::cout << getTitle() << "：太好了！我这里有个问题需要帮忙处理。\n";
        
    // 模拟小游戏
    std::cout << "=== 小游戏：解决学长的问题 ===\n";
    std::cout << "请计算：25 + 17 = ?\n";
    
    int answer;
    std::cin >> answer;
    
    // 检查答案
    if (answer == 42) {
        std::cout << "恭喜你答对了！学长的问题解决了。\n";
        // 奖励
        std::cout << "奖励：智力文理+" << 10 << "\n";
        std::cout << "消耗：时间+1小时,体力-15\n";
            
        // 这里需要调用主角类的接口来实际修改属性
    } else {
        std::cout << "答案错误，再仔细想想吧！\n";
        // 消耗
        std::cout << "消耗：时间+0.5小时，体力-5\n";
        // 这里需要调用主角类的接口来实际修改属性
    }
}

/// @brief 获取专业
std::string SeniorStudent::getMajor() const { return major; }
/// @brief 设置专业
void SeniorStudent::setMajor(const std::string& new_major) { major = new_major; }

/**
 * @brief 舍友类构造函数
 * */
Roommate::Roommate(const std::string& title, int intelligence = 50)
    : Student(title, intelligence) {
    setIdentity("舍友");
    setInteractionText("嘿，" + title + "在这里！需要帮忙吗？");
    addInteractionOption("开黑打游戏");
    addInteractionOption("请室友帮忙带饭");  // 玩家请室友带饭
    addInteractionOption("帮室友带饭");      // 玩家帮室友带饭
    // hasHelpedWithMeal = false;
    // hasBoughtMeal = false;
    meal_cost = 10; // 带饭费用
}

/**
 * @brief 完成玩家请室友带饭后交互内容
 * */
void Roommate::completeMealRequest() { 
    std::cout << getTitle() << "：这是你的饭！\n";
}
    
/**
 * @brief 完成玩家帮室友带饭后交互内容
 * */
void Roommate::completeMealProvide() { 
    std::cout << getTitle() << "：谢谢你帮我带饭！下次再麻烦你！\n";
}

/**
 * @brief 处理具体交互选项
 * @details 处理舍友类的具体交互选项，如开黑打游戏、请室友帮忙带饭、帮室友带饭
 * @param option_index 交互选项索引
 * */
void Roommate::handleInteraction(int option_index) const {
    switch (option_index) {
        case 0: // 开黑打游戏
            std::cout << getTitle() << "：来来来，开黑打游戏！\n";
            // 需增加打游戏收益处理代码 
            break;
            
        case 1: // 请室友帮忙带饭（玩家请室友带饭）
            std::cout << getTitle() << "：没问题！帮你带饭需要支付跑腿费" << meal_cost << "金币。确认吗？(y/n)\n";
            char confirm1;
            std::cin >> confirm1;
            if (confirm1 == 'y' || confirm1 == 'Y') {
                std::cout << "已支付" << meal_cost << "金币！" << getTitle() << "会去食堂帮你带饭。\n";
                std::cout << "(转向食堂工作人员选择菜品...)\n";
                // 需增加带饭收益处理代码
                // const_cast<Roommate*>(this)->hasBoughtMeal = true;
            } else {
                std::cout << "取消带饭请求。\n";
            }
            break;
            
        case 2: // 帮室友带饭（玩家帮室友带饭）
            std::cout << getTitle() << "：太好了！你要帮我带饭吗？我会支付你跑腿费" << meal_cost << "金币。确认吗？(y/n)\n";
            char confirm2;
            std::cin >> confirm2;
            if (confirm2 == 'y' || confirm2 == 'Y') {
                std::cout << getTitle() << "支付了" << meal_cost << "金币！请去食堂帮我带饭吧。\n";
                std::cout << "(转向食堂工作人员选择菜品...)\n";
                // 需增加带饭收益处理代码
                // const_cast<Roommate*>(this)->hasHelpedWithMeal = true;
            } else {
                std::cout << "取消带饭帮助。\n";
            }
            break;
    }
}

/**
 * @brief 获取带饭费用
 * @return 带饭费用
 */
int Roommate::getMealCost() const { return meal_cost; }

/**
 * @brief 宿舍管理员类构造函数
 * */
DormManager::DormManager(const std::string& title)
        : NPC(title, "宿舍管理员"), familiarity(0) {
        setInteractionText("我是宿舍管理员" + title + "，有什么需要帮忙的吗？");
        addInteractionOption("对话");
        addInteractionOption("帮忙干活");
    }

/**
 * @brief 处理对话
 * @details 与管理员的交互，增加熟悉度
 */
void DormManager::handleInteraction(int option_index) const {
    switch (option_index) {
        case 0: // 对话
            const_cast<DormManager*>(this)->familiarity += 5;
            std::cout << "与" << getTitle() << "对话，熟悉度提升到：" << familiarity << "\n";
            DormManager::checkSpecialDialogue();
            break;
        case 1: // 帮忙干活
            const_cast<DormManager*>(this)->familiarity += 10;
            std::cout << "帮忙干活，熟悉度提升到：" << familiarity << "\n";
            DormManager::checkSpecialDialogue();
            break;
    }
}
/**
 * @brief 检查特殊对话
 * @details 当熟悉度超过50时，有30%的概率触发特殊对话
 */
void DormManager::checkSpecialDialogue() const {
    if (familiarity >= 50) {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(1, 10);
            
        if (dis(gen) <= 3) {
            std::cout << getTitle() << "（神秘地说）：谢谢你一直的帮助，这个限定物品送给你！\n";
            // 需增加奖励处理代码
        }
    }
}

/**
 * @brief 获取熟悉度
 * @return 熟悉度
 */
int DormManager::getFamiliarity() const { return familiarity; }

/**
 * @brief 招聘人员类构造函数
 * */
Employer::Employer(const std::string& title, int money_reward, int stamina_cost, 
             const std::string& promotion_text)
        : NPC(title, "招聘人员"), money_reward(money_reward), stamina_cost(stamina_cost) {
        setInteractionText(promotion_text);
        addInteractionOption("接受工作");
        addInteractionOption("询问详情");
    }

/**
 * @brief 处理与招聘人员的交互
 * */
void Employer::handleInteraction(int option_index) const {
    switch (option_index) {
        case 0: // 接受工作
            std::cout << "你接受了工作，获得" << money_reward << "金币，消耗" << stamina_cost << "体力。\n";
            break;
        case 1: // 询问详情
            std::cout << "工作详情：奖励" << money_reward << "金币，需要消耗" << stamina_cost << "体力。\n";
                break;
    }
        //应完善
}

/**
 * @brief 图书管理员类构造函数
 * */
Librarian::Librarian(const std::string& title)
    : NPC(title, "图书管理员") {
    setInteractionText("欢迎来到图书馆，我是图书管理员" + title + "，需要帮助吗？");
    addInteractionOption("寻求知识帮助");
}

/**
 * @brief 处理与图书管理员的交互
 * */
void Librarian::handleInteraction(int option_index) const {
    if (option_index == 0) {
        std::cout << "在" << getTitle() << "的帮助下，你的智力得到了提升！\n";
    }
}

/**
 * @brief 食堂工作人员类构造函数
 * */
CanteenStaff::CanteenStaff(const std::string& title, 
                const std::vector<std::string>& food_types,
                const std::vector<int>& stamina_recovery,
                const std::vector<int>& food_costs)
        : NPC(title, "食堂工作人员"), food_types(food_types), 
          stamina_recovery(stamina_recovery), food_costs(food_costs) {
        setInteractionText("欢迎光临食堂！我是" + title + "，今天想吃点什么？");
        addInteractionOption("查看菜单");
        addInteractionOption("购买食物");
        addInteractionOption("为舍友买饭");
    }

/**
 * @brief 处理与食堂工作人员的交互
 * */
void CanteenStaff::handleInteraction(int option_index) const {
    switch (option_index) {
        case 0: // 查看菜单
            CanteenStaff::showMenu();
            break;
        case 1: // 购买食物
            std::cout << "请选择要购买的食物编号：\n";
            CanteenStaff::showMenu();
            int food_choice;
            std::cin >> food_choice;
            if (food_choice > 0 && food_choice <= static_cast<int>(food_types.size())) {
                std::cout << "购买了" << food_types[food_choice-1] << "，支付" 
                          << food_costs[food_choice-1] << "金币！\n";
                std::cout << "体力恢复：+" << stamina_recovery[food_choice-1] << "\n";
            } else {
                std::cout << "选择无效！\n";
            }
            break;
        case 2: // 为舍友买饭
            std::cout << "请选择要为舍友购买的食物：\n";
            CanteenStaff::showMenu();
            int roommate_food_choice;
            std::cin >> roommate_food_choice;
            if (roommate_food_choice > 0 && roommate_food_choice <= static_cast<int>(food_types.size())) {
                std::cout << "为舍友购买了" << food_types[roommate_food_choice-1] 
                          << "，支付" << food_costs[roommate_food_choice-1] << "金币！\n";
                std::cout << "体力恢复：+" << stamina_recovery[roommate_food_choice-1] << "\n";
                std::cout << "带饭成功！可以回去交给舍友了。\n";
            } else {
                std::cout << "选择无效！\n";
            }
            break;
    }
}
    
/**
 * @brief 显示菜单
 * */
void CanteenStaff::showMenu() const {
    std::cout << "=== 今日菜单 ===\n";
    for (size_t i = 0; i < food_types.size(); ++i) {
        std::cout << i+1 << ". " << food_types[i] << " - " << food_costs[i] 
                  << "金币 (+" << stamina_recovery[i] << "体力)\n";
    }
}

/**
 * @brief 教师类构造函数
 * */
Professor::Professor(const std::string& title, const std::string& subject)
        : NPC(title, "教师"), subject(subject) {
        setInteractionText("同学们好，我是" + subject + "课教师" + title + "。");
        addInteractionOption("请教问题");
        addInteractionOption("寻求指导");
    }
    
/**
 * @brief 处理与教师的交互
 * */
void Professor::handleInteraction(int option_index) const {
    switch (option_index) {
        case 0: // 请教问题
            std::cout << "向" << getTitle() << "教授请教问题。\n";
            break;
        case 1: // 寻求指导
            std::cout << "在" << getTitle() << "教授的指导下，智力得到了提升！\n";
            break;
    }
    //应完善
}

/**
 * @brief 教练员类构造函数
 * */
Coach::Coach(const std::string& title, const std::string& sport_type)
        : Professor(title, "体育"), sport_type(sport_type) {
    setInteractionText("同学好，我是体育老师。");
    addInteractionOption("参加训练");
        
    // 初始化运动项目数据
    Coach::initializeSportsData();
}
   
/**
 * @brief 处理与教练员的交互
 * */
void Coach::handleInteraction(int option_index) const {
    switch (option_index) {
        case 0:
            Coach::handleTrainingOptions();
            break;
    }
}
    
/**
 * @brief 处理训练项目选择
 */
void Coach::handleTrainingOptions() const {
    std::cout << "=== 训练项目 ===\n";
    for (const auto& sport : sports_data) {
        std::cout << sport.name << " - " << sport.time_cost << "小时，花费：";
        if (sport.cost == 0) {
            std::cout << "免费";
        } else {
            std::cout << sport.cost << "金币";
        }
        std::cout << "，体力+" << sport.stamina_gain << "\n";
    }
        
    std::cout << "请选择训练项目：\n";
    std::string choice;
    std::cin >> choice;
    
    for (const auto& sport : sports_data) {
        if (sport.name == choice) {
            // 检查金币是否足够（这里需要从主角类获取金币数量）
            int player_gold = 100; // 假设从主角类获取金币数量，这里用100作为示例
            
            std::cout << "您选择了" << choice << "训练！\n";
            std::cout << "将花费：" << sport.cost << "金币\n";
            std::cout << "将获得：体力+" << sport.stamina_gain << "\n";
            std::cout << "将消耗时间：" << sport.time_cost << "小时\n";
            
            if (player_gold >= sport.cost) {
                std::cout << "金币足够，训练成功！\n";
                Coach::handleTrainingReward(sport);
            } else {
                std::cout << "金币不足！需要" << sport.cost << "金币，但您只有" << player_gold << "金币。训练失败！\n";
            }
            return;
        }
    }
    std::cout << "无效的训练项目！\n";
}

/**
 * @brief 处理训练奖励
 * */
void Coach::handleTrainingReward(const Sport& sport) const {
    //从主角类中获取接口

    // 更新JSON文件中的器材拥有状态
    std::ifstream fileIn("SportsParam.json");
    if (!fileIn.is_open()) {
        std::cerr << "无法打开SportsParam.json文件进行读取" << std::endl;
        return;
    }
    
    try {
        json sports_json;
        fileIn >> sports_json;
        fileIn.close();
        
        // 根据运动类型更新器材拥有状态
        if (sport.name == "篮球" && sports_json.contains("basketball")) {
            sports_json["basketball"]["if_have_ball"] = true;
        } else if (sport.name == "足球" && sports_json.contains("football")) {
            sports_json["football"]["if_have_ball"] = true;
        } else if (sport.name == "羽毛球" && sports_json.contains("badminton")) {
            sports_json["badminton"]["if_have_bat"] = true;
        }
        
        // 写回JSON文件
        std::ofstream fileOut("SportsParam.json");
        if (!fileOut.is_open()) {
            std::cerr << "无法打开SportsParam.json文件进行写入" << std::endl;
            return;
        }
        
        fileOut << sports_json.dump(4); // 使用4个空格进行格式化
        fileOut.close();
        
        std::cout << "已成功获得" << sport.name << "器材！" << std::endl;
        
    } catch (const std::exception& e) {
        std::cerr << "JSON处理错误: " << e.what() << std::endl;
    }
}
    
/**
 * @brief 初始化运动项目数据
 * */
void Coach::initializeSportsData() {
    // 从JSON文件读取运动数据
    std::ifstream file("SportsParam.json");
    if (!file.is_open()) {
        std::cerr << "无法打开SportsParam.json文件, 使用默认数据" << std::endl;
        // 使用默认数据作为后备
        sports_data.push_back({"篮球", 0.5, 15, 5});
        sports_data.push_back({"足球", 1.0, 15, 8});
        sports_data.push_back({"跑步", 1.0, 0, 5});
        sports_data.push_back({"羽毛球", 0.5, 35, 10});
        return;
    }

    try {
        json sports_json;
        file >> sports_json;

        // 解析篮球数据
        if (sports_json.contains("basketball")) {
            auto& basketball = sports_json["basketball"];
            bool has_basketball = basketball["if_have_ball"].get<bool>();
            int cost = has_basketball ? 
                basketball["sum_money"]["if_have_ball"].get<int>() : 
                basketball["sum_money"]["if_not_have_ball"].get<int>();
            sports_data.push_back({"篮球", 
                                basketball["time"].get<float>(),
                                basketball["base_energy"].get<int>(),
                                cost});
        }

        // 解析足球数据
        if (sports_json.contains("football")) {
            auto& football = sports_json["football"];
            bool has_football = football["if_have_ball"].get<bool>();
            int cost = has_football ? 
                football["sum_money"]["if_have_ball"].get<int>() : 
                football["sum_money"]["if_not_have_ball"].get<int>();
            sports_data.push_back({"足球",
                                football["time"].get<float>(),
                                football["base_energy"].get<int>(),
                                cost});
        }

        // 解析跑步数据
        if (sports_json.contains("running")) {
            auto& running = sports_json["running"];
            sports_data.push_back({"跑步",
                                running["time"].get<float>(),   
                                running["base_energy"].get<int>(),
                                running["money"].get<int>()});
        }

        // 解析羽毛球数据
        if (sports_json.contains("badminton")) {
            auto& badminton = sports_json["badminton"];
            bool has_badminton_bat = badminton["if_have_bat"].get<bool>();
            int cost = has_badminton_bat ? 
                badminton["sum_money"]["if_have_bat"].get<int>() : 
                badminton["sum_money"]["if_not_have_bat"].get<int>();
            sports_data.push_back({"羽毛球",
                                badminton["time"].get<float>(),
                                badminton["base_energy"].get<int>(),
                                cost});
        }

    } catch (const std::exception& e) {
        std::cerr << "JSON解析错误: " << e.what() << "，使用默认数据" << std::endl;
        // 使用默认数据作为后备
        sports_data.push_back({"篮球", 0.5, 15, 5});
        sports_data.push_back({"足球", 1.0, 15, 8});
        sports_data.push_back({"跑步", 1.0, 0, 5});
        sports_data.push_back({"羽毛球", 0.5, 35, 10});
    }
}

/**
 * @brief 获取运动项目类型
 * @return 运动项目类型
 * */
std::string Coach::getSportType() const { return sport_type; }
