#include "Controller.h"

Controller::Controller(
    const LogLevel& level,
    const std::string& log_dir,
    const std::string root_dir):
    level(level),
    log_dir(log_dir),
    root_dir(root_dir) {
    // 构造函数
}

std::shared_ptr<Controller> Controller::getInstance(const LogLevel& level, const std::string& log_dir, const std::string root_dir) {
    static auto instance = std::shared_ptr<Controller>(new Controller(level, log_dir, root_dir));
    return instance;
}

void Controller::log(const LogLevel& level, const std::string& msg) {
    // 临时实现
    cout << msg << std::endl;
}
