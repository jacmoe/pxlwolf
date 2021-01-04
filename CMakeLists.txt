# This file is part of the
# ██████╗ ██╗  ██╗██╗     ██╗    ██╗ ██████╗ ██╗     ███████╗
# ██╔══██╗╚██╗██╔╝██║     ██║    ██║██╔═══██╗██║     ██╔════╝
# ██████╔╝ ╚███╔╝ ██║     ██║ █╗ ██║██║   ██║██║     █████╗  
# ██╔═══╝  ██╔██╗ ██║     ██║███╗██║██║   ██║██║     ██╔══╝  
# ██║     ██╔╝ ██╗███████╗╚███╔███╔╝╚██████╔╝███████╗██║     
# ╚═╝     ╚═╝  ╚═╝╚══════╝ ╚══╝╚══╝  ╚═════╝ ╚══════╝╚═╝     
#   project
#
#   https://github.com/jacmoe/pxlwolf
#
#   (c) 2020 - 2021 Jacob Moena
#
#   MIT License
#
cmake_minimum_required(VERSION 3.7)
set(CMAKE_TOOLCHAIN_FILE ${CMAKE_CURRENT_SOURCE_DIR}/vcpkg/scripts/buildsystems/vcpkg.cmake
  CACHE STRING "Vcpkg toolchain file")

project(pixelwolf)

set(CMAKE_CXX_STANDARD 17)

if(UNIX)
    set(THREADS_PREFER_PTHREAD_FLAD ON)
    find_package(Threads REQUIRED)
    find_package(X11 REQUIRED)
    find_package(PNG REQUIRED)
    include_directories(${PNG_INCLUDE_DIR})
endif(UNIX)

if(MSVC)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif()

if(WIN32)
include_directories(${WinSDK})
set(SDL2_DIR ${CMAKE_CURRENT_SOURCE_DIR}/vcpkg/installed/x64-windows-static/share/sdl2)
set(SDL2_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/vcpkg/installed/x64-windows-static/include/SDL2)
set(SOL_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/vcpkg/installed/x64-windows-static/include/sol)
set(LUA_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/vcpkg/installed/x64-windows-static/include)

set(LIBS_RELEASE
    dinput8
    shell32
    setupapi
    advapi32
    uuid
    version
    oleaut32
    ole32
    imm32
    winmm
    gdi32
    user32 
    ${CMAKE_CURRENT_SOURCE_DIR}/vcpkg/installed/x64-windows-static/lib/manual-link/SDL2main.lib
    ${CMAKE_CURRENT_SOURCE_DIR}/vcpkg/installed/x64-windows-static/lib/SDL2.lib
    ${CMAKE_CURRENT_SOURCE_DIR}/vcpkg/installed/x64-windows-static/lib/SDL2_ttf.lib
    ${CMAKE_CURRENT_SOURCE_DIR}/vcpkg/installed/x64-windows-static/lib/lua.lib
    ${CMAKE_CURRENT_SOURCE_DIR}/vcpkg/installed/x64-windows-static/lib/freetype.lib
    ${CMAKE_CURRENT_SOURCE_DIR}/vcpkg/installed/x64-windows-static/lib/zlib.lib
    ${CMAKE_CURRENT_SOURCE_DIR}/vcpkg/installed/x64-windows-static/lib/bz2.lib
    ${CMAKE_CURRENT_SOURCE_DIR}/vcpkg/installed/x64-windows-static/lib/libpng16.lib
    ${CMAKE_CURRENT_SOURCE_DIR}/vcpkg/installed/x64-windows-static/lib/zlib.lib
    ${CMAKE_CURRENT_SOURCE_DIR}/vcpkg/installed/x64-windows-static/lib/brotlidec-static.lib
    ${CMAKE_CURRENT_SOURCE_DIR}/vcpkg/installed/x64-windows-static/lib/brotlicommon-static.lib
    ${CMAKE_CURRENT_SOURCE_DIR}/vcpkg/installed/x64-windows-static/lib/physfs-static.lib
)

