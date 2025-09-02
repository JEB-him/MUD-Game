# 添加 CMake 最小依赖版本
cmake_minimum_required(VERSION 3.20)

# 项目名
project(OUCSurvSim VERSION 0.0.1)

# 指定 C++ 标准
set(CMAKE_CXX_STANDARD 20)
# 要求指定标准
set(CMAKE_CXX_STANDARD_REQUIRED True)

# 创建一个选项，是否编译测试文件
option(TEST "是否编译测试用例" True)
option(DEBUG "调试版本" True)

# 主要代码所在目录，将会生成游戏运行时的可执行文件
add_subdirectory(src)

if (TEST)
    # 测试文件所在目录，将会生成测试用的可执行文件
    add_subdirectory(tests)
elseif (DEBUG)
    message(WARGING "不编译测试用例，测试结果不针对最新版本")
endif()
