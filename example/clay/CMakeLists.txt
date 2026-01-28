cmake_minimum_required(VERSION 4.1)
project(dyc)

set(CMAKE_CXX_STANDARD 17)

# 确保所有目标（包括静态库）都生成地址无关代码
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

set(CORE_SOURCES
    helloworld.pb.cc
    helloworld_export.cpp
    helloworld_service.cpp
    my_app.cpp
)

add_library(dyc SHARED ${CORE_SOURCES})

if(MSVC)
    # 强制强制 MSVC 使用动态链接运行时 (MD/MDd)
    set(protobuf_MSVC_STATIC_RUNTIME OFF CACHE BOOL "Link protobuf to runtime statically" FORCE)

    # 对于你自己的项目目标
    # 当你选 Debug 模式编译时，组合结果为：MultiThreadedDebugDLL (即 MDd)。
    # 当你选 Release 模式编译时，组合结果为：MultiThreadedDLL (即 MD)
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL")
endif()

# 禁用 Protobuf 自带的测试和安装逻辑，加快编译速度并避免干扰
set(protobuf_BUILD_TESTS OFF CACHE BOOL "Build tests" FORCE)
set(protobuf_BUILD_EXPORT_SAMPLES OFF CACHE BOOL "Build samples" FORCE)
add_subdirectory(3rdparty/protobuf)

if(${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
    # 特定的 Linux 配置
    if (${CMAKE_SYSTEM_PROCESSOR} STREQUAL "arm64")
        message(STATUS "正在使用 Linux arm64 编译器")
    else()
        message(STATUS "正在使用 Linux x86_64 编译器")
    endif()
elseif (${CMAKE_SYSTEM_NAME} STREQUAL "Windows")
    # 特定的 Windows 配置
    message(STATUS "正在使用 Windows 编译器")
elseif (${CMAKE_SYSTEM_NAME} STREQUAL "Darwin")
    # 特定的 macOS 配置
    message(STATUS "正在使用 macOS 编译器")
elseif (${CMAKE_SYSTEM_NAME} STREQUAL "Android")
    # 特定的 Android 配置
    message(STATUS "正在使用 Android 编译器")
endif()

target_include_directories(dyc PRIVATE ${INC_DIRS})
target_link_directories(dyc PRIVATE ${LIB_DIRS})

if(${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
    target_link_libraries(dyc PRIVATE libprotobuf pthread)
elseif (${CMAKE_SYSTEM_NAME} STREQUAL "Windows")
    target_link_libraries(dyc PRIVATE libprotobuf)
endif()
