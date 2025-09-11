#include "Controller.h"
#include <iostream>

Controller::Controller(
    const LogLevel& level,
    const std::filesystem::path& log_dir,
    const std::filesystem::path root_dir):
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
    // 临时实现
    std::cout << msg << std::endl;
}

int Controller::run() {
    std::cout << "登录/注册..." << std::endl;
    std::cout << "初始化游戏..." << std::endl;
    std::cout << "运行游戏..." << std::endl;
    // TODO 修改此状态开启循环
    bool running = false;
    EventType event_type = EventType::NONE;
    Message msg;
    while (running) {
        std::cout << "获取事件..." << std::endl;
        // msg = getEvent(event_type);

        switch(event_type) {
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
