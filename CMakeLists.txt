# This file is part of the
# ██████╗ ██╗     ██╗  ██╗██╗    ██╗ ██████╗ ██╗     ███████╗
# ██╔══██╗██║     ╚██╗██╔╝██║    ██║██╔═══██╗██║     ██╔════╝
# ██████╔╝██║      ╚███╔╝ ██║ █╗ ██║██║   ██║██║     █████╗  
# ██╔═══╝ ██║      ██╔██╗ ██║███╗██║██║   ██║██║     ██╔══╝  
# ██║     ███████╗██╔╝ ██╗╚███╔███╔╝╚██████╔╝███████╗██║     
# ╚═╝     ╚══════╝╚═╝  ╚═╝ ╚══╝╚══╝  ╚═════╝ ╚══════╝╚═╝     
# project
# https://github.com/jacmoe/pxlwolf
#
# (c) 2020 Jacob Moena
#
# BSD 2-Clause "Simplified" License
#
cmake_minimum_required(VERSION 3.7)
set(CMAKE_TOOLCHAIN_FILE ${CMAKE_CURRENT_SOURCE_DIR}/vcpkg/scripts/buildsystems/vcpkg.cmake
  CACHE STRING "Vcpkg toolchain file")

project(pixelwolf)

set(CMAKE_CXX_STANDARD 17)

if(MSVC)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif()

set(THREADS_PREFER_PTHREAD_FLAD ON)
find_package(Threads REQUIRED)

find_package(OpenGL REQUIRED)

if(WIN32)
include_directories(${WinSDK})
set(SDL2_DIR ${CMAKE_CURRENT_SOURCE_DIR}/vcpkg/installed/x64-windows/share/sdl2)
set(sdl2-image_DIR ${CMAKE_CURRENT_SOURCE_DIR}/vcpkg/installed/x64-windows/share/sdl2-image)
set(SDL2_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/vcpkg/installed/x64-windows/include/SDL2)

set(SDL2_LIBS_RELEASE
    ${CMAKE_CURRENT_SOURCE_DIR}/vcpkg/installed/x64-windows/lib/SDL2.lib
    ${CMAKE_CURRENT_SOURCE_DIR}/vcpkg/installed/x64-windows/lib/SDL2_image.lib
    ${CMAKE_CURRENT_SOURCE_DIR}/vcpkg/installed/x64-windows/lib/manual-link/SDL2main.lib
)
set(SDL2_LIBS_DEBUG
    ${CMAKE_CURRENT_SOURCE_DIR}/vcpkg/installed/x64-windows/debug/lib/SDL2d.lib
    ${CMAKE_CURRENT_SOURCE_DIR}/vcpkg/installed/x64-windows/debug/lib/SDL2_image.lib
    ${CMAKE_CURRENT_SOURCE_DIR}/vcpkg/installed/x64-windows/debug/lib/manual-link/SDL2maind.lib
)
endif(WIN32)
if(UNIX)
set(SDL2_DIR ${CMAKE_CURRENT_SOURCE_DIR}/vcpkg/installed/x64-linux/share/sdl2)
set(sdl2-image_DIR ${CMAKE_CURRENT_SOURCE_DIR}/vcpkg/installed/x64-linux/share/sdl2-image)
set(SDL2_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/vcpkg/installed/x64-linux/include/SDL2)

set(SDL2_LIBS_RELEASE
    ${CMAKE_CURRENT_SOURCE_DIR}/vcpkg/installed/x64-linux/lib/libSDL2.a
    ${CMAKE_CURRENT_SOURCE_DIR}/vcpkg/installed/x64-linux/lib/libSDL2_image.a
    ${CMAKE_CURRENT_SOURCE_DIR}/vcpkg/installed/x64-linux/lib/libSDL2main.a
)
set(SDL2_LIBS_DEBUG
    ${CMAKE_CURRENT_SOURCE_DIR}/vcpkg/installed/x64-linux/debug/lib/libSDL2d.a
    ${CMAKE_CURRENT_SOURCE_DIR}/vcpkg/installed/x64-linux/debug/lib/libSDL2_image.a
    ${CMAKE_CURRENT_SOURCE_DIR}/vcpkg/installed/x64-linux/debug/lib/libSDL2maind.a
)
endif(UNIX)


find_package(SDL2 CONFIG REQUIRED)
find_package(sdl2-image CONFIG REQUIRED)


set(IncludeDir code/include/) # Change include/ to your include directory

include_directories(
    ${SDL2_INCLUDE_DIRS}
    ${IncludeDir}
)

add_subdirectory(code)
