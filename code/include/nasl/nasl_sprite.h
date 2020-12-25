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
//------------------------------------------------------------------------------
// Sprite sheets
//------------------------------------------------------------------------------
#ifndef NASLSPRITE_H
#define NASLSPRITE_H

#include "nasl_buffer.h"

typedef struct SpriteSheet {
    Buffer **sprites;
    int rows, cols;
    int width, height;
} SpriteSheet;


SpriteSheet nasl_sprite_load(const char *path, int rows, int cols);
Buffer* nasl_sprite_get(SpriteSheet ss, int row, int column);
void nasl_sprite_script_init();
void nasl_sprite_delete(SpriteSheet ss);

#endif // NASLSPRITE_H
