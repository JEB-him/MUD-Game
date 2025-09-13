/**
 * @file Scene.h
 * @author Jie Jiang
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
    Scene();
    static std::shared_ptr<std::vector<std::string>> getScenes();
private:
};
