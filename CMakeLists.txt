cmake_minimum_required(VERSION 3.10)
project(file-compressor VERSION 1.0)

# 设置C++标准
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 设置UTF-8编码
if(MSVC)
    add_compile_options(/utf-8)
endif()

# 获取源文件
set(SOURCES
    src/main.cpp
    src/Compressor.cpp
    src/HuffmanTree.cpp
)

# 添加头文件目录
include_directories(include)

# 创建可执行文件
add_executable(compressor ${SOURCES})

# 设置输出目录
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)

# 添加编译选项
if(MSVC)
    target_compile_options(compressor PRIVATE /W4)
else()
    target_compile_options(compressor PRIVATE -Wall -Wextra -Wpedantic)
endif()