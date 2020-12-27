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
    ./src/main.cpp
    ./src/utils.cpp
    ./src/dbg_console.cpp
)

set(Includes
    ../${IncludeDir}/utils.hpp
    ../${IncludeDir}/dbg_console.hpp
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

target_link_libraries(plxwolf
        Threads::Threads
        ${OPENGL_LIBRARIES}
        )

target_link_libraries(plxwolf optimized ${SDL2_LIBS_RELEASE} debug ${SDL2_LIBS_DEBUG})
