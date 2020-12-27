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

set(TARGET_NAME plxwolf)

set(Sources
    ./src/Main.cpp
    ./src/Camera.cpp
    ./src/Game.cpp
    ./src/RayCaster.cpp
    ./src/SDLRenderer.cpp
    ./src/utils.cpp
)

set(Includes
    ../${IncludeDir}/utils.h
    ../${IncludeDir}/Camera.h
    ../${IncludeDir}/Game.h
    ../${IncludeDir}/IRenderer.h
    ../${IncludeDir}/Mathlib.h
    ../${IncludeDir}/RayCaster.h
    ../${IncludeDir}/SDLDeleter.h
    ../${IncludeDir}/SDLRenderer.h
    ../${IncludeDir}/WallTypes.h
)

if (WIN32)
    add_executable(${TARGET_NAME}
        #WIN32
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

target_link_libraries(plxwolf PRIVATE optimized ${SDL2_LIBS_RELEASE} debug ${SDL2_LIBS_DEBUG})
