#include "Controller.h"
#include "View.h"
#include "tools.h"
#include <iostream>
#include <fstream>

Controller::Controller(
    const LogLevel& level,
    const std::filesystem::path& log_dir,
    const std::filesystem::path& root_dir):
    level(level),
    log_dir(log_dir),
    root_dir(root_dir) {
    // 构造函数
}

std::shared_ptr<Controller> Controller::getInstance(const LogLevel& level, const std::filesystem::path& log_dir, const std::filesystem::path& root_dir) {
    static auto instance = std::shared_ptr<Controller>(new Controller(level, log_dir, root_dir));
    return instance;
}

void Controller::log(const LogLevel& level, const std::string& msg) {
    std::ofstream debug_log(log_dir / DEBUG_FILE, std::ios::app);
    std::ofstream info_log(log_dir / INFO_FILE, std::ios::app);
    std::ofstream warn_log(log_dir / WARN_FILE, std::ios::app);
    std::ofstream error_log(log_dir / ERROR_FILE, std::ios::app);
    if (view != nullptr)
        switch (level) {
            case LogLevel::DEBUG:
                view->printLog(msg, "", Rgb(120, 120, 120));
                break;
            case LogLevel::INFO:
                view->printLog(msg, "white", Rgb(255, 255, 255));
                break;
            case LogLevel::WARN:
                view->printLog(msg, "", Rgb(219, 162, 0));
                break;
            case LogLevel::ERR:
                view->printLog(msg, "red", Rgb(255, 0, 0));
                break;
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
    map = std::make_shared<Map>();
    input = std::make_shared<InputHandler>();

    Message msg("Init Success!", 0);
    log(LogLevel::INFO, msg.msg);
    return msg;
}

Message Controller::load(std::string username) {
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
        iarchive(CEREAL_NVP(*protagonist));
                //  CEREAL_NVP(*backpack),
                 //CEREAL_NVP(*map));
        ifile.close();
    }
    map=std::make_shared<Map>();

    ifile.close();
    Message msg = Message("Load Success!", 0);
    log(LogLevel::INFO, msg.msg);
    return msg;
}

Message Controller::save() {
    // 游戏退出时保存文件，名称格式为 username.bin
    std::string username = protagonist->getName();
    std::string file_name = username + ".bin";
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

Message Controller::playerLogin() {
    const std::string invalidChars = ";:\"'\\/<>*?|";
    std::string user_name = "";
    do
    {
        
        std::cout << "Enter username: ";
        std::cin >> user_name;
        if (user_name.find_first_of(invalidChars) != std::string::npos)
        {
            std::cout << "Invalid username. Please avoid using special characters: " << invalidChars << std::endl;
            log (LogLevel::WARN, "Invalid username attempt: " + user_name);
            continue;
        }
    } while (1);
    log(LogLevel::INFO, "Username accepted: " + user_name);
    // 登录逻辑
    return Message("Login Success!", 0);
}


int Controller::run() {
    std::cout << "登录/注册..." << std::endl;
    std::cout << "初始化游戏..." << std::endl;
    std::cout << "创建测试临时数据..." << std::endl;
    map = std::make_shared<Map>("center.txt");
    view = View::getInstance();
    protagonist = std::make_shared<Protagonist>("TEST_USER_0001", "Jeb");
    std::cout << "创建完毕..." << std::endl;
    gameSleep(3000);
    view->reDraw();
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

        switch (event_type) {
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
    view->printQuestion("室友", "大爹带份饭可以吗？", "cyan");
    std::vector<std::string> tmp_ops {
        "1. 带一个",
        "2. no"
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


    log(LogLevel::INFO, "正在保存游戏...");
    gameSleep(2000);

    // 保持界面完整性
    std::cout << "\n\n";
    return 0;
}
