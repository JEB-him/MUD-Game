# 添加 CMake 最小依赖版本
cmake_minimum_required(VERSION 3.20)

# 项目名
project(OUCSurvSim VERSION 0.0.1)

# 指定 C++ 标准
set(CMAKE_CXX_STANDARD 20)
# 要求指定标准
set(CMAKE_CXX_STANDARD_REQUIRED True)
# 针对 VS2022 强制要求使用 UTF-8
if(CMAKE_GENERATOR MATCHES "Visual Studio" OR (CMAKE_GENERATOR STREQUAL "Ninja" AND (MSVC OR DEFINED CMAKE_VS_INSTALL_DIR OR DEFINED ENV{VisualStudioVersion})))
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /utf-8")
endif()
# 主要代码所在目录，将会生成游戏运行时的可执行文件
add_subdirectory(tests)
add_subdirectory(src)

