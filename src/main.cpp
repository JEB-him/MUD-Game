#define CATCH_CONFIG_RUNNER
#include <iostream>
#include <fstream>
#include <unordered_map>
#include "Controller.h"
#include "catch.hpp"
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
    std::cout << View::MOVU;
#endif
}

int main(int argc, char* argv[]) {
    // 检查运行环境
    envCheck();

    namespace fs = std::filesystem;
    std::unordered_map<std::string, Controller::LogLevel> levels {
        {"DEBUG", Controller::LogLevel::DEBUG},
        {"INFO", Controller::LogLevel::INFO},
        {"WARN", Controller::LogLevel::WARN},
        {"ERROR", Controller::LogLevel::ERROR}
    };

    std::string root_str="./", level="INFO", log_str = "logs/", program = "";
    bool help = false;
    Catch::Session session;  // There must be exactly one instance

    // Build a new parser on top of Catch's
    using namespace Catch::clara;

    std::ifstream welcome_file("static/Welcome.txt");
    std::string line;
    if (!welcome_file.is_open()) {
        std::cout << "文件打开失败，请检查项目是否完整";
    }
    while(std::getline(welcome_file, line)) {
<<<<<<< HEAD
        std::cout << line << endl;
=======
        std::cout << line << std::endl;
>>>>>>> origin/feature-controller
    }

    auto cli = Opt(root_str, "root directory")["-r"]["--root"]("所有配置文件的根目录(使用/)") |
               Opt(log_str, "log directory")["-g"]["--logs"]("日志文件输出目录(使用/)") |
               Opt(level, "log level")["-p"]["--glevel"]("日志等级\n决定日志的详细程度") |
               Help(help)|
               Arg(program, "test|run")("测试/进行游戏").required();
    auto result = cli.parse( Args( argc, argv ) );
    if( !result || help || program=="" || (program != "run" && program != "test")) {
        std::cout << "================================== Help =====================================" << std::endl;
        // 输出帮助信息
        if(program == "test")
            std::cout << session.cli() << std::endl;
        else
            std::cout << cli << std::endl;
        std::cout << "Use `" << argv[0] << " test --help` to get more options for test." << std::endl;
        std::cout << "================================== End =======================================" << std::endl;
        if (!result) std::cerr << "Error in command line: " << result.errorMessage() << std::endl;
        std::cout << "\n[38;5;45mSimple usage: " << argv[0] << " run[0m" << std::endl;
        std::cout << "[38;5;45mSimple usage: " << argv[0] << " test[0m" << std::endl;
        exit(1);
    }
    std::cout << "[38;5;8m================================== End =======================================[0m" << std::endl;

    // Let Catch (using Clara) parse the command line
    int returnCode = session.applyCommandLine(argc - 1, argv + 1);
    if (returnCode != 0) {
        return returnCode;
    }

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

    std::cout << "\n项目运行时目录设置为: " << root_dir << "..."<< std::endl;
    std::cout << "\n项目运行时目录设置为: " << log_dir << "..."<< std::endl;
    // 如果是测试
    if (program == "test") {
        for (int i = 0;i < argc; ++i) {
            std::cout << argv[i] << std::endl;
        }
        return session.run();
    } else if (program == "run") {
        auto controller = Controller::getInstance(levels[level], log_dir, root_dir);
        return controller->run();
    }
    if (help) {
        std::cout << "Documentation: " << root_dir.generic_string() << "/docs/html/index.html" << std::endl;
    }
}
