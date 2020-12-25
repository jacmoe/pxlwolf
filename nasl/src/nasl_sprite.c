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
#include <stdlib.h>

#include "nasl_buffer.h"
#include "nasl_image.h"
#include "nasl_sprite.h"


SpriteSheet nasl_sprite_load(const char *path, int rows, int cols) {
    Buffer *b = nasl_image_load(path);

    SpriteSheet ss = {
        .rows = rows,
        .cols = cols,
        .width =  b->width / cols,
        .height = b->height / rows,
        .sprites = malloc(sizeof(Buffer *) * rows * cols)
    };

    for (int j = 0; j < rows; j++) {
        for (int i = 0; i < cols; i++) {
            Buffer *sprite = nasl_buffer_get_subbuffer(
                    b, i * ss.width, j * ss.height, ss.width, ss.height
                    );

            ss.sprites[j * cols + i] = sprite;
        }
    }

    nasl_buffer_destroy(b);

    return ss;
}


Buffer* nasl_sprite_get(SpriteSheet ss, int row, int column)
{
    return ss.sprites[row * ss.cols + column];
}


void nasl_sprite_delete(SpriteSheet ss)
{
    for (int i = 0; i < ss.rows * ss.cols; i++) {
        free(ss.sprites[i]);
    }
}
