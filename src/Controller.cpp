#include "Controller.h"
#include "View.h"
#include "tools.h"
#include <iostream>
#include <fstream>
#include <regex>


Controller::Controller(
    const LogLevel &level,
    const std::filesystem::path &log_dir,
    const std::filesystem::path &root_dir):
    level(level),
    log_dir(log_dir),
    root_dir(root_dir) {
    // 构造函数
}

std::shared_ptr<Controller> Controller::getInstance(const LogLevel &level, const std::filesystem::path &log_dir, const std::filesystem::path &root_dir)
{
    static auto instance = std::shared_ptr<Controller>(new Controller(level, log_dir, root_dir));
    return instance;
}

void Controller::log(const LogLevel& level, const std::string& msg) {
    std::ofstream debug_log(log_dir / DEBUG_FILE, std::ios::app);
    std::ofstream info_log(log_dir / INFO_FILE, std::ios::app);
    std::ofstream warn_log(log_dir / WARN_FILE, std::ios::app);
    std::ofstream error_log(log_dir / ERROR_FILE, std::ios::app);
    if (view != nullptr) {
        Rgb rgb_color = {0 , 0, 0};
        switch (level) {
            case LogLevel::ERR:
                rgb_color = {255, 0, 0};
                if(this->level == LogLevel::ERR) view->printLog(msg, "", rgb_color);
            case LogLevel::WARN:
                if(rgb_color.r + rgb_color.g + rgb_color.b == 0) rgb_color = Rgb(219, 162, 0);
                if(this->level == LogLevel::WARN) view->printLog(msg, "", rgb_color);
            case LogLevel::INFO:
                if(rgb_color.r + rgb_color.g + rgb_color.b == 0) rgb_color = Rgb(255, 255, 255);
                if(this->level == LogLevel::INFO) view->printLog(msg, "", rgb_color);
            case LogLevel::DEBUG:
                if(rgb_color.r + rgb_color.g + rgb_color.b == 0) rgb_color = Rgb(120, 120, 120);
                if(this->level == LogLevel::DEBUG) view->printLog(msg, "", rgb_color);
                break;
        }
    } else {
        error_log << "未初始化 View 下调用 View::printLog" << std::endl;
    }
    if (this->level == LogLevel::INFO && level != LogLevel::DEBUG)
        info_log << msg << std::endl;
    if (this->level == LogLevel::WARN && level != LogLevel::INFO && level != LogLevel::DEBUG)
        warn_log << msg << std::endl;
    if (level == LogLevel::ERR)
        error_log << msg << std::endl;
    debug_log << msg << std::endl;
}

Message Controller::init() {
    protagonist = std::make_shared<Protagonist>();
    // backpack = std::make_shared<Backpack>();
    input = std::make_shared<InputHandler>();
    view = View::getInstance();

    Message msg {"Init Success!", 0};
    std::cout << msg.msg << std::endl;
    return msg.msg;
}

Message Controller::load(std::string username) {
    // 游戏开始时加载文件，名称格式为 username.bin
    std::filesystem::path file_name = root_dir / "saves" / (username + ".bin");
    std::ifstream ifile(file_name, std::ios::binary);
    Position init_pos {-1, -1};
    // 设置默认出生点
    // TODO 修改逻辑，应当通过默认场景类获得默认文件名
    std::string map_filename = "center.txt";
    Message msg;
    // 创建新用户之后还需要设置主角的位置
    if (!ifile.is_open()) {
        if(!std::filesystem::exists(file_name)){
            std::ofstream fout(file_name, std::ios::binary);
            fout.close();
            std::cout << "创建用户文件: " << file_name.string() << std::endl;
        }

        time_t now = time(NULL);
        // 将当前时间转换为本地时间
        struct tm *local_tm = localtime(&now);
        // 使用 strftime 格式化时间
        std::string playerID(25, '\0');
        strftime(playerID.data(), playerID.size(), "OUC_%Y-%m-%d_%H:%M:%S", local_tm);

        protagonist = std::make_shared<Protagonist>(playerID, username);
        msg = Message("Create a new account", 0);
    } else {
        // 使用cereal进行反序列化
        {
            cereal::BinaryInputArchive iarchive(ifile);
            iarchive(CEREAL_NVP(*protagonist));
            //  CEREAL_NVP(*backpack),
            // CEREAL_NVP(*map));
            ifile.close();
        }

        // 向场景类获取地图文件，向主角获取坐标
        init_pos = protagonist->getPosition();
        map_filename = "???";

        ifile.close();
        msg = Message("Load Success!", 0);
    }
    // 初始化地图数据
    if (init_pos.x != -1) {
        // 使用 init_pos 填入下面的参数列表中
        map = std::make_shared<Map>(map_filename);
    } else {
        // 仅使用地图文件初始化
        map = std::make_shared<Map>(map_filename);
    }
    std::cout << msg.msg << std::endl;
    view->reDraw();
    return msg;
}

