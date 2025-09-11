# 这个是运行游戏的入口
add_executable(game main.cpp)

# 将主要代码编译为静态库
# 这个地方为了大家编码方便，用了一个不太规范的方法来导入源文件
file(GLOB SOURCES "*.cpp")
# 需要排除的文件（多个文件写在一个 list 里）
set(EXCLUDED_FILES
    "${PROJECT_SOURCE_DIR}/src/main.cpp"
)

# 从 SOURCES 中移除
list(REMOVE_ITEM SOURCES ${EXCLUDED_FILES})
add_library(OUCSurvSim STATIC ${SOURCES})

file(TO_CMAKE_PATH "${PROJECT_SOURCE_DIR}" ROOT_DIR)
string(REPLACE "\\" "/" ROOT_DIR "${ROOT_DIR}/")

# 为 OUCSurvSim 添加版本号宏以及根目录,根目录宏定义后续将会移除，转而使用 Controller 的 root_dir
target_compile_definitions(OUCSurvSim PRIVATE OUCSurvSim_VERSION="${OUCSurvSim_VERSION}" PUBLIC ROOT_DIR="${ROOT_DIR}")
# 包含第三方头文件
target_include_directories(OUCSurvSim PUBLIC "${PROJECT_SOURCE_DIR}/Third")
# 将 OUC, Test 静态库链接到 game
target_link_libraries(game PRIVATE OUCSurvSim)
target_sources(game PRIVATE $<TARGET_OBJECTS:Test>)

# Curses
find_package(Curses REQUIRED)
include_directories(${CURSES_INCLUDE_DIR})
target_link_libraries(OUCSurvSim PRIVATE ${CURSES_LIBRARIES})
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
