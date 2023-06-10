cmake_minimum_required(VERSION 3.9)
project(artificial_nds)

set(CMAKE_CXX_STANDARD 23)

# Set to your devkitPro's installation dir if environment variable does not exist.
set(DEVKITPRO $ENV{DEVKITPRO})



add_definitions(-DARM9 -DARM7 -D_DEBUG -DWIN32)



#SET(CMAKE_C_COMPILER "${DEVKITPRO}/devkitARM/bin/arm-none-eabi-gcc")
#SET(CMAKE_CXX_COMPILER "${DEVKITPRO}/devkitARM/bin/arm-none-eabi-g++")
#SET(CMAKE_C_FLAGS "-g -Wall -O3 -marm -mthumb-interwork -march=armv5te -mtune=arm946e-s")
#SET(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS} -fno-rtti -fno-exceptions")

include_directories(${DEVKITPRO}/devkitARM)
include_directories(${DEVKITPRO}/libnds/lib)
include_directories(${DEVKITPRO}/libnds/include)
#include_directories(${DEVKITPRO}/libnds/include/nds)
#include_directories(${DEVKITPRO}/libnds/include/nds/arm9)
include_directories(${DEVKITPRO}/nflib/include)
include_directories(${DEVKITPRO}/devkitARM/arm-none-eabi/include/)

#link_directories(${DEVKITPRO}/libnds/lib)
#link_directories(${DEVKITPRO}/libnds/include)
#link_directories(${DEVKITPRO}/libnds/include/nds)
#link_directories(${DEVKITPRO}/libnds/include/nds/arm9)
#include_directories(${DEVKITPRO}/devkitARM)
#link_directories(${DEVKITPRO}/nflib/include)
#link_directories(${DEVKITPRO}/devkitARM/arm-none-eabi/include/)


add_executable(artificial_nds
        source/utils.h
        source/main.cpp source/instructions.h source/view.h source/gui_sprites.h source/backgrounds.h source/text_console.h source/imgdraw_test.h)