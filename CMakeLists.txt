cmake_minimum_required(VERSION 3.21)

# set the project name and version
project(TShell VERSION 1.0)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)


set(CMAKE_EXPORT_COMPILE_COMMANDS ON)


# 设置源文件目录
set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)


# 默认采用Release模式
if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()


add_subdirectory(lib)

# 添加可执行文件，并指定源文件
add_executable(${PROJECT_NAME}
    ${SRC_DIR}/main.cpp 
)

# 指定头文件目录
target_include_directories(
    ${PROJECT_NAME} 

    PUBLIC
    ${INCLUDE_DIR} 
)

# 链接动态库
target_link_libraries(
    ${PROJECT_NAME} 

    PRIVATE
    utilities
)
