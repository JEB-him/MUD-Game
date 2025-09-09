add_library(CatchMain STATIC
    test_main.cpp       # 包含 CATCH_CONFIG_MAIN，是测试的入口
)


file(GLOB SOURCES "unit/*.cpp")
add_executable(test ${SOURCES})

if(CMAKE_GENERATOR MATCHES "Visual Studio")
    message(STATUS "检测到 Visual Studio 生成器: ${CMAKE_GENERATOR}")
else()
    message(STATUS "其他平台")
    target_compile_options( CatchMain
        PRIVATE -Wall
    )

    target_compile_options( test
        PRIVATE -Wall -Wextra -g
    )
endif()

# 包含第三方头文件
target_include_directories(CatchMain PUBLIC "${PROJECT_SOURCE_DIR}/Third")
# 包含项目库的头文件
target_include_directories(test PRIVATE "${PROJECT_SOURCE_DIR}/src")
# 链接项目库
target_link_libraries(test PRIVATE OUCSurvSim)
# 链接 CatchMain 库
target_link_libraries(test PRIVATE CatchMain)

install(TARGETS test
        RUNTIME DESTINATION ".")

# 仅为 Windows 平台设置代码页
if(WIN32)
    # 设置代码页为 UTF-8 (65001)
    add_custom_command(TARGET test POST_BUILD
        COMMAND cmd /c "chcp 65001 > nul"
        COMMENT "Setting console code page to UTF-8"
    )
endif()
