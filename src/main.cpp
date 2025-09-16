#define CATCH_CONFIG_RUNNER
#include "catch.hpp"
#include "View.h"
#include "Controller.h"
#include "Welcome.h"
#include <iostream>
#include <fstream>
#include <unordered_map>
#include <filesystem>
#if defined(_WIN32) && !defined(__linux__)
#   include <windows.h>
#   include <clocale>
    struct SetupLocale {
        SetupLocale() {
            std::setlocale(LC_ALL, ".UTF-8");
        }
    };
    SetupLocale setupLocale;
#endif
#if !defined(_WIN32) && defined(__linux__)
#   include <unistd.h>
#endif

void envCheck() {
    // 环境检查
#if !defined(__GNUG__) && !defined(_MSC_VER)
    std::cerr << "本项目仅在 GNU(Linux)/MinGW(Windows)/VS2022 下测试通过" << std::endl;
    exit(-1);
#endif
#ifdef _WIN32
    // 启用虚拟终端
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD dwMode = 0;
    GetConsoleMode(hConsole, &dwMode);
    dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING; // 启用虚拟终端
    SetConsoleMode(hConsole, dwMode);
    // 双重保险
    system("chcp 65001");
    std::cout << "\x1bM";
#endif
}

// 显示欢迎信息
void showWelcome() {
    std::cout << OUC_WELCOME_CONTENT << std::endl;
}

// 显示主帮助信息
void showMainHelp(const char* programName) {
    std::cout << "================================== Help =====================================" << std::endl;
    std::cout << "Usage: " << programName << " <command> [options]" << std::endl;
    std::cout << std::endl;
    std::cout << "Available commands:" << std::endl;
    std::cout << "  run      启动游戏" << std::endl;
    std::cout << "  test     运行测试" << std::endl;
    std::cout << std::endl;
    std::cout << "Use `" << programName << " <command> --help` for more information about a command." << std::endl;
    std::cout << "Documentation: start docs/html/index.html (Windows)" << std::endl;
    std::cout << "               xdg-open docs/html/index.html (Linux)" << std::endl;
    std::cout << "================================== End =======================================" << std::endl;
    std::cout << "\n[38;5;45mSimple usage: " << programName << " run[0m" << std::endl;
    std::cout << "[38;5;45mSimple usage: " << programName << " test[0m" << std::endl;
}

// 处理 run 命令
int handleRunCommand(int argc, char* argv[]) {
    namespace fs = std::filesystem;
    std::unordered_map<std::string, Controller::LogLevel> levels {
        {"DEBUG", Controller::LogLevel::DEBUG},
        {"INFO", Controller::LogLevel::INFO},
        {"WARN", Controller::LogLevel::WARN},
        {"ERR", Controller::LogLevel::ERR}
    };

    std::string root_str = "./", level = "INFO", log_str = "logs/";
    bool help = false;
    
    // 构建 run 命令的解析器
    using namespace Catch::clara;
    auto cli = Opt(root_str, "root directory")["-r"]["--root"]("所有配置文件的根目录(使用/)") |
               Opt(log_str, "log directory")["-l"]["--logs"]("日志文件输出目录(使用/)") |
               Opt(level, "log level")["-g"]["--glevel"]("日志等级\n决定日志的详细程度") |
               Help(help);
    
    // 解析 run 命令的参数
    auto result = cli.parse(Args(argc, argv));
    if (!result || help) {
        std::cout << "================================== Run Command Help ===========================" << std::endl;
        std::cout << "Usage: " << argv[0] << " run [options]" << std::endl;
        std::cout << cli << std::endl;
        std::cout << "================================== End =======================================" << std::endl;
        if (!result) std::cerr << "Error in command line: " << result.errorMessage() << std::endl;
        return 1;
    }
    
    // 处理路径
    fs::path root_dir("."), log_dir("./logs");
    try {
        root_dir = fs::path(root_str);
        log_dir = fs::path(log_str);
        
        // 验证路径是否存在并且是一个目录
        if (!fs::exists(root_dir)) {
            std::cerr << "错误：指定的路径不存在 '" << root_dir << "'" << std::endl;
            return 1;
        }
        if (fs::exists(log_dir) && !fs::is_directory(root_dir)) {
            std::cerr << "错误：指定的路径不是一个目录 '" << log_dir << "'" << std::endl;
            return 1;
        }
        if (!fs::exists(log_dir)) {
            fs::create_directory(log_dir);
        }
        if (!fs::is_directory(root_dir)) {
            std::cerr << "错误：指定的路径不是一个目录 '" << root_dir << "'" << std::endl;
            return 1;
        }
        
        // 转化为绝对路径
        root_dir = fs::canonical(root_dir);
        log_dir = fs::canonical(log_dir);
    } catch (const fs::filesystem_error& e) {
        std::cerr << "文件系统错误: " << e.what() << std::endl;
        return 1;
    } catch (const std::exception& e) {
        std::cerr << "错误: " << e.what() << std::endl;
        return 1;
    }

    std::cout << "\n\n项目运行时目录设置为: " << root_dir << "..."<< std::endl;
    std::cout << "项目日志目录设置为: " << log_dir << "..."<< std::endl;
    
    // 创建控制器并运行游戏
    auto controller = Controller::getInstance(levels[level], log_dir, root_dir);
    int runcode = controller->run();
    
    // 确保光标正常显示
    View::enableCursor();
    return runcode;
}

// 处理 test 命令
int handleTestCommand(int argc, char* argv[]) {
    Catch::Session session;
    
    // 让 Catch 解析命令行参数
    int returnCode = session.applyCommandLine(argc, argv);
    if (returnCode != 0) {
        return returnCode;
    }
    
    return session.run();
}

int main(int argc, char* argv[]) {
    // 检查运行环境
    envCheck();
    
    // 显示欢迎信息
    showWelcome();
    
    // 如果没有提供子命令，显示帮助信息
    if (argc < 2) {
        showMainHelp(argv[0]);
        return 1;
    }
    
    // 获取子命令
    std::string command = argv[1];
    
    // 根据子命令调用相应的处理函数
    if (command == "run") {
        // 处理 run 命令，跳过第一个参数（程序名）和第二个参数（命令名）
        return handleRunCommand(argc - 1, argv + 1);
    } else if (command == "test") {
        // 处理 test 命令，跳过第一个参数（程序名）和第二个参数（命令名）
        return handleTestCommand(argc - 1, argv + 1);
    } else if (command == "--help" || command == "-h") {
        // 显示主帮助信息
        showMainHelp(argv[0]);
        return 0;
    } else {
        // 未知命令
        std::cerr << "未知命令: " << command << std::endl;
        showMainHelp(argv[0]);
        return 1;
    }
}
