#include "Controller.h"
#include "View.h"
#include "tools.h"
#include "Protagonist.h"
#include "NPC.h"
#include "InputHandler.h"
#include "Map.h"
#include "backpack.h"
#include "Store.h"
#include <iostream>
#include <fstream>
#include <regex>
#include "Store.h"
#include <fstream>
#include <string>
#include "Scene.h"
#if (defined(_WIN32) || defined(_WIN64))
#include <Windows.h>
#endif

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
    log(LogLevel::DEBUG, "Init: Pro");
    backpack = std::make_shared<Backpack>();
    log(LogLevel::DEBUG, "Init: back");
    input = std::make_shared<InputHandler>();
    log(LogLevel::DEBUG, "Init: input");
    view = View::getInstance();
    log(LogLevel::DEBUG, "Init view");
    scene = std::make_shared<Scene>();
    log(LogLevel::DEBUG, "Init scene");
    store = std::make_shared<Store>();

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
    std::string map_filename = "Dormitory.txt";
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
            iarchive(CEREAL_NVP(*protagonist),
                        CEREAL_NVP(*backpack));
            ifile.close();
        }
        backpack->namesToPointers();
        ifile.close();
        msg = Message("Load Success!", 0);
    }
    map=std::make_shared<Map>(map_filename);
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
            CEREAL_NVP(*protagonist),
            CEREAL_NVP(*backpack));
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
            ss.str("");
            ss.clear();
            view->printCmd(cmd);
            break;
        }

        // Backspace
        else if (ch == 8)
        {
            if (ss.str().length() > 0)
            {
                std::string str = ss.str().substr(0, ss.str().length() - 1);
                ss.str("");
                ss.clear();
                ss << str;
                view->printCmd(ss.str());
            }
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
    view = View::getInstance();
    view->reDraw();
    log(LogLevel::DEBUG, "Get event: " + cmd);
    // 处理cmd
    if (cmd == "move")
    {
        event_type = EventType::MOVE;
    }
    else if (cmd == "ac_npc")
    {
        event_type = EventType::AC_NPC;
    }
    else if (cmd == "ac_inst")
    {
        event_type = EventType::AC_INST;
    }
    else if (cmd == "open pack" || cmd == "open_pack")
    {
        event_type = EventType::OPEN_PACK;
    }
    else if (cmd == "refresh")
    {
        event_type = EventType::REFRESH;
    }
    else if (cmd == "status")
    {
        event_type = EventType::STATUS;
    }
    else if (cmd == "quit")
    {
        event_type = EventType::QUIT;
        return {"正常退出", 0};
    }
    else if (cmd == "store")
    {
        event_type = EventType::STORE;
    }
    else if (cmd == "help")
    {
        event_type = EventType::HELP;
    }
    else if (cmd == "use")
    {
        event_type = EventType::USE;
    }
    else
    {
        view = View::getInstance();
        view->reDraw();
        view->printQuestion("", "Enter \"help\" to get help.", "", Rgb(255, 255, 0));
        event_type = EventType::NONE;
    }
    return handleEvent(event_type);
}

