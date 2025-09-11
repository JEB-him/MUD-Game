file(GLOB SOURCES "unit/*.cpp")
add_library(Test ${SOURCES})

if(CMAKE_GENERATOR MATCHES "Visual Studio" OR (CMAKE_GENERATOR STREQUAL "Ninja" AND (MSVC OR DEFINED CMAKE_VS_INSTALL_DIR OR DEFINED ENV{VisualStudioVersion})))
    message(STATUS "检测到 Visual Studio 生成器: ${CMAKE_GENERATOR}")
else()
    message(STATUS "平台检测${CMAKE_GENERATOR}")
    target_compile_options( Test
        PRIVATE -Wall -Wextra -g
    )
endif()

# 包含第三方头文件
target_include_directories(Test PUBLIC "${PROJECT_SOURCE_DIR}/Third")
# 包含项目库的头文件
target_include_directories(Test PRIVATE "${PROJECT_SOURCE_DIR}/src")
# 链接项目库
target_link_libraries(Test PRIVATE OUCSurvSim)
