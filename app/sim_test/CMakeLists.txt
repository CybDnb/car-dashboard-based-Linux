cmake_minimum_required(VERSION 3.10)
project(sim_test)

add_subdirectory(../../component/log ${CMAKE_CURRENT_BINARY_DIR}/component_log)

add_executable(app_sim_test ui_sim.c)

target_link_libraries(app_sim_test PRIVATE log)
