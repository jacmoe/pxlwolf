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

project(pixelwolf)

if(MSVC)
    add_compile_options(
        $<$<CONFIG:>:/MT> #---------|
        $<$<CONFIG:Debug>:/MTd> #---|-- Statically link the runtime libraries
        $<$<CONFIG:Release>:/MT> #--|
    )
endif()

set(CMAKE_CXX_STANDARD 17)

if(MSVC)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
endif()

add_subdirectory(3rdparty/Lua)

add_subdirectory(3rdparty/physfs)

set(COSTUMIZE_BUILD TRUE)
#set(USE_AUDIO FALSE)
set(USE_MSVC_RUNTIME_LIBRARY_DLL FALSE)
add_subdirectory(3rdparty/raylib)

set(3RD_PARTY_DIR ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty)
set(RAYLIB_INCLUDE_DIR ${3RD_PARTY_DIR}/raylib/src)
set(RAYLIBCPP_INCLUDE_DIR ${3RD_PARTY_DIR}/raylib-cpp)
set(TOML11_INCLUDE_DIR ${3RD_PARTY_DIR}/toml11)
set(SOL_INCLUDE_DIR ${3RD_PARTY_DIR}/sol2)
set(ENTT_INCLUDE_DIR ${3RD_PARTY_DIR}/entt)
set(LUA_INCLUDE_DIR ${3RD_PARTY_DIR}/Lua/include)
set(PHYSFS_INCLUDE_DIR ${3RD_PARTY_DIR}/physfs/src)

include_directories(
    ${3RD_PARTY_DIR}
    ${RAYLIB_INCLUDE_DIR}
    ${RAYLIBCPP_INCLUDE_DIR}
    ${TOML11_INCLUDE_DIR}
    ${ENTT_INCLUDE_DIR}
    ${LUA_INCLUDE_DIR}
    ${SOL_INCLUDE_DIR}
    ${PHYSFS_INCLUDE_DIR}
    ./code/main
    ./code/components
    ./code/systems
    ./code/utility
)

add_subdirectory(code)
