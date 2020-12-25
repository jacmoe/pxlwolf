/*
*   This file is part of the
* ███╗   ██╗ █████╗ ███████╗██╗     
* ████╗  ██║██╔══██╗██╔════╝██║     
* ██╔██╗ ██║███████║███████╗██║     
* ██║╚██╗██║██╔══██║╚════██║██║     
* ██║ ╚████║██║  ██║███████║███████╗
* ╚═╝  ╚═══╝╚═╝  ╚═╝╚══════╝╚══════╝
*   project : https://github.com/jacmoe/nasl
*
*   Copyright 2017 Jacob Moen
*
*/
#ifndef NASLDRAW_H
#define NASLDRAW_H

#include <stdint.h>
#include "nasl_buffer.h"

enum FontSize
{
    FONT_SMALL = 1,
    FONT_NORMAL = 2
};

void nasl_draw_line(Buffer* b, int x0, int y0, int x1, int y1, uint32_t color);
void nasl_draw_vertical_line(Buffer* buffer, int x, int y1, int y2, uint32_t color);
void nasl_draw_rect(Buffer* b, int left, int top, int right, int bottom, uint32_t color);
void nasl_draw_text(Buffer* buffer, int x, int y, uint32_t color, int font_size, const char* fmt, ...);
void nasl_draw_script_init();

#endif /* NASLDRAW_H */
