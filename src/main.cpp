#include <iostream>
#include "Map.h"
#include <time.h>
#if defined(_WIN32) && !defined(__linux__)
#   include <windows.h>
#endif
#if !defined(_WIN32) && defined(__linux__)
#   include <unistd.h>
#endif

using std::cin, std::cout;
int main() {
#if !defined(__GNUG__)
    std::cerr << "本项目仅在 GNU(Linux)/MinGW(Windows) 下测试通过" << std::endl;
    exit(-1);
#endif
#ifdef _WIN32
    // 启用虚拟终端
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD dwMode = 0;
    GetConsoleMode(hConsole, &dwMode);
    dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING; // 启用虚拟终端
    SetConsoleMode(hConsole, dwMode);
    system("chcp 65001");
#endif
    cout << "你好世界.\n";
    return 0;
}
