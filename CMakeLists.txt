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

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

find_package(Allegro5 REQUIRED)

add_subdirectory(3rdparty/Lua)

add_subdirectory(3rdparty/physfs)

set(3RD_PARTY_DIR ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty)
set(TOML11_INCLUDE_DIR ${3RD_PARTY_DIR}/toml11)
set(SOL_INCLUDE_DIR ${3RD_PARTY_DIR}/sol2)
set(ENTT_INCLUDE_DIR ${3RD_PARTY_DIR}/entt)
set(LUA_INCLUDE_DIR ${3RD_PARTY_DIR}/Lua/include)
set(PHYSFS_INCLUDE_DIR ${3RD_PARTY_DIR}/physfs/src)
set(SPDLOG_INCLUDE_DIR ${3RD_PARTY_DIR}/spdlog/include)

include_directories(
    ${ALLEGRO_INCLUDE_DIR}
    ${3RD_PARTY_DIR}
    ${SPDLOG_INCLUDE_DIR}
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
