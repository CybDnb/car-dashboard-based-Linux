cmake_minimum_required(VERSION 3.10)
project(LinuxCarProject C)

message("CMAKE_SOURCE_DIR = ${CMAKE_SOURCE_DIR}")
# 输出: /mnt/otherdata/LinuxCarProject

message("PROJECT_SOURCE_DIR = ${PROJECT_SOURCE_DIR}")
# 输出: /mnt/otherdata/LinuxCarProject

# 设置项目根目录变量（自定义）
set(PROJECT_ROOT ${CMAKE_SOURCE_DIR})

add_subdirectory(app)