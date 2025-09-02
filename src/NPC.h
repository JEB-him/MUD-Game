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
    virtual void startInteraction() const;

    /// @brief 处理具体交互选项（可被子类重写）
    virtual void handleInteraction(int optionIndex) const;

    /// @brief 获取称呼
    std::string getTitle() const;
    /// @brief 获取身份
    std::string getIdentity() const;
    /// @brief 获取交互文本
    std::string getInteractionText() const;
    /// @brief 获取交互选项
    std::vector<std::string> getInteractionOptions() const;

    /// @brief 设置称呼
    void setTitle(const std::string& newTitle);
    /// @brief 设置身份
    void setIdentity(const std::string& newIdentity);
    /// @brief 设置交互文本
    void setInteractionText(const std::string& text);
    /// @brief 添加交互选项
    void addInteractionOption(const std::string& option);

protected:
    std::string title; ///< 称呼
    std::string job; ///< 身份
    std::string interactionText; ///< 交互文本
    std::vector<std::string> interactionOptions; ///< 交互选项
};

/**
 * @brief 学生类
 * @param title 称呼
 * @param intelligence 智力
 * */
class Student : public NPC {
public:
    Student(const std::string& title, int intelligence = 50)
        : NPC(title, "学生"), intelligence(intelligence);

    void handleInteraction(int optionIndex) const override;

    /// @brief 获取智力
    int getIntelligence() const;
    /// @brief 设置智力
    void setIntelligence(int value);

protected:
    mutable int intelligence; ///< 智力
};

/**
 * @brief 学长类
 */
class SeniorStudent : public Student {
public:
    SeniorStudent(const std::string& title, int intelligence = 60, 
                 const std::string& major = "计算机")
        : Student(title, intelligence), major(major);

    void handleInteraction(int optionIndex) const override;

    /**
     * @brief 处理帮助请求
     * @details 模拟小游戏
     */
    void handleHelpRequest() const;

    std::string getMajor() const;
    void setMajor(const std::string& newMajor);

private:
    std::string major; ///< 专业方向（文科、理科、综合等）
};

/**
 * @brief 舍友类
 * */
class Roommate : public Student {
public:
    Roommate(const std::string& title, int intelligence = 50)
        : Student(title, intelligence);
        
    /**
     * @brief 完成玩家请室友带饭后交互内容
     */
    void completeMealRequest();
    /**
     * @brief 完成玩家帮室友带饭后交互内容
     */
    void completeMealProvide();

    /**
    * @brief 处理具体交互选项
    * @details 处理舍友类的具体交互选项，如开黑打游戏、请室友帮忙带饭、帮室友带饭
    * @param optionIndex 交互选项索引
    * */
    void handleInteraction(int optionIndex) const override;

    /**
     * @brief 获取带饭费用
     * @return 带饭费用
     */
    int getMealCost() const;


private:
    int mealCost; ///< 带饭费用
};

/**
 * @brief 宿舍管理员类
 * */
class DormManager : public NPC {
public:
    DormManager(const std::string& title)
        : NPC(title, "宿舍管理员"), familiarity(0);

    /**
     * @brief 处理对话
     * @details 与管理员的交互，增加熟悉度
     */
    void handleInteraction(int optionIndex) const override;

    /**
     * @brief 检查特殊对话
     * @details 当熟悉度超过50时，有30%的概率触发特殊对话
     */
    void checkSpecialDialogue() const;

    /**
     * @brief 获取熟悉度
     * @return 熟悉度
     */
    int getFamiliarity() const;

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
        : NPC(title, "招聘人员"), moneyReward(moneyReward), staminaCost(staminaCost);

    /**
     * @brief 处理与招聘人员的交互
     */
    void handleInteraction(int optionIndex) const override;

private:
    int moneyReward; ///< 工作金币奖励
    int staminaCost; ///< 工作体力消耗
};

/**
 * @brief 图书管理员类
 * */
class Librarian : public NPC {
public:
    Librarian(const std::string& title)
        : NPC(title, "图书管理员");

    /**
     * @brief 处理与图书管理员的交互
     * */
    void handleInteraction(int optionIndex) const override;

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
          staminaRecovery(staminaRecovery), foodCosts(foodCosts);

    /**
     * @brief 处理与食堂工作人员的交互
     */
    void handleInteraction(int optionIndex) const override;
    
    /**
     * @brief 显示菜单
     */
    void showMenu() const;

private:
    std::vector<std::string> foodTypes; ///< 食物类型
    std::vector<int> staminaRecovery; ///< 恢复体力值
    std::vector<int> foodCosts; ///< 食物价格
};

/**
 * @brief 教师类
 * */
class Professor : public NPC {
public:
    Professor(const std::string& title, const std::string& subject)
        : NPC(title, "教师"), subject(subject);
    /**
     * @brief 处理与教师的交互
     */
    void handleInteraction(int optionIndex) const override;

private:
    std::string subject; ///< 科目
};

/**
 * @brief 教练员类
 * */
class Coach : public Professor {
public:
    Coach(const std::string& title, const std::string& sportType)
        : Professor(title, "体育"), sportType(sportType);
    
    /**
     * @brief 处理与教练员的交互
     */
    void handleInteraction(int optionIndex) const override;
    
    /**
     * @brief 处理训练项目选择
     */
    void handleTrainingOptions() const;
    
    /**
     * @brief 处理训练奖励
     */
    void handleTrainingReward(const Sport& sport) const;
    
    /**
     * @brief 初始化运动项目数据
     */
    void initializeSportsData();

    /**
     * @brief 获取运动项目类型
     * @return 运动项目类型
     */
    std::string getSportType() const;

private:
    struct SportData {     ///< 运动项目数据结构体
        std::string name;  // 运动项目名称
        double timeCost;   // 消耗时间（小时）
        int cost;          // 总开销（金币）
        int staminaGain;   // 体力收益
    };
    
    std::string sportType; ///< 运动类型
    std::vector<SportData> sportsData; ///< 运动项目数据
};