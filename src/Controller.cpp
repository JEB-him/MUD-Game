#include "Controller.h"
#include <iostream>
#include <fstream>
#include <regex>


Controller::Controller(
    const LogLevel &level,
    const std::filesystem::path &log_dir,
    const std::filesystem::path &root_dir) : level(level),
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
    std::cout << msg << std::endl;
}

Message Controller::init()
{
    protagonist = std::make_shared<Protagonist>();
    // backpack = std::make_shared<Backpack>();
    map = std::make_shared<Map>();
    input = std::make_shared<InputHandler>();

    Message msg("Init Success!", 0);
    log(LogLevel::INFO, msg.msg);
    return msg;
}

Message Controller::load(std::string username)
{
    // 游戏开始时加载文件，名称格式为 username.bin
    std::filesystem::path file_name = root_dir / "saves" / (username + ".bin");
    std::ifstream ifile(file_name, std::ios::binary);
    if (!ifile.is_open())
    {
        if(!std::filesystem::exists(file_name)){
            std::ofstream fout(file_name, std::ios::binary);
            fout.close();
            log(LogLevel::INFO, "创建用户文件: " + file_name.string());
        }

        time_t now = time(NULL);
        // 将当前时间转换为本地时间
        struct tm *local_tm = localtime(&now);
        // 使用 strftime 格式化时间
        std::string playerID(25, '\0');
        strftime(playerID.data(), playerID.size(), "OUC_%Y-%m-%d_%H:%M:%S", local_tm);

        protagonist = std::make_shared<Protagonist>(playerID, username);
        // backpack = std::make_shared<Backpack>(); // 新建一个空的背包
        Message msg = Message("Create a new account", 0);

        log(LogLevel::INFO, msg.msg);
        return msg;
    }
    // 使用cereal进行反序列化
    {
        cereal::BinaryInputArchive iarchive(ifile);
        iarchive(CEREAL_NVP(*protagonist));
        //  CEREAL_NVP(*backpack),
        // CEREAL_NVP(*map));
        ifile.close();
    }
    map = std::make_shared<Map>();

    ifile.close();
    Message msg = Message("Load Success!", 0);
    log(LogLevel::INFO, msg.msg);
    return msg;
}

Message Controller::save()
{
    // 游戏退出时保存文件，名称格式为 username.bin
    std::string username = protagonist->getName();
    std::filesystem::path file_name = root_dir / "saves" / (username + ".bin");
    std::ofstream ofile(file_name, std::ios::binary);
    if (!ofile.is_open())
    {
        Message msg = Message("Save Failed!", -1);
        std::cerr << msg.msg << std::endl;
        log(LogLevel::ERR, msg.msg);
        return msg;
    }
    // 使用cereal进行序列化
    {
        cereal::BinaryOutputArchive oarchive(ofile);
        oarchive(
            CEREAL_NVP(*protagonist));

        ofile.close();
    }
    map->save();
    ofile.close();
    Message msg = Message("Save Success!", 0);
    log(LogLevel::INFO, msg.msg);
    return msg;
}

Message Controller::getEvent(EventType &event_type)
{
    std::stringstream ss;
    ss.str("");
    ss.clear();
    std::string cmd = "";

    while (1)
    {
        int ch = input->waitKeyDown();
        // Enter
        if (ch == 10)
        {
            cmd = ss.str();
            break;
        }

        // Backspace
        else if (ch == 8)
        {
            std::string content = ss.str();
            content.pop_back();
            ss.str(content);
            ss.clear();
            ss << content;
            continue;
        }

        // Unexpect input
        else if (ch == 0)
        {
            continue;
        }

        else
        {
            ss << char(ch);
        }
        // 这里将处理好的ss传给View
    }
    // 处理cmd
    // switch (cmd)
    // {
    // case "move":
    //     event_type = EventType::MOVE;
    //     break;
    // case "ac_npc", case "ac npc":
    //     event_type = EventType::AC_NPC;
    //     break;
    // case "ac_inst", case "ac inst":
    //     event_type = EventType::AC_INST;
    //     break;
    // case "open pack", case "open_pack":
    //     event_type = EventType::OPEN_PACK;
    //     break;
    // case "refresh":
    //     event_type = EventType::REFRESH;
    //     break;
    // case "status":
    //     event_type = EventType::STATUS;
    //     break;
    // case "jump":
    //     event_type = EventType::JUMP;
    //     break;
    // case "tp":
    //     event_type = EventType::TP;
    //     break;
    // }

    Message msg("Command: " + cmd, 0);
    return msg;
}

