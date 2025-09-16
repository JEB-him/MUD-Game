#include "Scene.h"
#include <exception>
#include <iostream>
#include "Controller.h"

using json = nlohmann::json;

#include <fstream>

Scene::Scene() : name("Canteen") {
        loadExits();
}

Scene::Scene(const std::string& scene_name) 
    : name(scene_name) {
    loadExits();
}

Scene::~Scene() {}

bool Scene::loadSceneFile(std::filesystem::path fi) {
    std::ifstream file(fi);
    if (!file.is_open()) {
        return false;
    }
    try {
        
        json scenes_json;
        file >> scenes_json;
        file.close();

        // 处理场景数据
        if (scenes_json.contains(name)) {
            const auto& exits_obj = scenes_json[name];
            exits.clear(); // 清空现有出口
            
            for (auto it = exits_obj.begin(); it != exits_obj.end(); ++it) {
                try {
                    int exit_id = std::stoi(it.key());
                    std::string target_scene = it.value().get<std::string>();
                    
                    // 将出口信息添加到exits映射中
                    exits[exit_id] = target_scene;
                } catch (const std::exception& e) {
                }
            }
            return true;
        } else {
            return false;
        }
    } catch (const std::exception& e) {
        file.close();
        return false;
    }
}

void Scene::loadExits() {
    loadSceneFile(scene_file / "scenes.json");
}

std::string Scene::getSceneName(int key) {
        Controller::getInstance()->log(Controller::LogLevel::DEBUG , "key ok ");
    auto it = exits.find(key);
    if (it != exits.end()) {
        return it->second;
    } else {
        return ""; // 或者返回一个默认值，表示未找到
    }
}

std::string Scene::getNPCname(const char& specialChar) {
    std::filesystem::path filePath = scene_file / "NPCs.json";
    std::ifstream file(filePath);
    
    if (!file.is_open()) {
        Controller::getInstance()->log(Controller::LogLevel::DEBUG, "DEBUGor opening NPC file: " + filePath.string());
        return "";
    }
    
    // 解析 JSON
    json npcData;
    try {
        file >> npcData;
        file.close();
    } catch (const std::exception& e) {
        Controller::getInstance()->log(Controller::LogLevel::DEBUG, "DEBUGor parsing NPC JSON: " + std::string(e.what()));
        file.close();
        return "";
    }
    
    // 将char转换为string以便比较
    std::string specialCharStr(1, specialChar);
    
    // 遍历所有 NPC 查找匹配的 special_char
    for (auto& [npcName, npcInfo] : npcData.items()) {
        if (npcInfo.contains("special_char") && 
            npcInfo["special_char"].is_string() && 
            npcInfo["special_char"] == specialCharStr) {
            return npcName;
        }
    }
    
    // 如果没有找到匹配的NPC
    Controller::getInstance()->log(Controller::LogLevel::WARN, "No NPC found with special char: " + specialCharStr);
    return "";
}