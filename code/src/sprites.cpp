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
#include <cstdlib>

#include "buffer.hpp"
#include "sprites.hpp"
#include "system.hpp"


SpriteSheet SS_LoadSpriteSheet(const char *path, int rows, int cols) {
    Buffer *b = S_LoadImage(path);

    SpriteSheet ss;
    ss.rows = rows;
    ss.cols = cols;
    ss.width =  b->width / cols;
    ss.height = b->height / rows;
    ss.sprites = (Buffer**)malloc(sizeof(Buffer *) * rows * cols);

    for (int j = 0; j < rows; j++) {
        for (int i = 0; i < cols; i++) {
            Buffer *sprite = B_GetSubBuffer(
                    b, i * ss.width, j * ss.height, ss.width, ss.height
                    );

            ss.sprites[j * cols + i] = sprite;
        }
    }

    B_DeleteBuffer(b);

    return ss;
}


Buffer *SS_GetSprite(SpriteSheet ss, int x, int y) {
    return ss.sprites[y * ss.cols + x];
}


void SS_DeleteSpriteSheet(SpriteSheet ss) {
    for (int i = 0; i < ss.rows * ss.cols; i++) {
        free(ss.sprites[i]);
    }
}
