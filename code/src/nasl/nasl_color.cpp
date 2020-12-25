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
#include "nasl_color.h"

uint32_t c64_palette[16] = {
    0x000000, 0xFFFFFF, 0x000088, 0xEEFFAA, 0xCC44CC, 0x55CC00, 0xAA0000, 0x77EEEE,
    0x5588DD, 0x004466, 0x7777FF, 0x333333, 0x777777, 0x66FFAA, 0xFF8800, 0xBBBBBB
};

uint32_t nasl_color_scale(uint32_t c, double intensity)
{
    uint8_t red = GETR(c);
    uint8_t green = GETG(c);
    uint8_t blue = GETB(c);

    red *= intensity;
    green *= intensity;
    blue *= intensity;

    return BUILDRGB(red, green, blue);
}