Message Controller::save() {
    // 游戏退出时保存文件，名称格式为 username.bin
    std::string username = protagonist->getName();
    std::filesystem::path file_name = root_dir / "saves" / (username + ".bin");
    std::ofstream ofile(file_name, std::ios::binary);
    if (!ofile.is_open()) {
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

Message Controller::getEvent(EventType &event_type) {
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
            char tmp_ch;
            ss >> tmp_ch;
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

        view->printCmd(ss.str());
    }
    // TODO 修正逻辑，只有按下 Enter 才需要处理下面的逻辑
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
            std::cout << "**Empty username attempt**" << std::endl;
            continue;
        }

        if (isValidUsername(name)) {
            break;
        } else {
            std::cout <<"\033[31m"<< "Invalid username. Only letters, numbers, underscores, and Chinese characters are allowed." << "\033[0m" << std::endl;
            std::cout << "\033[31m**Invalid username attempt: " << name << "**\033[0m" << std::endl;
        }
    } while (true);

    user_name = name;
    std::cout  << "SUCCESS! Hello: "<<"\033[93m" << name << "\033[0m" << std::endl;
    std::cout << "Username accepted: " << name << std::endl;

    // 如果是新用户，写入用户名文件
    if (user_set.find(name) == user_set.end()) {
        std::ofstream fout(users_file, std::ios::app);
        fout << name << std::endl;
        fout.close();
        std::cout << "新用户已加入名单: " << name << std::endl;
    }

    return Message("Login Success!", 0);
}


int Controller::run()
{
    init();
    std::cout << "初始化成功" << std::endl;
    std::string user_name;
    playerLogin(user_name);
    load(user_name);
    log(LogLevel::DEBUG, "运行游戏...");
    log(LogLevel::DEBUG, "DEBUG...");
    bool running = true;
    // 防止死循环
    // TODO 修改回合次数
    static int turns = 1;
    EventType event_type = EventType::NONE;
    Message msg;
    while (running && turns--) {
        log(LogLevel::DEBUG, "获取事件...");
        // msg = getEvent(event_type);

        switch (event_type)
        {
        case EventType::MOVE:
            log(LogLevel::DEBUG, "移动主角...");
            break;
        case EventType::QUIT:
            log(LogLevel::INFO, "退出游戏...");
            running = false;
            break;
        case EventType::NONE:
            log(LogLevel::DEBUG, "无按键响应...");
            break;
        }
    }

    // 测试用
    std::stringstream ss;
    view->printCmd("测试命令");
    gameSleep(500);
    view->printCmd("2 hello cat");
    view->printQuestion("", "清晨，你在室友的闹铃声中醒来....", "white");
    view->printQuestion("", "清清晨，你在室友的闹铃声中醒来....清晨，你在室友的闹铃声中醒来....清晨，你在室友的闹铃声中醒来....清晨，你在室友的闹铃声中醒来....清晨，你在室友的闹铃声中醒来....清晨，你在室友的闹铃声中醒来....清晨，你在室友的闹铃声中醒来....清晨，你在室友的闹铃声中醒来....清晨，你在室友的闹铃声中醒来....清晨，你在室友的闹铃声中醒来....清晨，你在室友的闹铃声中醒来....清晨，你在室友的闹铃声中醒来....清晨，你在室友的闹铃声中醒来....晨，你在室友的闹铃声中醒来....", "white");
    gameSleep(1000);
    view->printQuestion("室友", "大爹带份饭可以吗？", "cyan");
    std::vector<std::string> tmp_ops {
        "1. 带一个",
        "2. no",
        "3. fwefew",
        "3. fwefew",
        "3. fWEFew",
        "3. fWEFew",
        "3. fWEFew",
        "3. fWEFew",
        "3. fWEFew",
        "3. fWEFew",
        "3. fwefw",
        "3. fweew",
        "3. fefew",
        "3. fwefw",
        "3.fwefew",
        "3. fefew",
        "3. fWEFew",
        "3. fwefw",
        "3. fweew",
        "3. fefew",
        "3. fwefw",
        "3.fwefew",
        "3. fefew",
        "3. fWEFew",
        "3. fwefw",
        "3. fweew",
        "3. fefew",
        "3. fwefw",
        "3.fwefew",
        "3. fefew",
        "3. fWEFew",
        "3. fwefw",
        "3. fweew",
        "3. fefew",
        "3. fwefw",
        "3.fwefew",
        "3. fefew",
        "3. fWEFew",
        "3. fwefw",
        "3. fweew",
        "3. fefew",
        "3. fwefw",
        "3.fwefew",
        "3. fefew",
        "3. fWEFew",
        "3. fwefw",
        "3. fweew",
        "3. fefew",
        "3. fwefw",
        "3.fwefew",
        "3. fefew",
        "3. fWEFew",
        "3. fwefw",
        "3. fweew",
        "3. fefew",
        "3. fwefw",
        "3.fwefew",
        "3. fefew",
        "3.fwfew",
        "3. fefew",
    };
    view->printOptions(tmp_ops);
    view->printQuestion("NPC", "你好", "white");
    log(LogLevel::DEBUG, "移动主角");
    int map_event, id;
    Position old_pos = map->getPos();
    ss << "初始位置: " << old_pos.x << " " << old_pos.y;
    log(LogLevel::DEBUG, ss.str());
    ss.str("");
    gameSleep(500);
    map->moveProtagonist(0, map_event, id);
    Position pos = map->getPos();
    protagonist->setPosition(pos);
    view->drawPoMove(old_pos, pos);
    ss << "当前位置: " << pos.x << " " << pos.y;
    log(LogLevel::DEBUG, ss.str());
    ss.str("");
    // 测试结束


    // 保存游戏
    save();

    // 保持界面完整性
    std::cout << "\n\n";
    return 0;
}
