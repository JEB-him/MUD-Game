#include "Controller.h"

#include <cereal/archives/binary.hpp>
#include <cereal/types/string.hpp>
#include <cereal/types/memory.hpp>
#include <cereal/types/vector.hpp>

#include <string>
#include <fstream>

Controller::Controller(
    const LogLevel &level,
    const std::string &log_dir,
    const std::string root_dir)
    : level(level),
      log_dir(log_dir),
      root_dir(root_dir) {} // 构造函数

std::shared_ptr<Controller> Controller::getInstance(const LogLevel &level,
                                                    const std::string &log_dir,
                                                    const std::string root_dir)
{
    static std::shared_ptr<Controller> instance(new Controller(level, log_dir, root_dir));
    return instance;
}

void Controller::log(const LogLevel &level, const std::string &msg)
{
    // 临时实现
    cout << msg << std::endl;
}

Message Controller::init(std::string username, Protagonist &pro, Backpack &backpack)
{
    // 初始化 Protagonist 和 Backpack
    Protagonist pro;
    pro.setName(username);
    Backpack backpack;
}

Message Controller::save(Protagonist &pro, Backpack &backpack)
{
    // 游戏退出时保存文件，名称格式为 username.bin
    std::string username = pro.getName();
    std::string file_name = username + ".bin";
    std::ofstream ofile(file_name, std::ios::binary);
    if (!ofile.is_open())
    {
        std::cerr << "Error: Cannot open file! " << file_name << std::endl;
        return Message("Save Failed!", -1);
    }
    // 使用cereal进行序列化
    {
        cereal::BinaryOutputArchive oarchive(ofile);
        oarchive(pro, backpack);
        ofile.close();
    }

    ofile.close();
    return Message("Save Success!", 0);
}

void InputHandler()
{
    int k = waitkeydown(); // 获取键盘操作
}