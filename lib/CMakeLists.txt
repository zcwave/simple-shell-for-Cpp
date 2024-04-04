# 添加 utilities 动态库
add_library(utilities SHARED
    utilities.cpp
    utilities.h
)

# 指定 utilities 库的包含目录
target_include_directories(utilities PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)
