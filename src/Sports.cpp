/**
 * @file Sports.cpp
 * @author LanTukai
 * @brief �˶�����ض��������
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
    * @brief ȷ��ѡ��
    * @return true ȷ��
    * @return false ȡ��
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
    * @brief ��ȡ�����ļ�
    * @param config_file �����ļ���
    * @param config JSON ��ʽ�����ļ�
    * */
    std::ifstream config_file("SportsParam.json");
    if (!config_file.is_open()) {
        std::cerr << "�޷��������ļ� SportsParam.json" << endl;
        return 1;
    }
    
    json config;
    config_file >> config;
    config_file.close();

    /**
    * @brief ��ȡ�˶������ļ�
    * @param file �˶������ļ���
    * @param line �˶������ļ��е�һ��
    * */
    std::ifstream file("SportsIntro.txt");
    std::string line;
    
    if (file.is_open()) {
        while (std::getline(file, line)) {
            std::cout << line << std::endl;
        }
        file.close();
    } else {
        std::cout << "�޷����ļ�" << std::endl;
    }

    /**
    * @brief ����������ʼ��
    * @param health ����ֵ
    * @param max_health ��󽡿�ֵ
    * */

    Parameter base_parameter;
    base_parameter.health = 100; // ����ֵ����ʱʹ�ã�����Ӧ�ӱ�����ȡ��

    const int max_health = (float)config["base"]["max_health"];

    /**
    * @brief �˶����Ͷ����ʼ��
    * @param time �˶�ʱ��
    * @param energy �˶���õ�����
    * @param health ����ֵ
    * @param money �˶����ĵĽ�Ǯ
    * @note ���������ܲ�����ë��
    * @note ʱ���ӦΪfloat����
    * */

    /**
    * @brief �����˶�
    * */
    Parameter basketball_parameter;
    basketball_parameter.time = (float)config["basketball"]["time"];
    basketball_parameter.energy = (float)config["basketball"]["base_energy"] * base_parameter.health / max_health;
    basketball_parameter.health = (float)base_parameter.health;
    
    /**
    * @brief �����Ƿ�ӵ������������
    * */
    bool has_basketball = config["basketball"]["if_have_ball"];
    if (has_basketball) {
        basketball_parameter.money = config["basketball"]["sum_money"]["if_have_ball"];
    } else {
        basketball_parameter.money = config["basketball"]["sum_money"]["if_not_have_ball"];
    }
    Sports basketball(basketball_parameter);

    /**
    * @brief �����˶�
    * */
    Parameter football_parameter;
    football_parameter.time = (float)config["football"]["time"];
    football_parameter.energy = (float)config["football"]["base_energy"] * base_parameter.health / max_health;
    football_parameter.health = (float)base_parameter.health;

    /**
    * @brief �����Ƿ�ӵ������������
    * */
    bool has_football = config["football"]["if_have_ball"];
    if (has_football) {
        football_parameter.money = config["football"]["sum_money"]["if_have_ball"];
    } else {
        football_parameter.money = config["football"]["sum_money"]["if_not_have_ball"];
    }
    Sports football(football_parameter);

    /**
    * @brief �ܲ��˶�
    * */
    Parameter running_parameter;
    running_parameter.time = (float)config["running"]["time"];
    running_parameter.energy = (float)config["running"]["base_energy"] * base_parameter.health / max_health;
    running_parameter.health = (float)base_parameter.health;
    running_parameter.money = (float)config["running"]["money"];
    Sports running(running_parameter);

    /**
    * @brief ��ë���˶�
    * */
    Parameter badminton_parameter;
    badminton_parameter.time = config["badminton"]["time"];
    badminton_parameter.energy = (float)config["badminton"]["base_energy"] * base_parameter.health / max_health;
    badminton_parameter.health = (float)base_parameter.health;

    /**
    * @brief �����Ƿ�ӵ����ë�����ļ������
    * */
    bool has_badminton_bat = (bool)config["badminton"]["if_have_bat"];
    if (has_badminton_bat) {
        badminton_parameter.money = (float)config["badminton"]["sum_money"]["if_have_bat"];
    } else {
        badminton_parameter.money = (float)config["badminton"]["sum_money"]["if_not_have_bat"];
    }
    Sports badminton(badminton_parameter);

    /**
    * @brief �������ԣ�Ӧ�ӱ�����ȡ
    * @param time �ҵ�ʱ��
    * @param energy �ҵ�����
    * @param health �ҵĽ���ֵ
    * @param money �ҵĽ��
    * */
    Parameter my_parameter;
    my_parameter.time = 100.0;
    my_parameter.energy = 100.0;
    my_parameter.health = 100.0;
    my_parameter.money = 100.0;



    /**
    * @brief �˶�ѡ��
    * */
    while (true) {
        std::cout << "��ѡ���˶���������ţ���" << std::endl;
        std::cout << "1. ����" << std::endl;
        std::cout << "2. ����" << std::endl;
        std::cout << "3. �ܲ�" << std::endl;
        std::cout << "4. ��ë��" << std::endl;
        int choice;
        std::cin >> choice;
        if (choice == 1) {
            std::cout << "��ѡ���������˶�" << std::endl;
            std::cout << "�㽫����ʱ�䣺" << basketball.getParameter().time << "Сʱ" << std::endl;
            std::cout << "�㽫���������" << basketball.getParameter().energy << std::endl;
            std::cout << "�㽫���Ľ�ң�" << basketball.getParameter().money << std::endl;
            std::cout << "�Ƿ�ȷ��(ȷ��������1):" << std::endl;
            if (ifSure()) {
                if (my_parameter.money >= basketball.getParameter().money) {
                    my_parameter.money -= basketball.getParameter().money;
                    my_parameter.energy += basketball.getParameter().energy;
                    my_parameter.time += basketball.getParameter().time;

                    std::cout << "�˶��ɹ���" << std::endl;
                    
                    /**
                    * @brief �޸�����Ϊ��ӵ�У�д���ļ�
                    * @param config �����ļ�
                    * @param out_file ����ļ�
                    * @note 4���ո�����
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
                    std::cout << "��Ҳ��㣬�˶�ʧ�ܣ�" << std::endl;
                }
            } else{
                std::cout << "��ȡ��" << std::endl;
            }
            /**
            * @brief �˳�ѭ��
            * */
            break;
        } else if (choice == 2) {
            std::cout << "��ѡ���������˶�" << std::endl;
            std::cout << "�㽫����ʱ�䣺" << football.getParameter().time << "Сʱ" << std::endl;
            std::cout << "�㽫���������" << football.getParameter().energy << std::endl;
            std::cout << "�㽫���Ľ�ң�" << football.getParameter().money << std::endl;
            std::cout << "�Ƿ�ȷ��(ȷ��������1):" << std::endl;
            if (ifSure()) {
                if (my_parameter.money >= football.getParameter().money) {
                    my_parameter.money -= football.getParameter().money;
                    my_parameter.energy += football.getParameter().energy;
                    my_parameter.time += football.getParameter().time;

                    std::cout << "�˶��ɹ���" << std::endl;

                    /**
                    * @brief �޸�����Ϊ��ӵ�У�д���ļ�
                    * @param config �����ļ�
                    * @param out_file ����ļ�
                    * @note 4���ո�����
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
                    std::cout << "��Ҳ��㣬�˶�ʧ�ܣ�" << std::endl;
                }
            } else{
                std::cout << "��ȡ��" << std::endl;
            }
            /**
            * @brief �˳�ѭ��
            * */
            break;
        } else if (choice == 3) {
            std::cout << "��ѡ�����ܲ��˶�" << std::endl;
            std::cout << "�㽫����ʱ�䣺" << running.getParameter().time << "Сʱ" << std::endl;
            std::cout << "�㽫���������" << running.getParameter().energy << std::endl;
            std::cout << "�㽫���Ľ�ң�" << running.getParameter().money << std::endl;
            std::cout << "�Ƿ�ȷ��(ȷ��������1):" << std::endl;
            if (ifSure()) {
                my_parameter.energy += running.getParameter().energy;
                my_parameter.time += running.getParameter().time;
                std::cout << "�˶��ɹ���" << std::endl;
            } else{
                std::cout << "��ȡ��" << std::endl;
            }
            /**
            * @brief �˳�ѭ��
            * */
            break;
        } else if (choice == 4) {
            std::cout << "��ѡ������ë���˶�" << std::endl;
            std::cout << "�㽫����ʱ�䣺" << badminton.getParameter().time << "Сʱ" << std::endl;
            std::cout << "�㽫���������" << badminton.getParameter().energy << std::endl;
            std::cout << "�㽫���Ľ�ң�" << badminton.getParameter().money << std::endl;
            std::cout << "�Ƿ�ȷ��(ȷ��������1):" << std::endl;
            if (ifSure()) {
                if (my_parameter.money >= badminton.getParameter().money) {
                    my_parameter.money -= badminton.getParameter().money;
                    my_parameter.energy += badminton.getParameter().energy;
                    my_parameter.time += badminton.getParameter().time;

                    std::cout << "�˶��ɹ���" << std::endl;

                    /**
                    * @brief �޸���ë����Ϊ��ӵ�У�д���ļ�
                    * @param config �����ļ�
                    * @param out_file ����ļ�
                    * @note 4���ո�����
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
                    std::cout << "��Ҳ��㣬�˶�ʧ�ܣ�" << std::endl;
                }
            } else{
                std::cout << "��ȡ��" << std::endl;
            }
            /**
            * @brief �˳�ѭ��
            * */
            break;
        } else {
            std::cout << "�����������������" << std::endl;
        }
    }
    
    return 0;
}
