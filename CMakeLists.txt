cmake_minimum_required(VERSION 3.21)

# 设置项目名称和版本
project(TShell VERSION 1.0)

# 指定 C++ 标准
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# 导出编译命令
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# 设置源文件目录
set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)

# 默认构建类型为 Debug
if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

# 添加 lib 文件夹下的 CMakeLists.txt
add_subdirectory(lib)

# 添加可执行文件，并指定源文件
add_executable(${PROJECT_NAME}
    ${SRC_DIR}/main.cpp 
    ${SRC_DIR}/tsh.cpp 
    ${SRC_DIR}/signal_handlers.cpp 
    ${SRC_DIR}/Jobs.cpp 
)

# 指定头文件目录
target_include_directories(${PROJECT_NAME} 
    PUBLIC
    ${INCLUDE_DIR} 
)

# 链接动态库
target_link_libraries(${PROJECT_NAME} 
    PRIVATE
    utilities
)
