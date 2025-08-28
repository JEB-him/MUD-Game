add_library(CatchMain STATIC
    test_main.cpp       # 包含 CATCH_CONFIG_MAIN，是测试的入口
)

add_executable(test
    unit/Map-test.cpp   # Map 测试模块
)

target_compile_options( CatchMain
    PRIVATE -Wall
)

target_compile_options( test
    PRIVATE -Wall -Wextra
)

# 包含 Catch2 头文件
target_include_directories(CatchMain PUBLIC "${PROJECT_SOURCE_DIR}/Catch")
# 包含项目库的头文件
target_include_directories(test PRIVATE "${PROJECT_SOURCE_DIR}/src")
# 链接项目库
target_link_libraries(test PRIVATE OUCSurvSim)
# 链接 CatchMain 库
target_link_libraries(test PRIVATE CatchMain)

install(TARGETS test
        RUNTIME DESTINATION ".")
