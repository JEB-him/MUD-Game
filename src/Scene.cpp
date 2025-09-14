#include "Scene.h"
#include <iostream>

Scene::Scene() : name(""), scene_file(".config/scenes.json") {
    // 默认构造函数
}

Scene::Scene(const std::string& scene_name, const std::filesystem::path& file_path) 
    : name(scene_name), scene_file(file_path) {
        if(loadSceneFile()){
            loadExits();
        } else {
            std::cerr << "错误: 无法加载场景文件: " << scene_file << std::endl;
        }
    }

bool Scene::loadSceneFile() {
    std::ifstream file(scene_file);
    if (!file.is_open()) {
        std::cerr << "错误: 无法打开场景文件: " << scene_file << std::endl;
        return false;
    }
    try {
        file >> scenes_json;
        file.close();
        return true;
    } catch (const std::exception& e) {
        std::cerr << "错误: 解析场景文件时出错 - " << e.what() << std::endl;
        file.close();
        return false;
    }
}

void Scene::loadExits() {
    // 清空现有出口
    exits.clear();
    
    // 确保已加载场景数据
    if (scenes_json.empty()) {
        if (!loadSceneFile()) {
            std::cerr << "错误: 无法加载场景数据" << std::endl;
            return;
        }
    }
    
    // 查找当前场景的出口
    if (scenes_json.contains(name)) {
        const auto& exits_obj = scenes_json[name];
        for (auto it = exits_obj.begin(); it != exits_obj.end(); ++it) {
            try {
                int exit_id = std::stoi(it.key());
                std::string target_scene = it.value().get<std::string>();
                
                // 将出口信息添加到exits映射中
                exits[exit_id] = target_scene;
            } catch (const std::exception& e) {
                std::cerr << "警告: 处理出口时出错 - " << e.what() << std::endl;
            }
        }
    } else {
        std::cerr << "警告: 未找到场景 '" << name << "' 的配置" << std::endl;
    }
}

std::string Scene::getSceneName(int key) {
    auto it = exits.find(key);
    if (it != exits.end()) {
        return it->second;
    } else {
        return ""; // 或者返回一个默认值，表示未找到
    }
}