Message Controller::handleEvent(EventType &event_type)
{
    static int NPCid = -1;
    switch (event_type)
    {
    case EventType::MOVE:
    {
        int ch = -1;
        Position pos;
        view = View::getInstance();
        view->printQuestion("", "按下ESC键退出移动模式。", "", Rgb(255, 255, 0));
        while ((ch = input->waitKeyDown()) != 10)
        {
            Position last_pos = map->getPos();
            switch (ch)
            {
            case 'w':
                map->moveProtagonist(0, event_type, NPCid);
                log(LogLevel::DEBUG, "NPCid: " + std::to_string(NPCid));
                break;
            case 'd':
                map->moveProtagonist(1, event_type, NPCid);
                log(LogLevel::DEBUG, "NPCid: " + std::to_string(NPCid));
                break;
            case 's':
                map->moveProtagonist(2, event_type, NPCid);
                log(LogLevel::DEBUG, "NPCid: " + std::to_string(NPCid));
                break;
            case 'a':
                map->moveProtagonist(3, event_type, NPCid);
                log(LogLevel::DEBUG, "NPCid: " + std::to_string(NPCid));
                break;
            case 27:
                return Message("Escape key pressed!", 0);
            }
            log(LogLevel::DEBUG, "------------------");
            pos = map->getPos();
            view->drawPoMove(last_pos, pos);
            log(LogLevel::DEBUG, "Move Success!");
            handleEvent(event_type);
        }
        return Message("Move Success!", 0);
    }
    case EventType::AC_NPC:
    {
        // 从move 拿到NPCid -> 借助Scene获取名称 -> 创建NPC对象 -> 顺序调用即可
        log(LogLevel::DEBUG, "AC_NPC");
        view = View::getInstance();
        if (NPCid == -1)
            return Message("Invalid NPC id!", -1);
        std::string NPCname = scene->getNPCname(NPCid);
        log(LogLevel::DEBUG, "Got name!" + NPCname);
        if (NPCname.empty())
            return Message("Invalid NPC id!", -1);
        npc = nullptr;
        npc = std::make_shared<NPC>(std::to_string(NPCname[0]), NPCname.substr(1), NPCid);
        npc->loadInteractionConfig(NPCname, (getRootDir() / ".config/NPCs.json").string());
        npc->startInteraction();
        return Message("NPC interaction Success!", 0);
    }
    case EventType::AC_INST:
    {
        log(LogLevel::DEBUG, "AC_NPC");
        view = View::getInstance();
        if (NPCid == -1)
            return Message("Invalid NPC id!", -1);
        std::string NPCname = scene->getNPCname(NPCid);
        log(LogLevel::DEBUG, "Got name!" + NPCname);
        if (NPCname.empty())
            return Message("Invalid NPC id!", -1);
        npc = nullptr;
        npc = std::make_shared<NPC>(std::to_string(NPCname[0]), NPCname.substr(1), NPCid);
        npc->loadInteractionConfig(NPCname, (getRootDir() / ".config/NPCs.json").string());
        npc->startInteraction();
        return Message("NPC interaction Success!", 0);
    }
    case EventType::OPEN_PACK:
    {
        view = View::getInstance();
        std::vector<std::shared_ptr<Item>> item_pts = backpack->getBackpackItems();
        std::vector<std::string> item_names;
        if (item_pts.empty())
        {
            view->printQuestion("", "Your backpack is empty.", "", Rgb(255, 255, 0));
            // std::string bookname = "advanced_mathematics";
            // (backpack->getBackpackItems()).push_back((backpack->getItemCreator()).createItem(bookname));
            // log(LogLevel::DEBUG, "Add item!");
        }
        else
        {
            for (auto &item_pt : item_pts)
            {
                item_names.push_back(item_pt->getName());
            }
        }
        view->printOptions(item_names);
        return Message("Open Pack Success!", 0);
    }
    case EventType::REFRESH:
    {
        view->reDraw();
        return Message("Refresh Success!", 0);
    }
    case EventType::STATUS:
    {
        view = View::getInstance();
        view->printOptions(protagonist->getStatus());
        return Message("Show status.", 0);
    }
    case EventType::JUMP:
    {
        // 拿到map.move的NPCid->给Scene对象->拿到场景文件名->重新创建Scene(新场景)->创建Map(新地图)->初始化地图数据->重新绘制地图->提示用户场景名称
        if (NPCid == -1)
        {
            return Message("Jump to default map.", 0);
        }
        std::string scene_name = scene->getSceneName(NPCid);
        if (scene_name.empty())
        {
            return Message("Invalid NPC id!", -1);
        }
        scene = nullptr;
        scene = std::make_shared<Scene>(scene_name);
        map = nullptr;
        map = std::make_shared<Map>(scene_name, Position(-1, -1));
        view = View::getInstance();
        view->reDraw();
        return Message("Jump Success!", 0);
    }
    case EventType::USE:
    {
        view = View::getInstance();
        backpack->addItem("energy_drink");
        std::vector<std::shared_ptr<Item>>
            item_pts = backpack->getBackpackItems();
        std::vector<std::string> item_names;
        int i = 1;
        for (auto &item_pt : item_pts)
        {
            item_names.push_back(std::to_string(i) + ". " + item_pt->getName());
            i++;
        }
        view->printOptions(item_names);
        view->printQuestion("", "Enter the number of the item you want to use.", "", Rgb(255, 255, 0));
        view->printQuestion("", "Enter ESC to cancel.", "", Rgb(255, 255, 0));
        int ch = -1;
        while (1)
        {
            ch = input->waitKeyDown();
            if (ch >= '0' && ch <= '9')
            {
                ch = ch - '0';
                break;
            }
            else if (ch == 27)
            {
                ch = -1;
                break;
            }
            else
            {
                view->printQuestion("", "Invalid input. Please enter a number.", "", Rgb(255, 255, 0));
            }
        }
        log(LogLevel::DEBUG, "Got index: " + std::to_string(ch));
        if (ch != -1)
        {
            backpack->useFunctionOfItem(ch, *protagonist);
            return Message("Use item Success!", 0);
        }
        else
            return Message("Use item canceled.", 0);
        view->reDraw();
    }
    case EventType::STORE:
    {
        view = View::getInstance();
        log(LogLevel::DEBUG, "getInstance!");
        store->showProducts(0);
        log(LogLevel::DEBUG, "Show products.");
        std::stringstream s1;
        s1.str("");
        s1.clear();
        std::string option = "";
        int index = -1;
        while (1)
        {
            int ch = input->waitKeyDown();
            // Enter
            if (ch == 10)
            {
                option = s1.str();
                s1.str("");
                s1.clear();
                index = std::stoi(option);
                break;
            }
            // Backspace
            else if (ch == 8)
            {
                if (s1.str().length() > 0)
                {
                    std::string str = s1.str().substr(0, s1.str().length() - 1);
                    s1.str("");
                    s1.clear();
                    s1 << str;
                    view->printCmd(s1.str());
                }
                continue;
            }
            else if (ch == 27)
            {
                index = -1;
                view->printCmd("");
                break;
            }
            else if (ch >= '0' && ch <= '9')
            {
                s1 << char(ch);
            }
            view->printCmd(s1.str());
        }
        view->reDraw();
        log(LogLevel::DEBUG, "get index: " + std::to_string(index));
        if (index == -1)
        {
            return Message("Store closed.", 0);
        }
        store->buyProduct(index);
        log(LogLevel::DEBUG, "buyProduct");
        return Message("Buy product.", 0);
    }
    case EventType::HELP:
    {
        ifstream fin(".config/help.txt");
        if (!fin.is_open())
        {
            return Message("Help file not found!", -1);
        }
        std::vector<std::string> help_msgs;
        std::string line;
        for (int i = 0; i < 11; i++)
        {
            std::getline(fin, line);
            if (line.empty())
                break;
            help_msgs.push_back(line);
        }
        view->printOptions(help_msgs);
    }
    case EventType::NONE:
    {
        return Message("Invalid command!", -1);
    }
    }
}