set(LIBS_DEBUG
    dinput8
    shell32
    setupapi
    advapi32
    uuid
    version
    oleaut32
    ole32
    imm32
    winmm
    gdi32
    user32 
    ${CMAKE_CURRENT_SOURCE_DIR}/vcpkg/installed/x64-windows-static/debug/lib/SDL2d.lib
    ${CMAKE_CURRENT_SOURCE_DIR}/vcpkg/installed/x64-windows-static/debug/lib/manual-link/SDL2maind.lib
    ${CMAKE_CURRENT_SOURCE_DIR}/vcpkg/installed/x64-windows-static/debug/lib/SDL2_ttf.lib
    ${CMAKE_CURRENT_SOURCE_DIR}/vcpkg/installed/x64-windows-static/debug/lib/lua.lib
    ${CMAKE_CURRENT_SOURCE_DIR}/vcpkg/installed/x64-windows-static/debug/lib/freetyped.lib
    ${CMAKE_CURRENT_SOURCE_DIR}/vcpkg/installed/x64-windows-static/debug/lib/zlibd.lib
    ${CMAKE_CURRENT_SOURCE_DIR}/vcpkg/installed/x64-windows-static/debug/lib/bz2d.lib
    ${CMAKE_CURRENT_SOURCE_DIR}/vcpkg/installed/x64-windows-static/debug/lib/libpng16d.lib
    ${CMAKE_CURRENT_SOURCE_DIR}/vcpkg/installed/x64-windows-static/debug/lib/zlibd.lib
    ${CMAKE_CURRENT_SOURCE_DIR}/vcpkg/installed/x64-windows-static/debug/lib/brotlidec-static.lib
    ${CMAKE_CURRENT_SOURCE_DIR}/vcpkg/installed/x64-windows-static/debug/lib/brotlicommon-static.lib
    ${CMAKE_CURRENT_SOURCE_DIR}/vcpkg/installed/x64-windows-static/debug/lib/physfs-static.lib
)
endif(WIN32)
if(UNIX)
set(SDL2_DIR ${CMAKE_CURRENT_SOURCE_DIR}/vcpkg/installed/x64-linux/share/sdl2)
set(SDL2_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/vcpkg/installed/x64-linux/include/SDL2)
set(SOL_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/vcpkg/installed/x64-linux/include/sol)
set(LUA_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/vcpkg/installed/x64-linux/include)

set(LIBS_RELEASE
    ${CMAKE_CURRENT_SOURCE_DIR}/vcpkg/installed/x64-linux/lib/libSDL2.a
    ${CMAKE_CURRENT_SOURCE_DIR}/vcpkg/installed/x64-linux/lib/libSDL2main.a
    ${CMAKE_CURRENT_SOURCE_DIR}/vcpkg/installed/x64-linux/lib/libSDL2_ttf.a
    ${CMAKE_CURRENT_SOURCE_DIR}/vcpkg/installed/x64-linux/lib/liblua.a
    ${CMAKE_CURRENT_SOURCE_DIR}/vcpkg/installed/x64-linux/lib/liblua.a
    ${CMAKE_CURRENT_SOURCE_DIR}/vcpkg/installed/x64-linux/lib/libfreetype.a
    ${CMAKE_CURRENT_SOURCE_DIR}/vcpkg/installed/x64-linux/lib/libz.a
    ${CMAKE_CURRENT_SOURCE_DIR}/vcpkg/installed/x64-linux/lib/libbz2.a
    ${CMAKE_CURRENT_SOURCE_DIR}/vcpkg/installed/x64-linux/lib/libbrotlidec-static.a
    ${CMAKE_CURRENT_SOURCE_DIR}/vcpkg/installed/x64-linux/lib/libbrotlicommon-static.a
    ${CMAKE_CURRENT_SOURCE_DIR}/vcpkg/installed/x64-linux/lib/libphysfs.a
    dl
    png
)
set(LIBS_DEBUG
    ${CMAKE_CURRENT_SOURCE_DIR}/vcpkg/installed/x64-linux/debug/lib/libSDL2d.a
    ${CMAKE_CURRENT_SOURCE_DIR}/vcpkg/installed/x64-linux/debug/lib/libSDL2maind.a
    ${CMAKE_CURRENT_SOURCE_DIR}/vcpkg/installed/x64-linux/debug/lib/libSDL2_ttf.a
    ${CMAKE_CURRENT_SOURCE_DIR}/vcpkg/installed/x64-linux/debug/lib/liblua.a
    ${CMAKE_CURRENT_SOURCE_DIR}/vcpkg/installed/x64-linux/debug/lib/libfreetyped.a
    ${CMAKE_CURRENT_SOURCE_DIR}/vcpkg/installed/x64-linux/debug/lib/libz.a
    ${CMAKE_CURRENT_SOURCE_DIR}/vcpkg/installed/x64-linux/debug/lib/libbz2d.a
    ${CMAKE_CURRENT_SOURCE_DIR}/vcpkg/installed/x64-linux/debug/lib/libbrotlidec-static.a
    ${CMAKE_CURRENT_SOURCE_DIR}/vcpkg/installed/x64-linux/debug/lib/libbrotlicommon-static.a
    ${CMAKE_CURRENT_SOURCE_DIR}/vcpkg/installed/x64-linux/debug/lib/libphysfs.a
    dl
    png
)
endif(UNIX)

set(IncludeDir code/include/)

include_directories(
    ${SDL2_INCLUDE_DIRS}
    ${SOL_INCLUDE_DIRS}
    ${LUA_INCLUDE_DIRS}
    ${IncludeDir}
)

add_subdirectory(code)
