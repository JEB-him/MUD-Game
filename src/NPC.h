/**
 * @brief 非玩家角色类
 * @details   身份一览：学生Student、舍友Roommate(继承于学生)、宿舍管理员DormManager 、
          招聘人员Employer or 兼职中介PartTimeAgent 、食堂工作人员CanteenStaff 、
          教师Professor 、教练员Coach、图书管理员Librarian 等
 * */

#include <iostream>
#include <memory> ///< 智能指针
#include <string>
#include <vector>
#include <random>

/**
 * @brief NPC基类
 */
class NPC {
public:
    NPC(const std::string& title, const std::string& job)
        : title(title), job(job) {}

    virtual ~NPC() = default;

    /// @brief 树状交互逻辑入口
    virtual void startInteraction() const {
    std::cout << interactionText << "\n";
    
    if (interactionOptions.empty()) {
        std::cout << "暂时没有可交互的选项。\n";
        return;
    }
    
    // 显示一级交互选项
    std::cout << "请选择交互选项：\n";
    for (size_t i = 0; i < static_cast<int>(interactionOptions.size()); ++i) {
        std::cout << i + 1 << ". " << interactionOptions[i] << "\n";
    }
    std::cout << "0. 退出交互\n";
    
    int choice;
    while (true) {
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
        
        if (choice > 0 && choice <= static_cast<int>(interactionOptions.size())) {
            handleInteraction(choice - 1);
            return;
        } else {
            std::cout << "无效的选择，请重新选择：\n";
        }
    }
}

    /// @brief 处理具体交互选项（可被子类重写）
    virtual void handleInteraction(int optionIndex) const {
        std::cout << "你选择了：" << interactionOptions[optionIndex] << "\n";
        // 基类默认处理，子类可以重写实现具体逻辑
        std::cout << "这个功能还在开发中。\n";
    }

    /// @brief 获取称呼
    std::string getTitle() const { return title; }
    /// @brief 获取身份
    std::string getIdentity() const { return job; }
    /// @brief 获取交互文本
    std::string getInteractionText() const { return interactionText; }
    /// @brief 获取交互选项
    std::vector<std::string> getInteractionOptions() const { return interactionOptions; }

    /// @brief 设置称呼
    void setTitle(const std::string& newTitle) { title = newTitle; }
    /// @brief 设置身份
    void setIdentity(const std::string& newIdentity) { job = newIdentity; }
    /// @brief 设置交互文本
    void setInteractionText(const std::string& text) { interactionText = text; }
    /// @brief 添加交互选项
    void addInteractionOption(const std::string& option) { interactionOptions.push_back(option); }

protected:
    std::string title; ///< 称呼
    std::string job; ///< 身份
    std::string interactionText; ///< 交互文本
    std::vector<std::string> interactionOptions; ///< 交互选项
};

/**
 * @brief 学生类
 * */
class Student : public NPC {
public:
    Student(const std::string& title, int intelligence = 50)
        : NPC(title, "学生"), intelligence(intelligence) {
        setInteractionText("你好，我是学生" + title + "，我们一起学习吧！");
        addInteractionOption("请教问题");
    }

    void handleInteraction(int optionIndex) const override {
        if (optionIndex == 0) { // 请教问题
            std::cout << "你向" << getTitle() << "请教问题。\n";
            // 需增加提升智力处理代码，需向主角类获取接口
            
        }
    }

    int getIntelligence() const { return intelligence; } ///< 获取智力
    void setIntelligence(int value) { intelligence = value; } ///< 设置智力

private:
    int intelligence;
};

/**
 * @brief 学长类
 */
class SeniorStudent : public Student {
public:
    SeniorStudent(const std::string& title, int intelligence = 60, 
                 const std::string& major = "计算机")
        : Student(title, intelligence), major(major) {
        setIdentity("学长");
        setInteractionText("你好，我是" + major + "专业的学长" + title + "，最近有点忙，能帮帮我吗？");
        addInteractionOption("帮助处理问题");
    }

    void handleInteraction(int optionIndex) const override {
        if (optionIndex == 0) { // 帮助处理问题
            handleHelpRequest();
        }
    }

