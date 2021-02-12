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
set(TARGET_NAME pxlwolf)

set(main
    ./main/Application.cpp
    ./main/Application.hpp
    ./main/Entity.cpp
    ./main/Entity.hpp
    ./main/Game.cpp
    ./main/Game.hpp
    ./main/Level.cpp
    ./main/Level.hpp
    ./main/Pixelator.cpp
    ./main/Pixelator.hpp
    ./main/RayCaster.cpp
    ./main/RayCaster.hpp
)

set(utility
    ./utility/dbg_console.cpp
    ./utility/dbg_console.hpp
    ./utility/ImageAtlas.cpp
    ./utility/ImageAtlas.hpp
    ./utility/Map.cpp
    ./utility/Map.hpp
    ./utility/SDLDeleter.hpp
    # ./utility/SpriteSheetLoader.cpp
    # ./utility/SpriteSheetLoader.hpp
    ./utility/Timer.cpp
    ./utility/Timer.hpp
    ./utility/utils.cpp
    ./utility/utils.hpp
    ${3RD_PARTY_DIR}/physfs.cpp
    ${3RD_PARTY_DIR}/physfs.hpp
    # ${3RD_PARTY_DIR}/lua_main.hpp
)

if(WIN32)
add_executable(${TARGET_NAME}
    WIN32
    ./main.cpp
    ${main}
    ${utility}
)
else()
add_executable(${TARGET_NAME}
    ./main.cpp
    ${main}
    ${utility}
)
endif()

if(WIN32)
target_link_libraries(pxlwolf lua  physfs-static SDL2::SDL2 SDL2::SDL2main SDL_ttf::SDL_ttf)
else()
target_link_libraries(pxlwolf lua  physfs-static SDL2::SDL2 SDL_ttf::SDL_ttf)
endif()
