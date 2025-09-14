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
    std::filesystem::path scene_file;  //< 场景配置文件路径
   
    nlohmann::json scenes_json;  //< 存储解析后的JSON数据

    /**
     * @brief 默认构造函数
     */
    static std::shared_ptr<std::vector<std::string>> getScenes();

    /**
     * @brief 从文件加载 NPC 数据并获取具有指定场景属性的 NPC
     * @return 包含 NPC 名称和类型的映射的共享指针
     */
    std::shared_ptr<std::map<std::string, NPCType>> getNPCs();

    /**
     * @brief 从 JSON 对象中加载特殊字符NPC
     */
    std::shared_ptr<std::map<std::string, NPCType>> getSpecialCharNPCs();
    
    Scene() : name(""), scene_file(".config/scenes.json") {}
    
    /**
     * @brief 带参数的构造函数
     * @param scene_name 场景名称
     * @param file_path 场景配置文件路径
     */
    Scene(const std::string& scene_name, const std::filesystem::path& file_path);
    
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
     * @return 成功返回true，失败返回false
     */
    bool loadSceneFile();
    /**
     * @brief 根据出口键获取目标场景名称
     * @param key 出口id
     */
    std::string getSceneName(int key);
};