    /**
     * @brief 处理帮助请求
     * @details 模拟小游戏 - 简单的数学题
     */
    void handleHelpRequest() const {
        std::cout << getTitle() << "：太好了！我这里有个问题需要帮忙处理。\n";
        
        // 模拟小游戏 - 简单的数学题
        std::cout << "=== 小游戏：解决学长的问题 ===\n";
        std::cout << "请计算：25 + 17 = ?\n";
        
        int answer;
        std::cin >> answer;
        
        if (answer == 42) {
            std::cout << "恭喜你答对了！学长的问题解决了。\n";
            std::cout << "奖励：智力文理+" << 10 << "\n";
            std::cout << "消耗：时间+1小时,体力-15\n";
            
            // 这里需要调用主角类的接口来实际修改属性
        } else {
            std::cout << "答案错误，再仔细想想吧！\n";
            std::cout << "消耗：时间+0.5小时，体力-5\n";

            // 这里需要调用主角类的接口来实际修改属性    
        }
    }

    std::string getMajor() const { return major; }
    void setMajor(const std::string& newMajor) { major = newMajor; }

private:
    std::string major; ///< 专业方向（文科、理科、综合等）
};

/**
 * @brief 舍友类
 * */
class Roommate : public Student {
public:
    Roommate(const std::string& title, int intelligence = 50)
        : Student(title, intelligence) {
        setIdentity("舍友");
        setInteractionText("嘿，" + title + "在这里！需要帮忙吗？");
        addInteractionOption("开黑打游戏");
        addInteractionOption("请室友帮忙带饭");  // 玩家请室友带饭
        addInteractionOption("帮室友带饭");      // 玩家帮室友带饭
        // hasHelpedWithMeal = false;
        // hasBoughtMeal = false;
        mealCost = 10; // 带饭费用
    }

    /**
     * @brief 完成玩家请室友带饭后交互内容
     */
    void completeMealRequest() { 
        std::cout << getTitle() << "：这是你的饭！\n";
    }
    
    /**
     * @brief 完成玩家帮室友带饭后交互内容
     */
    void completeMealProvide() { 
        std::cout << getTitle() << "：谢谢你帮我带饭！下次再麻烦你！\n";
    }

    void handleInteraction(int optionIndex) const override {
        switch (optionIndex) {
            case 0: // 开黑打游戏
                std::cout << getTitle() << "：来来来，开黑打游戏！\n";
                // 需增加打游戏收益处理代码 
                break;
            
            case 1: // 请室友帮忙带饭（玩家请室友带饭）
                std::cout << getTitle() << "：没问题！帮你带饭需要支付跑腿费" << mealCost << "金币。确认吗？(y/n)\n";
                char confirm1;
                std::cin >> confirm1;
                if (confirm1 == 'y' || confirm1 == 'Y') {
                    std::cout << "已支付" << mealCost << "金币！" << getTitle() << "会去食堂帮你带饭。\n";
                    std::cout << "(转向食堂工作人员选择菜品...)\n";
                    // 需增加带饭收益处理代码
                    // const_cast<Roommate*>(this)->hasBoughtMeal = true;
                } else {
                    std::cout << "取消带饭请求。\n";
                }
                break;
            
            case 2: // 帮室友带饭（玩家帮室友带饭）
                std::cout << getTitle() << "：太好了！你要帮我带饭吗？我会支付你跑腿费" << mealCost << "金币。确认吗？(y/n)\n";
                char confirm2;
                std::cin >> confirm2;
                if (confirm2 == 'y' || confirm2 == 'Y') {
                    std::cout << getTitle() << "支付了" << mealCost << "金币！请去食堂帮我带饭吧。\n";
                    std::cout << "(转向食堂工作人员选择菜品...)\n";
                    // 需增加带饭收益处理代码
                    // const_cast<Roommate*>(this)->hasHelpedWithMeal = true;
                } else {
                    std::cout << "取消带饭帮助。\n";
                }
                break;
            }
    }
    // void setBoughtMeal(bool bought) { hasBoughtMeal = bought; }
    // bool hasHelpedWithMealStatus() const { return hasHelpedWithMeal; }
    // bool hasBoughtMealStatus() const { return hasBoughtMeal; }
    int getMealCost() const { return mealCost; }

    // // 重置带饭状态
    // void resetMealStatus() {
    //     hasHelpedWithMeal = false;
    //     hasBoughtMeal = false;
    // }

private:
    // mutable bool hasHelpedWithMeal;  // 玩家帮室友带饭的状态
    // mutable bool hasBoughtMeal;      // 室友帮玩家带饭的状态
    int mealCost; // 带饭费用
};

