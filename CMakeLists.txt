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

# 这个是运行游戏的入口
add_executable(game src/main.cpp)
file(GLOB SOURCES "src/*.cpp")
# 需要排除的文件（多个文件写在一个 list 里）
set(EXCLUDED_FILES
)

# 从 SOURCES 中移除
list(REMOVE_ITEM SOURCES ${EXCLUDED_FILES})
target_sources(game PUBLIC ${SOURCES})
file(GLOB TEST_SOURCES "${PROJECT_SOURCE_DIR}/tests/unit/*.cpp")
target_sources(game PUBLIC ${TEST_SOURCES})

file(TO_CMAKE_PATH "${PROJECT_SOURCE_DIR}" ROOT_DIR)
string(REPLACE "\\" "/" ROOT_DIR "${ROOT_DIR}/")

# 为 OUCSurvSim 添加版本号宏以及根目录,根目录宏定义后续将会移除，转而使用 Controller 的 root_dir
target_compile_definitions(game PRIVATE OUCSurvSim_VERSION="${OUCSurvSim_VERSION}" PUBLIC ROOT_DIR="${ROOT_DIR}")

# 包含第三方头文件
target_include_directories(game PUBLIC "${PROJECT_SOURCE_DIR}/Third")
# 包含主要代码头文件
target_include_directories(game PUBLIC "${PROJECT_SOURCE_DIR}/src")

# Curses
find_package(Curses REQUIRED)
include_directories(${CURSES_INCLUDE_DIR})
target_link_libraries(game PRIVATE ${CURSES_LIBRARIES})

# 检测是否为 Visual Studio 生成器
if(CMAKE_GENERATOR MATCHES "Visual Studio" OR (CMAKE_GENERATOR STREQUAL "Ninja" AND (MSVC OR DEFINED CMAKE_VS_INSTALL_DIR OR DEFINED ENV{VisualStudioVersion})))
    message(STATUS "检测到 Visual Studio 生成器: ${CMAKE_GENERATOR}")
else()
    message(STATUS "平台检测:${CMAKE_GENERATOR}")
    # 编译模式下开启警告和调试信息
    target_compile_options(game
        PRIVATE -g -Wall -Wextra
    )
endif()

install(TARGETS game
        RUNTIME DESTINATION ".")