Message Controller::playerLogin(std::string &user_name)
{
    // 用户名文件夹和名单文件路径
    std::filesystem::path saves_dir = root_dir / "saves";
    std::filesystem::path users_file = saves_dir / "users.txt";
    std::set<std::string> user_set;

    // 如果目录不存在，创建目录
    if (!std::filesystem::exists(saves_dir))
    {
        std::filesystem::create_directories(saves_dir);
        log(LogLevel::INFO, "创建用户存档目录: " + saves_dir.string());
    }

    // 如果名单文件不存在，创建空文件
    if (!std::filesystem::exists(users_file))
    {
        std::ofstream fout(users_file);
        fout.close();
        log(LogLevel::INFO, "创建用户名单文件: " + users_file.string());
    }

    // 读取已存在的用户名
    {
        std::ifstream fin(users_file);
        std::string line;
        while (std::getline(fin, line))
        {
            if (!line.empty())
                user_set.insert(line);
        }
        fin.close();
    }

    // 打印所有已存在的用户名
    std::cout << "已存在的用户目录：" << std::endl;
    if (user_set.empty())
    {
        std::cout << "（暂无用户，请新建用户名）" << std::endl;
    }
    else
    {
        int i = 0;
        for (const auto &name : user_set)
        {
            std::cout << i++ << ": " << name << std::endl;
        }
    }

    std::string name;
    do
    {
        std::cout << "Enter username: ";
        std::getline(std::cin, name);

        if (name.empty())
        {
            std::cout << "Username cannot be empty. Please try again." << std::endl;
            std::cout << "**Empty username attempt**" << std::endl;
            continue;
        }

        if (isValidUsername(name))
        {
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
    if (user_set.find(name) == user_set.end())
    {
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
    static int turns = 100;
    EventType event_type = EventType::NONE;
    Message msg;
    view->printQuestion("", "Welcome to OUCSurvSim!", "", Rgb(255, 255, 0));
    view->printQuestion("", "Enter \"help\" to get help.", "", Rgb(255, 255, 0));
    while (running && turns--)
    {
        msg = getEvent(event_type);
        if (event_type == EventType::QUIT) {
            break;
        }
        event_type = EventType::NONE;
    }

    // 保存游戏
    save();

    // 保持界面完整性
    std::cout << "\n\n";
    return 0;
}

std::filesystem::path Controller::getRootDir() const {
    return root_dir;
}
