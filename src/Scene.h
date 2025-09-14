/**
 * @file Scene.h
 * @author Jie Jiang && Haozhe Tang
 */

#include <vector>
#include <memory>
#include "json.hpp"

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
private:
};
