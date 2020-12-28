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
#   BSD 2-Clause "Simplified" License
#
set(TARGET_NAME pxlwolf)

set(Sources
    ./src/buffer.cpp
    ./src/collision.cpp
    ./src/color.cpp
    ./src/dbg_console.cpp
    ./src/draw.cpp
    ./src/geometry.cpp
    ./src/main.cpp
    ./src/map.cpp
    ./src/sprites.cpp
    ./src/system.cpp
    ./src/utils.cpp
)

set(Includes
    ../${IncludeDir}/buffer.hpp
    ../${IncludeDir}/collision.hpp
    ../${IncludeDir}/color.hpp
    ../${IncludeDir}/dbg_console.hpp
    ../${IncludeDir}/dbg.hpp
    ../${IncludeDir}/defs.hpp
    ../${IncludeDir}/draw.hpp
    ../${IncludeDir}/geometry.hpp
    ../${IncludeDir}/map.hpp
    ../${IncludeDir}/sprites.hpp
    ../${IncludeDir}/system.hpp
    ../${IncludeDir}/utils.hpp
)

if (WIN32)
    add_executable(${TARGET_NAME}
        WIN32
        ${Sources}
        ${Includes}
    )
endif (WIN32)
if (UNIX)
    add_executable(${TARGET_NAME}
        ${Sources}
        ${Includes}
    )
endif (UNIX)

#target_link_libraries(pxlwolf ${SDL2_LIBS_RELEASE})
target_link_libraries(pxlwolf debug ${SDL2_LIBS_DEBUG})
