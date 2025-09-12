#include "catch.hpp"
#include "Protagonist.h"
#include <sstream>
#include <fstream>
#include <filesystem>

// 测试用例：主角类基础功能测试
TEST_CASE("Protagonist Basic Functionality", "[Protagonist]") {
    // 测试用主角ID和姓名（与测试JSON文件匹配）
    const std::string TEST_ID = "OUC_2025_08_31";
    const std::string TEST_NAME = "小明";
    Protagonist protag(TEST_ID, TEST_NAME);

    // 测试1：基础信息获取
    SECTION("Basic Information Getters") {
        REQUIRE(protag.getProtagonistId() == TEST_ID);
        REQUIRE(protag.getName() == TEST_NAME);
    }

    // 测试2：从JSON文件加载属性
    SECTION("JSON File Loading") {
        // 验证从测试JSON加载的初始属性
        auto baseAttrs = protag.getBaseAttrs();
        REQUIRE(baseAttrs[BasicValue::ProtagonistAttr::INTEL_SCI] == 10);
        REQUIRE(baseAttrs[BasicValue::ProtagonistAttr::INTEL_ARTS] == 10);
        REQUIRE(baseAttrs[BasicValue::ProtagonistAttr::STRENGTH] == 50);
        REQUIRE(baseAttrs[BasicValue::ProtagonistAttr::MONEY] == 10);
        REQUIRE(baseAttrs[BasicValue::ProtagonistAttr::HEALTH] == 10);
    }

    // 测试3：属性修改接口
    SECTION("Attribute Update Interface") {
        // 测试合法的属性增加
        int msg = protag.updateAttr(BasicValue::ProtagonistAttr::MONEY, 50, true).status;
        REQUIRE(msg == 0);
        REQUIRE(protag.getBaseAttrs()[BasicValue::ProtagonistAttr::MONEY] == 60);

        // 测试合法的属性设置
        msg = protag.updateAttr(BasicValue::ProtagonistAttr::HEALTH, 80, false).status;
        REQUIRE(msg == 0);
        REQUIRE(protag.getBaseAttrs()[BasicValue::ProtagonistAttr::HEALTH] == 80);

        // 测试属性越界
        msg = protag.updateAttr(BasicValue::ProtagonistAttr::STRENGTH, 150, false).status;
        REQUIRE(msg == 1);  // 越界错误

        // 测试隐藏属性修改
        msg = protag.updateAttr(BasicValue::ProtagonistAttr::INTELSCI_BOOST, 5, false).status;
        REQUIRE(msg == 0);
        REQUIRE(protag.getHiddenAttrs()[BasicValue::ProtagonistAttr::INTELSCI_BOOST] == 5.0f);

        // 测试受伤状态修改
        msg = protag.updateAttr(BasicValue::ProtagonistAttr::IS_INJURED, 1, false).status;
        REQUIRE(msg  == 0);
        std::string stateDesc;
        REQUIRE(protag.getHealthState(stateDesc) == BasicValue::HealthState::INJURED);
    }

    // 测试4：健康状态判断
    SECTION("Health State Judgment") {
        std::string stateDesc;

        // 测试受伤状态
        protag.updateAttr(BasicValue::ProtagonistAttr::IS_INJURED, 1, false);
        REQUIRE(protag.getHealthState(stateDesc) == BasicValue::HealthState::INJURED);
        REQUIRE(stateDesc == "受伤");

        // 测试健康状态
        protag.updateAttr(BasicValue::ProtagonistAttr::IS_INJURED, 0, false);
        protag.updateAttr(BasicValue::ProtagonistAttr::HEALTH, 80, false);
        REQUIRE(protag.getHealthState(stateDesc) == BasicValue::HealthState::HEALTHY);
        REQUIRE(stateDesc == "健康");

        // 测试生病状态
        protag.updateAttr(BasicValue::ProtagonistAttr::HEALTH, 50, false);
        REQUIRE(protag.getHealthState(stateDesc) == BasicValue::HealthState::SICK);
        REQUIRE(stateDesc == "生病");

        // 测试死亡状态
        protag.updateAttr(BasicValue::ProtagonistAttr::HEALTH, 0, false);
        REQUIRE(protag.getHealthState(stateDesc) == BasicValue::HealthState::DEAD);
        REQUIRE(stateDesc == "死亡");
    }

    // // 测试5：序列化与反序列化
    // SECTION("Serialization and Deserialization") {
    //     // 修改部分属性用于测试
    //     protag.updateAttr(BasicValue::ProtagonistAttr::MONEY, 100, false);
    //     protag.updateAttr(BasicValue::ProtagonistAttr::INTELARTS_BOOST_RATE, 1.5f, false);
    //     protag.updateAttr(BasicValue::ProtagonistAttr::IS_INJURED, 1, false);

    //     // 序列化
    //     std::string serializedData = protag.serialize();
    //     REQUIRE(!serializedData.empty());

    //     // 创建新对象并反序列化
    //     Protagonist newProtag(TEST_ID, TEST_NAME);
    //     auto msg = newProtag.deserialize(serializedData);
    //     REQUIRE(msg.status == 0);

    //     // 验证反序列化结果
    //     REQUIRE(newProtag.getBaseAttrs()[BasicValue::ProtagonistAttr::MONEY] == 100);
    //     REQUIRE(newProtag.getHiddenAttrs()[BasicValue::ProtagonistAttr::INTELARTS_BOOST_RATE] == 1.5f);
        
    //     std::string stateDesc;
    //     REQUIRE(newProtag.getHealthState(stateDesc) == BasicValue::HealthState::INJURED);
    // }

    // // 测试6：JSON文件错误处理
    // SECTION("JSON Error Handling") {
    //     // 备份原测试文件
    //     std::filesystem::path testFile = ".\\..\\tests\\unit\\Protagonist_Test.json";
    //     std::filesystem::path backupFile = testFile.string() + ".bak";
    //     std::filesystem::copy_file(testFile, backupFile, std::filesystem::copy_options::overwrite_existing);

    //     // 测试文件不存在的情况
    //     std::filesystem::remove(testFile);
    //     Protagonist badProtag(TEST_ID, TEST_NAME);
    //     auto baseAttrs = badProtag.getBaseAttrs();
    //     REQUIRE(baseAttrs[BasicValue::ProtagonistAttr::HEALTH] == 100);  // 使用默认值

    //     // 恢复原测试文件
    //     std::filesystem::copy_file(backupFile, testFile, std::filesystem::copy_options::overwrite_existing);
    //     std::filesystem::remove(backupFile);
    // }
}