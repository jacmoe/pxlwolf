#
#   This file is part of the
# ███╗   ██╗ █████╗ ███████╗██╗     
# ████╗  ██║██╔══██╗██╔════╝██║     
# ██╔██╗ ██║███████║███████╗██║     
# ██║╚██╗██║██╔══██║╚════██║██║     
# ██║ ╚████║██║  ██║███████║███████╗
# ╚═╝  ╚═══╝╚═╝  ╚═╝╚══════╝╚══════╝
#   project : https://github.com/jacmoe/nasl
#
#   Copyright 2017 Jacob Moen
#
#

# Paths
# -----
project(nasl_lib)

set(TARGET_NAME nasl)

set(Headers
    include/nasl_defs.h
    include/nasl_dbg.h
    include/nasl_script.h
    include/nasl_graphics.h
    include/nasl_color.h
    include/nasl_buffer.h
    include/nasl_draw.h
    include/nasl_image.h
    include/nasl_sprite.h
    include/nasl_geometry.h
)

set(Sources
    src/nasl_script.c
    src/nasl_graphics.c
    src/nasl_graphics_script.c
    src/nasl_color.c
    src/nasl_color_script.c
    src/nasl_buffer.c
    src/nasl_buffer_script.c
    src/nasl_draw.c
    src/nasl_draw_script.c
    src/nasl_image.c
    src/nasl_image_script.c
    src/nasl_sprite.c
    src/nasl_sprite_script.c
    src/nasl_geometry.c
)

set(3rdParty_Headers
    3rd/include/my_basic.h
    3rd/include/stb_image.h
    3rd/include/sysfont.h
)

set(3rdParty_Source
    3rd/src/my_basic.c
)

include_directories(
    include
    3rd/include
)

add_library(${TARGET_NAME} STATIC
    ${Headers}
    ${Sources}
    ${3rdParty_Headers}
    ${3rdParty_Source}
)

set_property(TARGET ${TARGET_NAME} PROPERTY C_STANDARD 99) # 90 99 11