// Message challenge::handleCmd(std::string cmd)
// {
//     switch (cmd)
//     {
//     case "help":
//         /* code */
//         break;

//     case "quit":
//         save();
//         break;

//     case "status":
//         /* code */
//         break;

//     case "backpack":
//         /* code */
//         break;

//     default:
//         break;
//     }
// }

// 登录逻辑，在init()后调用
Message Controller::playerLogin(std::string &user_name) {
    // 用户名文件夹和名单文件路径
    std::filesystem::path saves_dir = root_dir / "saves";
    std::filesystem::path users_file = saves_dir / "users.txt";
    std::set<std::string> user_set;

    // 如果目录不存在，创建目录
    if (!std::filesystem::exists(saves_dir)) {
        std::filesystem::create_directories(saves_dir);
        log(LogLevel::INFO, "创建用户存档目录: " + saves_dir.string());
    }

    // 如果名单文件不存在，创建空文件
    if (!std::filesystem::exists(users_file)) {
        std::ofstream fout(users_file);
        fout.close();
        log(LogLevel::INFO, "创建用户名单文件: " + users_file.string());
    }

    // 读取已存在的用户名
    {
        std::ifstream fin(users_file);
        std::string line;
        while (std::getline(fin, line)) {
            if (!line.empty()) user_set.insert(line);
        }
        fin.close();
    }

    // 打印所有已存在的用户名
    std::cout << "已存在的用户目录：" << std::endl;
    if (user_set.empty()) {
        std::cout << "（暂无用户，请新建用户名）" << std::endl;
    } else {
        int i=0;
        for (const auto& name : user_set) {
            std::cout << i++ << ": " << name << std::endl;
        }
    }

    std::string name;
    do {
        std::cout << "Enter username: ";
        std::getline(std::cin, name);

        if (name.empty()) {
            std::cout << "Username cannot be empty. Please try again." << std::endl;
            log(LogLevel::WARN, "Empty username attempt");
            continue;
        }

        if (isValidUsername(name)) {
            break;
        } else {
            std::cout <<"\033[31m"<< "Invalid username. Only letters, numbers, underscores, and Chinese characters are allowed." << "\033[0m" << std::endl;
            log(LogLevel::WARN, "Invalid username attempt: " + name);
        }
    } while (true);

    user_name = name;
    std::cout  << "SUCCESS! Hello: "<<"\033[93m" << name << "\033[0m" << std::endl;
    log(LogLevel::INFO, "Username accepted: " + name);

    // 如果是新用户，写入用户名文件
    if (user_set.find(name) == user_set.end()) {
        std::ofstream fout(users_file, std::ios::app);
        fout << name << std::endl;
        fout.close();
        log(LogLevel::INFO, "新用户已加入名单: " + name);
    }

    return Message("Login Success!", 0);
}


int Controller::run()
{
    init();
    std::cout << "登录/注册..." << std::endl;
    std::string user_name;
    playerLogin(user_name);
    std::cout << "初始化游戏..." << std::endl;
    load(user_name);
    std::cout << "运行游戏..." << std::endl;
    // TODO 修改此状态开启循环
    bool running = false;
    EventType event_type = EventType::NONE;
    Message msg;
    while (running)
    {
        std::cout << "获取事件..." << std::endl;
        // msg = getEvent(event_type);

        switch (event_type)
        {
        case EventType::MOVE:
            std::cout << "移动主角..." << std::endl;
            break;
        case EventType::QUIT:
            std::cout << "退出游戏..." << std::endl;
            running = false;
            break;
        case EventType::NONE:
            break;
        }
    }
    return 0;
}
