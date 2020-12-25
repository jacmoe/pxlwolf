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
#include <assert.h>
#include <string.h>

#include "nasl_buffer.h"
#include "nasl_color.h"

static Buffer* main_buffer = 0;

Buffer *nasl_buffer_create(int width, int height)
{
    Buffer *b = malloc(sizeof(Buffer));

    b->width = width;
    b->height = height;

    b->pixels = malloc(sizeof(uint32_t) * width * height);
    memset(b->pixels, 0, sizeof(uint32_t) * width * height);

    return b;
}

void nasl_buffer_set_mainbuffer(Buffer *buf)
{
    main_buffer = buf;
}

Buffer* nasl_buffer_get_mainbuffer()
{
    return main_buffer;
}

void nasl_buffer_clear(Buffer *b, uint32_t color)
{
    for (int i = 0; i < b->width * b->height; i++)
    {
        b->pixels[i] = color;
    }
}

uint32_t nasl_buffer_get_pixel(Buffer *b, int x, int y)
{
    return b->pixels[y * b->width + x];
}

Buffer *nasl_buffer_get_subbuffer(Buffer *buf, int x, int y, int width, int height)
{
    assert(x + width <= buf->width);
    assert(y + height <= buf->height);

    Buffer *b = nasl_buffer_create(width, height);

    for (int j = 0; j < height; j++)
    {
        for (int i = 0; i < width; i++)
        {
            nasl_buffer_set_pixel(b, i, j, nasl_buffer_get_pixel(buf, x + i, y + j));
        }
    }

    return b;
}

void nasl_buffer_blit(Buffer *dest, Buffer *src, int x, int y)
{
    assert(x + src->width <= dest->width);
    assert(y + src->height <= dest->height);

    for (int j = 0; j < src->height; j++)
    {
        for (int i = 0; i < src->width; i++)
        {
            uint32_t src_pixel = nasl_buffer_get_pixel(src, i, j);
            if (src_pixel != 65535) // same as TRANSPARENT
            {
                nasl_buffer_set_pixel(dest, i + x, j + y, src_pixel);
            }
        }
    }
}

void nasl_buffer_destroy(Buffer *buf)
{
    free(buf->pixels);
    free(buf);
}
