# 这个是运行游戏的入口
add_executable(game main.cpp)

# 将主要代码编译为静态库
# 这个地方为了大家编码方便，用了一个不太规范的方法来导入源文件
file(GLOB SOURCES "*.cpp")
file(GLOB_RECURSE excluded_files main.cpp)
list(REMOVE_ITEM SOURCES ${excluded_files})
add_library(OUCSurvSim STATIC ${SOURCES})

# 为 OUCSurvSim 添加版本号宏
target_compile_definitions(OUCSurvSim PRIVATE OUCSurvSim_VERSION="${OUCSurvSim_VERSION}")
# 包含第三方头文件
target_include_directories(OUCSurvSim PUBLIC "${PROJECT_SOURCE_DIR}/Third")
# 将 OUCSurvSim 静态库链接到 game
target_link_libraries(game PUBLIC OUCSurvSim)

# 编译模式下开启警告和调试信息
if (CACHE{DEBUG})
    target_compile_options(game
        PRIVATE -g -Wall -Wextra
    )
else()
    # release 启用编译优化
    target_compile_options(game
        PRIVATE -O3
    )
endif()

install(TARGETS game
        RUNTIME DESTINATION ".")
