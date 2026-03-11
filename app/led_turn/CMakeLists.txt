cmake_minimum_required(VERSION 3.10)
project(led_turn C)
set(CMAKE_C_COMPILER /mnt/otherdata/rk3588_workspace/ELF2-linux-source/prebuilts/gcc/linux-x86/aarch64/gcc-arm-10.3-2021.07-x86_64-aarch64-none-linux-gnu/bin/aarch64-none-linux-gnu-gcc)
set(CMAKE_CXX_STANDARD 11)

add_executable(led_turn app_led_turn.c)

target_include_directories(led_turn PRIVATE
    ${CMAKE_SOURCE_DIR}/include
)