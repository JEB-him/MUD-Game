#include "Scene.h"
#include <exception>
using json = nlohmann::json;

static std::shared_ptr<std::vector<std::string>>
getScenes()
{
}

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
        // TODO 补全文件名
        std::ifstream f("filename");
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
        std::cerr << "Error loading NPC data: " << e.what() << std::endl;
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
        // TODO 补全文件名
        std::ifstream f("filename");
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