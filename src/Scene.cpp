#include "Scene.h"
#include <exception>
#include <iostream>
#include "Controller.h"

using json = nlohmann::json;

#include <fstream>

/**
 * @brief 从文件加载 NPC 数据并获取具有指定场景属性的 NPC
 * @param scene 场景名称
 * @return 包含 NPC 名称和类型的映射的共享指针
 */
std::shared_ptr<std::map<std::string, NPCType>> Scene::getNPCs()
{
    auto result = std::make_shared<std::map<std::string, NPCType>>();
    static const std::map<std::string, NPCType> npc_name_to_type = {
        {"Student", NPCType::STUDENT},
        {"Professor", NPCType::PROFESSOR},
        {"CanteenStaff", NPCType::CANTEEN_STAFF},
        {"Roommate", NPCType::ROOMMATE},
        {"DormManager", NPCType::DORM_MANAGER},
        {"Employer", NPCType::EMPLOYER},
        {"Librarian", NPCType::LIBRARIAN},
        {"Football", NPCType::COACH},
        {"Basketball", NPCType::COACH},
        {"Badminton", NPCType::COACH},
        {"Running", NPCType::COACH},
        {"Bed", NPCType::INSTRUMENT}};

    try
    {
        std::ifstream f(".config/scenes.json");
        json npc_data = json::parse(f);

        // 遍历所有 NPC
        for (auto &[npc_name, npc_info] : npc_data.items())
        {
            // 检查是否有 scenes 属性
            if (npc_info.contains("scenes") && npc_info["scenes"].is_array())
            {
                // 检查 scenes 数组中是否包含指定的场景
                auto scenes = npc_info["scenes"];
                bool has_scene = false;

                for (const auto &s : scenes)
                {
                    if (s.get<std::string>() == name)
                    {
                        has_scene = true;
                        break;
                    }
                }

                // 如果包含指定场景，添加到结果中
                if (has_scene)
                {
                    // 查找 NPC 名称对应的类型
                    auto it = npc_name_to_type.find(npc_name);
                    if (it != npc_name_to_type.end())
                    {
                        (*result)[npc_name] = it->second;
                    }
                    else
                    {
                        // 没有找到映射，使用默认值
                        (*result)[npc_name] = NPCType::INSTRUMENT;
                    }
                }
            }
        }
    }
    catch (const std::exception &e)
    {
        // 处理异常，例如文件不存在或 JSON 格式错误
        Controller::getInstance()->log(Controller::LogLevel::ERR,"Error loading NPC data: " + std::string(e.what()));
    }

    return result;
}

/**
 * @brief 从文件加载 NPC 数据并获取具有指定场景属性的 NPC
 * @param scene 场景名称
 * @return 包含 NPC 名称和类型的映射的共享指针
 */
std::shared_ptr<std::map<std::string, NPCType>> Scene::getSpecialCharNPCs()
{

    auto result = std::make_shared<std::map<std::string, NPCType>>();
    static const std::map<std::string, NPCType> npc_char_to_type = {
        {"f", NPCType::DORM_MANAGER},
        {"g", NPCType::EMPLOYER},
        {"h", NPCType::LIBRARIAN},
        {"j", NPCType::CANTEEN_STAFF},
        {"k", NPCType::PROFESSOR}};

    try
    {
        std::ifstream f(".config/scenes.json");
        json npc_data = json::parse(f);

        // 遍历所有 NPC
        for (auto &[npc_name, npc_info] : npc_data.items())
        {
            // 检查是否有 special_char 属性
            if (npc_info.contains("special_char"))
            {
                std::string special_char_str = npc_info["special_char"].get<std::string>();

                // 查找 NPC 名称对应的类型
                auto it = npc_char_to_type.find(special_char_str);
                if (it != npc_char_to_type.end())
                {
                    (*result)[special_char_str] = it->second;
                }
                else
                {
                    // 没有找到映射，使用默认值
                    (*result)[special_char_str] = NPCType::INSTRUMENT;
                }
            }
        }
    }
    catch (const std::exception &e)
    {
        // 处理异常，例如文件不存在或 JSON 格式错误
        std::cerr << "Error loading NPC data: " << e.what() << std::endl;
    }

    return result;
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