/**
 * @file Scene.h
 * @author Jie Jiang 
 */
#pragma once
#include <vector>
#include <memory>
#include <string>
#include <map>
#include <fstream>
#include <filesystem>
#include "json.hpp"

#include "NPCType.h"
#include "Controller.h"

/**
 * @brief 场景类    
 * @details 
 */
class Scene {
public:
    std::string name;   //< 场景名称
    std::map<int, std::string> exits;  //< 场景出口映射
    std::filesystem::path scene_file=".config/";  //< 场景配置文件路径
    
    Scene();
    
    /**
     * @brief 带参数的构造函数
     * @param scene_name 场景名称
     */
    Scene(const std::string& scene_name);
    
    /**
     * @brief 析构函数
     */
    ~Scene();

    /**
     * @brief 获取当前场景的出口信息
     */
    void loadExits();
    
    /**
     * @brief 加载场景配置文件
     * @param fi 文件路径
     * @return 成功返回true，失败返回false
     */
    bool loadSceneFile(std::filesystem::path fi);
    
    /**
     * @brief 根据出口键获取目标场景名称
     * @param key 出口id
     */
    std::string getSceneName(int key);

    /**
     * @brief 根据特殊字符获取NPC ID
     * @param specialChar NPC的特殊字符标识
     * @return NPC的ID字符串，如果未找到则返回空字符串
     */
    std::string getNPCname(const char& specialChar);
};