/**
 * @brief 宿舍管理员类
 * */
class DormManager : public NPC {
public:
    DormManager(const std::string& title)
        : NPC(title, "宿舍管理员"), familiarity(0) {
        setInteractionText("我是宿舍管理员" + title + "，有什么需要帮忙的吗？");
        addInteractionOption("对话");
        addInteractionOption("帮忙干活");
    }

    /**
     * @brief 处理对话
     * @details 与管理员的交互，增加熟悉度
     */
    void handleInteraction(int optionIndex) const override {
        switch (optionIndex) {
            case 0: // 对话
                const_cast<DormManager*>(this)->familiarity += 5;
                std::cout << "与" << getTitle() << "对话，熟悉度提升到：" << familiarity << "\n";
                checkSpecialDialogue();
                break;
            case 1: // 帮忙干活
                const_cast<DormManager*>(this)->familiarity += 10;
                std::cout << "帮忙干活，熟悉度提升到：" << familiarity << "\n";
                checkSpecialDialogue();
                break;
        }
    }

    /**
     * @brief 检查特殊对话
     * @details 当熟悉度超过50时，有30%的概率触发特殊对话
     */
    void checkSpecialDialogue() const {
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

    int getFamiliarity() const { return familiarity; }

private:
    mutable int familiarity; //熟悉度，应该储存到文件中
};

/**
 * @brief 招聘人员类
 * */
class Employer : public NPC {
public:
    Employer(const std::string& title, int moneyReward, int staminaCost, 
             const std::string& promotionText)
        : NPC(title, "招聘人员"), moneyReward(moneyReward), staminaCost(staminaCost) {
        setInteractionText(promotionText);
        addInteractionOption("接受工作");
        addInteractionOption("询问详情");
    }

    /**
     * @brief 处理与招聘人员的交互
     */
    void handleInteraction(int optionIndex) const override {
        switch (optionIndex) {
            case 0: // 接受工作
                std::cout << "你接受了工作，获得" << moneyReward << "金币，消耗" << staminaCost << "体力。\n";
                break;
            case 1: // 询问详情
                std::cout << "工作详情：奖励" << moneyReward << "金币，需要消耗" << staminaCost << "体力。\n";
                break;
        }
        //应完善
    }

private:
    int moneyReward;
    int staminaCost;
};

/**
 * @brief 图书管理员类
 * */
class Librarian : public NPC {
public:
    Librarian(const std::string& title)
        : NPC(title, "图书管理员") {
        setInteractionText("欢迎来到图书馆，我是图书管理员" + title + "，需要帮助吗？");
        addInteractionOption("寻求知识帮助");
    }

    void handleInteraction(int optionIndex) const override {
        if (optionIndex == 0) {
            std::cout << "在" << getTitle() << "的帮助下，你的智力得到了提升！\n";
        }
    }
};

/**
 * @brief 食堂工作人员类
 * */
class CanteenStaff : public NPC {
public:
    CanteenStaff(const std::string& title, 
                const std::vector<std::string>& foodTypes,
                const std::vector<int>& staminaRecovery,
                const std::vector<int>& foodCosts)
        : NPC(title, "食堂工作人员"), foodTypes(foodTypes), 
          staminaRecovery(staminaRecovery), foodCosts(foodCosts) {
        setInteractionText("欢迎光临食堂！我是" + title + "，今天想吃点什么？");
        addInteractionOption("查看菜单");
        addInteractionOption("购买食物");
        addInteractionOption("为舍友买饭");
    }

    /**
     * @brief 处理与食堂工作人员的交互
     */
    void handleInteraction(int optionIndex) const override {
        switch (optionIndex) {
            case 0: // 查看菜单
                showMenu();
                break;
            case 1: // 购买食物
                std::cout << "请选择要购买的食物编号：\n";
                showMenu();
                int foodChoice;
                std::cin >> foodChoice;
                if (foodChoice > 0 && foodChoice <= static_cast<int>(foodTypes.size())) {
                    std::cout << "购买了" << foodTypes[foodChoice-1] << "，支付" 
                              << foodCosts[foodChoice-1] << "金币！\n";
                    std::cout << "体力恢复：+" << staminaRecovery[foodChoice-1] << "\n";
                } else {
                    std::cout << "选择无效！\n";
                }
                break;
            case 2: // 为舍友买饭
                std::cout << "请选择要为舍友购买的食物：\n";
                showMenu();
                int roommateFoodChoice;
                std::cin >> roommateFoodChoice;
                if (roommateFoodChoice > 0 && roommateFoodChoice <= static_cast<int>(foodTypes.size())) {
                    std::cout << "为舍友购买了" << foodTypes[roommateFoodChoice-1] 
                              << "，支付" << foodCosts[roommateFoodChoice-1] << "金币！\n";
                    std::cout << "带饭成功！可以回去交给舍友了。\n";
                } else {
                    std::cout << "选择无效！\n";
                }
                break;
        }
    }
    
