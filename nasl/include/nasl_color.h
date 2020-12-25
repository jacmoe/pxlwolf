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
/*-*- mode: c;-*-*/
#ifndef NASLCOLOR_H
#define NASLCOLOR_H
#include <stdint.h>

// Little-endian
enum Color
{
    BLACK = 0x000000,
    WHITE = 0xFFFFFF,
    RED = 0x000088,
    CYAN = 0xEEFFAA,
    VIOLET = 0xCC44CC,
    GREEN = 0x55CC00,
    BLUE = 0xAA0000,
    YELLOW = 0x77EEEE,
    ORANGE = 0x5588DD,
    BROWN = 0x004466,
    LIGHTRED = 0x7777FF,
    GREY1 = 0x333333,
    GREY2 = 0x777777,
    LIGHTGREEN = 0x66FFAA,
    LIGHTBLUE = 0xFF8800,
    GREY3 = 0xBBBBBB,
    TRANSPARANT = 0xFFFF00,
};

extern uint32_t c64_palette[16];

#define GETR(c) (((c) >> 0) & 0xFF)
#define GETG(c) (((c) >> 8) & 0xFF)
#define GETB(c) (((c) >> 16) & 0xFF)

#define BUILDRGB(r, g, b) ((r) | (g) << 8 | (b) << 16)

uint32_t nasl_color_scale(uint32_t color, double intensity);
void nasl_color_script_init();

#endif /* NASLCOLOR_H */
