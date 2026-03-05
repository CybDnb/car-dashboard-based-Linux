cmake_minimum_required(VERSION 3.10)
project(LOG C)

add_library(log STATIC
    Src/log.c
)

target_include_directories(log PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/Src
    ${CMAKE_CURRENT_SOURCE_DIR}/platform_adapt
)

if (CMAKE_SOURCE_DIR STREQUAL PROJECT_SOURCE_DIR)
    add_executable(log_demo test.c)
    target_link_libraries(log_demo PRIVATE log)
endif()
