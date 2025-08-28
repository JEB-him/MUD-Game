/**
 * @file Sports.cpp
 * @author LanTukai
 * @brief 运动场相关对象与操作
 * @note g++ SportsParam.json Sports.cpp -o Sports.exe
 * */

#include "Sports.h"
#include <iostream>
#include <fstream>
#include <string>
#include "json.hpp"
using std::cout;
using std::endl;
using std::cin;
using json = nlohmann::json;

bool ifSure() {
    /**
    * @brief 确认选择
    * @return true 确认
    * @return false 取消
    * */
    int choice_sure = 0;
    std::cin >> choice_sure;
    if (choice_sure == 1) {
        return true;
    } else {
        return false;
    }
}

int main() {

    /**
    * @brief 读取配置文件
    * @param config_file 配置文件流
    * @param config JSON 格式配置文件
    * */
    std::ifstream config_file("SportsParam.json");
    if (!config_file.is_open()) {
        std::cerr << "无法打开配置文件 SportsParam.json" << endl;
        return 1;
    }
    
    json config;
    config_file >> config;
    config_file.close();

    /**
    * @brief 读取运动介绍文件
    * @param file 运动介绍文件流
    * @param line 运动介绍文件中的一行
    * */
    std::ifstream file("SportsIntro.txt");
    std::string line;
    
    if (file.is_open()) {
        while (std::getline(file, line)) {
            std::cout << line << std::endl;
        }
        file.close();
    } else {
        std::cout << "无法打开文件" << std::endl;
    }

    /**
    * @brief 基础参数初始化
    * @param health 健康值
    * @param max_health 最大健康值
    * */

    Parameter base_parameter;
    base_parameter.health = 100; // 健康值（暂时使用，后续应从背包读取）

    const int max_health = (float)config["base"]["max_health"];

    /**
    * @brief 运动类型对象初始化
    * @param time 运动时间
    * @param energy 运动获得的体力
    * @param health 健康值
    * @param money 运动消耗的金钱
    * @note 篮球，足球，跑步，羽毛球
    * @note 时间等应为float类型
    * */

    /**
    * @brief 篮球运动
    * */
    Parameter basketball_parameter;
    basketball_parameter.time = (float)config["basketball"]["time"];
    basketball_parameter.energy = (float)config["basketball"]["base_energy"] * base_parameter.health / max_health;
    basketball_parameter.health = (float)base_parameter.health;
    
    /**
    * @brief 根据是否拥有篮球计算费用
    * */
    bool has_basketball = config["basketball"]["if_have_ball"];
    if (has_basketball) {
        basketball_parameter.money = config["basketball"]["sum_money"]["if_have_ball"];
    } else {
        basketball_parameter.money = config["basketball"]["sum_money"]["if_not_have_ball"];
    }
    Sports basketball(basketball_parameter);

    /**
    * @brief 足球运动
    * */
    Parameter football_parameter;
    football_parameter.time = (float)config["football"]["time"];
    football_parameter.energy = (float)config["football"]["base_energy"] * base_parameter.health / max_health;
    football_parameter.health = (float)base_parameter.health;

    /**
    * @brief 根据是否拥有足球计算费用
    * */
    bool has_football = config["football"]["if_have_ball"];
    if (has_football) {
        football_parameter.money = config["football"]["sum_money"]["if_have_ball"];
    } else {
        football_parameter.money = config["football"]["sum_money"]["if_not_have_ball"];
    }
    Sports football(football_parameter);

    /**
    * @brief 跑步运动
    * */
    Parameter running_parameter;
    running_parameter.time = (float)config["running"]["time"];
    running_parameter.energy = (float)config["running"]["base_energy"] * base_parameter.health / max_health;
    running_parameter.health = (float)base_parameter.health;
    running_parameter.money = (float)config["running"]["money"];
    Sports running(running_parameter);

    /**
    * @brief 羽毛球运动
    * */
    Parameter badminton_parameter;
    badminton_parameter.time = config["badminton"]["time"];
    badminton_parameter.energy = (float)config["badminton"]["base_energy"] * base_parameter.health / max_health;
    badminton_parameter.health = (float)base_parameter.health;

    /**
    * @brief 根据是否拥有羽毛球球拍计算费用
    * */
    bool has_badminton_bat = (bool)config["badminton"]["if_have_bat"];
    if (has_badminton_bat) {
        badminton_parameter.money = (float)config["badminton"]["sum_money"]["if_have_bat"];
    } else {
        badminton_parameter.money = (float)config["badminton"]["sum_money"]["if_not_have_bat"];
    }
    Sports badminton(badminton_parameter);

    /**
    * @brief 自身属性，应从背包读取
    * @param time 我的时间
    * @param energy 我的体力
    * @param health 我的健康值
    * @param money 我的金币
    * */
    Parameter my_parameter;
    my_parameter.time = 100.0;
    my_parameter.energy = 100.0;
    my_parameter.health = 100.0;
    my_parameter.money = 100.0;



    /**
    * @brief 运动选择
    * */
    while (true) {
        std::cout << "请选择运动（输入序号）：" << std::endl;
        std::cout << "1. 篮球" << std::endl;
        std::cout << "2. 足球" << std::endl;
        std::cout << "3. 跑步" << std::endl;
        std::cout << "4. 羽毛球" << std::endl;
        int choice;
        std::cin >> choice;
        if (choice == 1) {
            std::cout << "你选择了篮球运动" << std::endl;
            std::cout << "你将消耗时间：" << basketball.getParameter().time << "小时" << std::endl;
            std::cout << "你将获得体力：" << basketball.getParameter().energy << std::endl;
            std::cout << "你将消耗金币：" << basketball.getParameter().money << std::endl;
            std::cout << "是否确认(确认请输入1):" << std::endl;
            if (ifSure()) {
                if (my_parameter.money >= basketball.getParameter().money) {
                    my_parameter.money -= basketball.getParameter().money;
                    my_parameter.energy += basketball.getParameter().energy;
                    my_parameter.time += basketball.getParameter().time;

                    std::cout << "运动成功！" << std::endl;
                    
                    /**
                    * @brief 修改篮球为已拥有，写回文件
                    * @param config 配置文件
                    * @param out_file 输出文件
                    * @note 4个空格缩进
                    * */
                    json config;
                    std::ifstream config_file("SportsParam.json");
                    config_file >> config;
                    config_file.close();
                    config["basketball"]["if_have_ball"] = true;
                    std::ofstream out_file("SportsParam.json");
                    out_file << config.dump(4); 
                    out_file.close();

                } else {
                    std::cout << "金币不足，运动失败！" << std::endl;
                }
            } else{
                std::cout << "已取消" << std::endl;
            }
            /**
            * @brief 退出循环
            * */
            break;
        } else if (choice == 2) {
            std::cout << "你选择了足球运动" << std::endl;
            std::cout << "你将消耗时间：" << football.getParameter().time << "小时" << std::endl;
            std::cout << "你将获得体力：" << football.getParameter().energy << std::endl;
            std::cout << "你将消耗金币：" << football.getParameter().money << std::endl;
            std::cout << "是否确认(确认请输入1):" << std::endl;
            if (ifSure()) {
                if (my_parameter.money >= football.getParameter().money) {
                    my_parameter.money -= football.getParameter().money;
                    my_parameter.energy += football.getParameter().energy;
                    my_parameter.time += football.getParameter().time;

                    std::cout << "运动成功！" << std::endl;

                    /**
                    * @brief 修改足球为已拥有，写回文件
                    * @param config 配置文件
                    * @param out_file 输出文件
                    * @note 4个空格缩进
                    * */
                    json config;
                    std::ifstream config_file("SportsParam.json");
                    config_file >> config;
                    config_file.close();
                    config["football"]["if_have_ball"] = true;
                    std::ofstream out_file("SportsParam.json");
                    out_file << config.dump(4);
                    out_file.close();

                } else {
                    std::cout << "金币不足，运动失败！" << std::endl;
                }
            } else{
                std::cout << "已取消" << std::endl;
            }
            /**
            * @brief 退出循环
            * */
            break;
        } else if (choice == 3) {
            std::cout << "你选择了跑步运动" << std::endl;
            std::cout << "你将消耗时间：" << running.getParameter().time << "小时" << std::endl;
            std::cout << "你将获得体力：" << running.getParameter().energy << std::endl;
            std::cout << "你将消耗金币：" << running.getParameter().money << std::endl;
            std::cout << "是否确认(确认请输入1):" << std::endl;
            if (ifSure()) {
                my_parameter.energy += running.getParameter().energy;
                my_parameter.time += running.getParameter().time;
                std::cout << "运动成功！" << std::endl;
            } else{
                std::cout << "已取消" << std::endl;
            }
            /**
            * @brief 退出循环
            * */
            break;
        } else if (choice == 4) {
            std::cout << "你选择了羽毛球运动" << std::endl;
            std::cout << "你将消耗时间：" << badminton.getParameter().time << "小时" << std::endl;
            std::cout << "你将获得体力：" << badminton.getParameter().energy << std::endl;
            std::cout << "你将消耗金币：" << badminton.getParameter().money << std::endl;
            std::cout << "是否确认(确认请输入1):" << std::endl;
            if (ifSure()) {
                if (my_parameter.money >= badminton.getParameter().money) {
                    my_parameter.money -= badminton.getParameter().money;
                    my_parameter.energy += badminton.getParameter().energy;
                    my_parameter.time += badminton.getParameter().time;

                    std::cout << "运动成功！" << std::endl;

                    /**
                    * @brief 修改羽毛球拍为已拥有，写回文件
                    * @param config 配置文件
                    * @param out_file 输出文件
                    * @note 4个空格缩进
                    * */
                    json config;
                    std::ifstream config_file("SportsParam.json");
                    config_file >> config;
                    config_file.close();
                    config["badminton"]["if_have_bat"] = true;
                    std::ofstream out_file("SportsParam.json");
                    out_file << config.dump(4);
                    out_file.close();

                } else {
                    std::cout << "金币不足，运动失败！" << std::endl;
                }
            } else{
                std::cout << "已取消" << std::endl;
            }
            /**
            * @brief 退出循环
            * */
            break;
        } else {
            std::cout << "输入错误，请重新输入" << std::endl;
        }
    }
    
    return 0;
}
