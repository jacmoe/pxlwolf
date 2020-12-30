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
#   (c) 2020 Jacob Moena
#
#   MIT License
#
cmake_minimum_required(VERSION 3.7)
set(CMAKE_TOOLCHAIN_FILE ${CMAKE_CURRENT_SOURCE_DIR}/vcpkg/scripts/buildsystems/vcpkg.cmake
  CACHE STRING "Vcpkg toolchain file")

project(pixelwolf)

set(CMAKE_CXX_STANDARD 17)

find_package(OpenGL REQUIRED)

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
#set(GL_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/vcpkg/installed/x64-windows-static/include/gl)

set(SDL2_LIBS_RELEASE
    #${CMAKE_CURRENT_SOURCE_DIR}/vcpkg/installed/x64-windows-static/lib/GLU32.lib
    #${CMAKE_CURRENT_SOURCE_DIR}/vcpkg/installed/x64-windows-static/lib/OpenGL32.lib
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
)
set(SDL2_LIBS_DEBUG
    #${CMAKE_CURRENT_SOURCE_DIR}/vcpkg/installed/x64-windows-static/debug/lib/GLU32.lib
    #${CMAKE_CURRENT_SOURCE_DIR}/vcpkg/installed/x64-windows-static/debug/lib/OpenGL32.lib
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
)
endif(WIN32)
if(UNIX)
set(SDL2_DIR ${CMAKE_CURRENT_SOURCE_DIR}/vcpkg/installed/x64-linux/share/sdl2)
set(SDL2_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/vcpkg/installed/x64-linux/include/SDL2)
set(GL_INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/vcpkg/installed/x64-linux/include/GL)

set(SDL2_LIBS_RELEASE
    ${CMAKE_CURRENT_SOURCE_DIR}/vcpkg/installed/x64-linux/lib/libSDL2.a
    ${CMAKE_CURRENT_SOURCE_DIR}/vcpkg/installed/x64-linux/lib/libSDL2main.a
    dl
    png
)
set(SDL2_LIBS_DEBUG
    ${CMAKE_CURRENT_SOURCE_DIR}/vcpkg/installed/x64-linux/debug/lib/libSDL2d.a
    ${CMAKE_CURRENT_SOURCE_DIR}/vcpkg/installed/x64-linux/debug/lib/libSDL2maind.a
    dl
    png
)
endif(UNIX)


find_package(SDL2 CONFIG REQUIRED)


set(IncludeDir code/include/) # Change include/ to your include directory

include_directories(
    ${GL_INCLUDE_DIRS}
    ${SDL2_INCLUDE_DIRS}
    ${IncludeDir}
)

add_subdirectory(code)
