cmake_minimum_required(VERSION 3.10)
project(led-driver C)

set(KDIR "/mnt/otherdata/rk3588_workspace/ELF2-linux-source/kernel")
set(ARCH "arm64")
set(CROSS_COMPILE "/mnt/otherdata/rk3588_workspace/ELF2-linux-source/prebuilts/gcc/linux-x86/aarch64/gcc-arm-10.3-2021.07-x86_64-aarch64-none-linux-gnu/bin/aarch64-none-linux-gnu-")

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_definitions(-D__KERNEL__ -DMODULE)

include_directories(
    ${KDIR}/include
    ${KDIR}/include/uapi
    ${KDIR}/include/generated
    ${KDIR}/include/generated/uapi

    ${KDIR}/arch/${ARCH}/include
    ${KDIR}/arch/${ARCH}/include/uapi
    ${KDIR}/arch/${ARCH}/include/generated
    ${KDIR}/arch/${ARCH}/include/generated/uapi
)

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/bsp_gpio
)

add_custom_target(my_driver ALL
    COMMAND make -C ${KDIR} M=${CMAKE_CURRENT_SOURCE_DIR}/bsp_gpio
            ARCH=${ARCH} CROSS_COMPILE=${CROSS_COMPILE} modules
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bsp_gpio
    COMMENT "Building Linux Kernel Module..."
)
 
add_custom_target(my_driver_clean
    COMMAND make -C ${KDIR} M=${CMAKE_CURRENT_SOURCE_DIR}/bsp_gpio
            ARCH=${ARCH} CROSS_COMPILE=${CROSS_COMPILE} clean
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bsp_gpio
    COMMENT "Cleaning Linux Kernel Module build files..."
)