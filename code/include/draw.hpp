/*# This file is part of the
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
#*/
#pragma once
#include "geometry.hpp"
#include "sprites.hpp"
#include "color.hpp"

void D_DrawLine(Buffer *b, int x0, int y0, int x1, int y1, uint32_t color);
void D_DrawCircle(Buffer *b, int x0, int y0, int radius, uint32_t color);
void D_DrawSegment(Buffer *b, Segment l, uint32_t color);
void D_DrawBox(Buffer *buf, Box b, uint32_t color);
void D_DrawText(Buffer *b, SpriteSheet ascii, int x, int y, const char *fmt, ...);