    /**
     * @brief 显示菜单
     */
    void showMenu() const {
        std::cout << "=== 今日菜单 ===\n";
        for (size_t i = 0; i < foodTypes.size(); ++i) {
            std::cout << i+1 << ". " << foodTypes[i] << " - " << foodCosts[i] 
                      << "金币 (+" << staminaRecovery[i] << "体力)\n";
        }
    }

private:
    std::vector<std::string> foodTypes;
    std::vector<int> staminaRecovery;
    std::vector<int> foodCosts;
};

/**
 * @brief 教师类
 * */
class Professor : public NPC {
public:
    Professor(const std::string& title, const std::string& subject)
        : NPC(title, "教师"), subject(subject) {
        setInteractionText("同学们好，我是" + subject + "课教师" + title + "。");
        addInteractionOption("请教问题");
        addInteractionOption("寻求指导");
    }
    
    /**
     * @brief 处理与教师的交互
     */
    void handleInteraction(int optionIndex) const override {
        switch (optionIndex) {
            case 0: // 请教问题
                std::cout << "向" << getTitle() << "教授请教问题。\n";
                break;
            case 1: // 寻求指导
                std::cout << "在" << getTitle() << "教授的指导下，智力得到了提升！\n";
                break;
        }
        //应完善
    }

private:
    std::string subject;
};

/**
 * @brief 教练员类
 * */
class Coach : public Professor {
public:
    Coach(const std::string& title, const std::string& sportType)
        : Professor(title, "体育"), sportType(sportType) {
        setInteractionText("同学好，我是体育老师。");
        addInteractionOption("参加训练");
        
        // 初始化运动项目数据
        initializeSportsData();
    }
    
    /**
     * @brief 处理与教练员的交互
     */
    void handleInteraction(int optionIndex) const override {
        switch (optionIndex) {
            case 0:
                handleTrainingOptions();
                //需添加收益处理函数
                handleTrainingReward();
                break;
        }
    }
    
    /**
     * @brief 处理训练项目选择
     */
    void handleTrainingOptions() const {
        std::cout << "=== 训练项目 ===\n";
        for (const auto& sport : sportsData) {
            std::cout << sport.name << " - " << sport.timeCost << "小时，花费：";
            if (sport.cost == 0) {
                std::cout << "免费\n";
            } else {
                std::cout << sport.cost << "金币\n";
            }
        }
        
        std::cout << "请选择训练项目：\n";
        std::string choice;
        std::cin >> choice;
        
        for (const auto& sport : sportsData) {
            if (sport.name == choice) {
                std::cout << "参加了" << choice << "训练！体力+" << sport.staminaGain << "!\n";
                return;
            }
        }
        std::cout << "无效的训练项目！\n";
    }
    
    /**
     * @brief 处理训练奖励
     */
    void handleTrainingReward() const {
        //从主角类中获取接口
    }
    
    /**
     * @brief 初始化运动项目数据
     */
    void initializeSportsData() {
        // 篮球
        sportsData.push_back({"篮球", 0.5, 15, 5}); // 场地费5 + 篮球10
        // 足球  
        sportsData.push_back({"足球", 1.0, 15, 8}); // 足球15
        // 跑步
        sportsData.push_back({"跑步", 1.0, 0, 5}); // 免费
        // 羽毛球
        sportsData.push_back({"羽毛球", 0.5, 35, 10}); // 场地费5 + 羽毛球10 + 球拍20
    }

    std::string getSportType() const { return sportType; }

private:
    struct SportData {
        std::string name;
        double timeCost;    // 消耗时间（小时）
        int cost;           // 总开销（金币）
        int staminaGain;    // 体力收益
    };
    
    std::string sportType;
    std::vector<SportData> sportsData;
};