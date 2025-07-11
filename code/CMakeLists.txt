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
    ./main/types.hpp
)

set(utility
    ./utility/dbg_console.cpp
    ./utility/dbg_console.hpp
    ./utility/ImageAtlas.cpp
    ./utility/ImageAtlas.hpp
    ./utility/Map.cpp
    ./utility/Map.hpp
    ./utility/ALDeleter.hpp
    # ./utility/SpriteSheetLoader.cpp
    # ./utility/SpriteSheetLoader.hpp
    ./utility/utils.cpp
    ./utility/utils.hpp
    # ${3RD_PARTY_DIR}/lua_main.hpp
)

add_executable(${TARGET_NAME}
    ./main.cpp
    ${main}
    ${utility}
)

target_link_libraries(pxlwolf lua ${ALLEGRO_LIBRARIES})
