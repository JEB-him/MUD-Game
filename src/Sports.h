/**
 * @file Sports.h
 * @author LanTukai
 * @brief 运动类
 * */

/**
 * @brief 运动参数类
 * @param time 运动时间
 * @param energy 运动获得的体力
 * @param health 健康值
 * @param money 运动消耗的金钱
 * @note 编译命令 g++ -o Sports Sports.cpp -std=c++11  && ./Sports
 * */

class Parameter {
public:
    float time;
    float energy;
    float health;
    float money;
};

class Sports {
public:
    // Sports() = default;
    Sports(Parameter parameter);
    Parameter getParameter() const;
    void setParameter(Parameter parameter);
private:
    Parameter parameter;

};

/**
 * @brief 带参构造函数
 * @param param 运动参数
 * */
Sports::Sports(Parameter param) : parameter(param) {
    /**
     * @brief 初始化参数
     * @param param 运动参数
     * */
    Sports::setParameter(param);
}

/**
 * @brief 获取参数
 * @return 运动参数
 * */

Parameter Sports::getParameter() const {
    return parameter;
}

/**
 * @brief 设置参数
 * @param param 运动参数
 * */
void Sports::setParameter(Parameter param) {
    parameter = param;
}

