file(GLOB SOURCES "${PROJECT_SOURCE_DIR}/tests/unit/*.cpp")
add_library(Test OBJECT ${SOURCES})

file(TO_CMAKE_PATH "${PROJECT_SOURCE_DIR}" ROOT_DIR)
string(REPLACE "\\" "/" ROOT_DIR "${ROOT_DIR}/")
target_compile_definitions(Test PRIVATE OUCSurvSim_VERSION="${OUCSurvSim_VERSION}" PUBLIC ROOT_DIR="${ROOT_DIR}")

# 检测是否为 Visual Studio 生成器
if(CMAKE_GENERATOR MATCHES "Visual Studio" OR (CMAKE_GENERATOR STREQUAL "Ninja" AND (MSVC OR DEFINED CMAKE_VS_INSTALL_DIR OR DEFINED ENV{VisualStudioVersion})))
    message(STATUS "检测到 Visual Studio 生成器: ${CMAKE_GENERATOR}")
else()
    message(STATUS "平台检测:${CMAKE_GENERATOR}")
    # 编译模式下开启警告和调试信息
    target_compile_options(Test
        PRIVATE -g -Wall -Wextra
    )
endif()
# 包含第三方头文件
target_include_directories(Test PUBLIC "${PROJECT_SOURCE_DIR}/Third")
# 包含主要代码头文件
target_include_directories(Test PUBLIC "${PROJECT_SOURCE_DIR}/src")