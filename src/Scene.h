/**
 * @file Scene.h
 * @author Jie Jiang && Haozhe Tang
 */

#include <vector>
#include <memory>
#include <fstream>
#include <string>
#include <map>
#include "json.hpp"

#include "NPCType.h"
#include "Controller.h"

/**
 * @brief 场景类    
 * @details 场景类管理场景中应该存在的 NPC, 随机物品, 器械，入口，出口\n
 *          以及用于初始化的地图文件的文件名
 */
class Scene {
public:
    std::string name;   //< 场景名称
    std::map<int, std::string> exit; // < 场景中的出口列表，key为出口的唯一ID
    
    Scene() = default;
    /**
     * @brief 从 JSON 对象中加载场景数据
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

private:
};
