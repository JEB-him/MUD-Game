#include "Controller.h"
#include <iostream>

Controller::Controller(
    const LogLevel &level,
    const std::filesystem::path &log_dir,
    const std::filesystem::path root_dir) : level(level),
                                            log_dir(log_dir),
                                            root_dir(root_dir)
{
    // 构造函数
}

std::shared_ptr<Controller> Controller::getInstance(const LogLevel &level, const std::filesystem::path &log_dir, const std::filesystem::path &root_dir)
{
    static auto instance = std::shared_ptr<Controller>(new Controller(level, log_dir, root_dir));
    return instance;
}

void Controller::log(const LogLevel &level, const std::string &msg)
{
    // 临时实现
    cout << msg << std::endl;
}

Message Controller::init()
{
    protagonist = std::make_shared<Protagonist>();
    backpack = std::make_shared<Backpack>();
    map = std::make_shared<Map>();

    Message msg("Init Success!", 0);
    log(LogLevel::INFO, msg.msg);
    return msg;
}

Message Controller::load(std::string username)
{
    // 游戏开始时加载文件，名称格式为 username.bin
    std::string file_name = username + ".bin";
    std::ifstream ifile(file_name, std::ios::binary);
    if (!ifile.is_open())
    {
        Message msg = Message("Create a new accout", 0);

        log(LogLevel::INFO, msg.msg);
        return msg;
    }
    // 使用cereal进行反序列化
    {
        cereal::BinaryInputArchive iarchive(ifile);
        iarchive(protagonist, backpack, map);
        ifile.close();
    }

    ifile.close();
    Message msg = Message("Load Success!", 0);
    log(LogLevel::INFO, msg.msg);
    return msg;
}

Message Controller::save()
{
    // 游戏退出时保存文件，名称格式为 username.bin
    std::string username = protagonist.getName();
    std::string file_name = username + ".bin";
    std::ofstream ofile(file_name, std::ios::binary);
    if (!ofile.is_open())
    {
        Message msg = Message("Save Failed!", -1);
        std::cerr << msg.msg << std::endl;
        log(LogLevel::ERROR, msg.msg);
        return msg;
    }
    // 使用cereal进行序列化
    {
        cereal::BinaryOutputArchive oarchive(ofile);
        oarchive(protagonist, backpack, map);
        ofile.close();
    }

    ofile.close();
    Message msg = Message("Save Success!", 0);
    log(LogLevel::INFO, msg.msg);
    return msg;
}

Message Controller::getCmd()
{
    std::stringstream ss;
    ss.str("");
    ss.clear();

    while (1)
    {
        ch = input.waitKeyDown();
        // Enter
        if (ch == 10)
        {
            std::string cmd = ss.str();
            handleCmd(cmd);
            return ("Successfully sent and handled command: " + cmd, 0);
        }

        // Backspace
        else if (ch == 8)
        {
            std::string content = ss.str();
            content.pop_back();
            ss.str(content);
            ss.clear();
            ss << content;
            // 将ss实时显示在屏幕上
            // 需要确定接口...
            continue;
        }

        // Unexpect input
        else if (ch == 0)
        {
            continue;
        }

        else
            // 将ss实时显示在屏幕上
            // 需要确定接口...
            ss << ch;
    }
    return Message("No command input!", -1);
}

Message handleCmd(std::string cmd)
{
    switch (cmd)
    {
    case "help":
        /* code */
        break;

    case "quit":
        save();
        break;

    case "status":
        /* code */
        break;

    case "backpack":
        /* code */
        break;

    default:
        break;
    }
}