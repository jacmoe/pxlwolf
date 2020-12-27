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
    ./src/color.cpp
    ./src/dbg_console.cpp
    ./src/geometry.cpp
    ./src/main.cpp
    ./src/system.cpp
    ./src/utils.cpp
)

set(Includes
    ../${IncludeDir}/buffer.hpp
    ../${IncludeDir}/color.hpp
    ../${IncludeDir}/dbg_console.hpp
    ../${IncludeDir}/dbg.hpp
    ../${IncludeDir}/defs.hpp
    ../${IncludeDir}/geometry.hpp
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

target_link_libraries(pxlwolf optimized ${SDL2_LIBS_RELEASE} debug ${SDL2_LIBS_DEBUG})
