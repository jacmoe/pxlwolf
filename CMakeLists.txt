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
#   (c) 2020 - 2025 Jacob Moena
#
#   MIT License
#
cmake_minimum_required(VERSION 3.28)

project(pixelwolf)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_STANDARD 17)

include(FetchContent)

FetchContent_Declare(
  allegro5
  GIT_REPOSITORY https://github.com/jacmoe/allegro5.git
  GIT_TAG        v52101
)

FetchContent_GetProperties(allegro5)
if(NOT allegro5_POPULATED)
  FetchContent_MakeAvailable(allegro5)
endif()

set(ALLEGRO_LIBRARIES allegro allegro_image allegro_font allegro_color allegro_primitives allegro_ttf allegro_main)

add_subdirectory(3rdparty/Lua)

set(3RD_PARTY_DIR ${CMAKE_CURRENT_SOURCE_DIR}/3rdparty)
set(SOL_INCLUDE_DIR ${3RD_PARTY_DIR}/sol2)
set(ENTT_INCLUDE_DIR ${3RD_PARTY_DIR}/entt)
set(LUA_INCLUDE_DIR ${3RD_PARTY_DIR}/Lua/include)
set(SPDLOG_INCLUDE_DIR ${3RD_PARTY_DIR}/spdlog/include)

include_directories(
    ${allegro5_SOURCE_DIR}/include
    ${3RD_PARTY_DIR}
    ${SPDLOG_INCLUDE_DIR}
    ${ENTT_INCLUDE_DIR}
    ${LUA_INCLUDE_DIR}
    ${SOL_INCLUDE_DIR}
    ./code
)

add_subdirectory(code)
