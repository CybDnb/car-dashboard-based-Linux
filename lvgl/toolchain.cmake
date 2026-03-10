# 目标操作系统名称 (必需)
# 例如: Linux, Windows, Generic (对于无操作系统的嵌入式设备) [citation:4][citation:8]
set(CMAKE_SYSTEM_NAME Linux)

# 目标系统处理器架构 (可选，但推荐设置)
# 例如: arm, aarch64, x86_64 [citation:2][citation:4]
set(CMAKE_SYSTEM_PROCESSOR aarch64)

# 指定 C 和 C++ 交叉编译器的路径 (必需)
# 可以使用完整路径或仅指定编译器名称（如果它位于系统 PATH 中） [citation:2][citation:8]
set(TOOLCHAIN_DIR /mnt/otherdata/rk3588_workspace/ELF2-linux-source/prebuilts/gcc/linux-x86/aarch64/gcc-arm-10.3-2021.07-x86_64-aarch64-none-linux-gnu)
set(CMAKE_C_COMPILER /mnt/otherdata/rk3588_workspace/ELF2-linux-source/prebuilts/gcc/linux-x86/aarch64/gcc-arm-10.3-2021.07-x86_64-aarch64-none-linux-gnu/bin/aarch64-none-linux-gnu-gcc)
set(CMAKE_CXX_COMPILER /mnt/otherdata/rk3588_workspace/ELF2-linux-source/prebuilts/gcc/linux-x86/aarch64/gcc-arm-10.3-2021.07-x86_64-aarch64-none-linux-gnu/bin/aarch64-none-linux-gnu-g++)
set(CMAKE_SYSROOT ${TOOLCHAIN_DIR}/aarch64-none-linux-gnu/libc)
set(CMAKE_FIND_ROOT_PATH ${CMAKE_SYSROOT})
# 控制 find_* 命令的搜索行为 [citation:2][citation:8][citation:10]
# 查找程序时，只在主机环境查找 (因为编译时需要运行的程序通常是主机工具)
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
# 查找库和头文件时，只在目标环境 (即 sysroot 和 CMAKE_FIND_ROOT_PATH) 查找
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY) # 用于 find_package [citation:5]

find_program(HOST_PKG_CONFIG pkg-config)
set(ENV{PKG_CONFIG_DIR} "")
set(ENV{PKG_CONFIG_LIBDIR} "${CMAKE_SYSROOT}/usr/lib/pkgconfig:${CMAKE_SYSROOT}/usr/share/pkgconfig")
set(ENV{PKG_CONFIG_SYSROOT_DIR} ${CMAKE_SYSROOT})

message(STATUS "Using pkg-config: ${PKG_CONFIG_EXECUTABLE}")
message(STATUS "PKG_CONFIG_LIBDIR: $ENV{PKG_CONFIG_LIBDIR}")