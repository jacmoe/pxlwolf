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
// Buffers are rectangular arrays of pixels.
//------------------------------------------------------------------------------
#ifndef NASLBUFFER_H
#define NASLBUFFER_H

//#include "dbg.h"
#include "nasl_color.h"

typedef struct Buffer
{
    int width, height;
    uint32_t *pixels;
} Buffer;

// Returns a Buffer of the given width and height.
Buffer* nasl_buffer_create(int width, int height);

void nasl_buffer_set_mainbuffer(Buffer *buf);
Buffer* nasl_buffer_get_mainbuffer();

// Register Buffer scripting methods
void nasl_buffer_script_init();

// Free a Buffer
void nasl_buffer_destroy(Buffer* buf);

// Fills a Buffer with color
void nasl_buffer_clear(Buffer* b, uint32_t color);

// Returns a rectangular sub-buffer of buf starting at pixel (x,y) and with
// the given width and height.
Buffer *nasl_buffer_get_subbuffer(Buffer* buf, int x, int y, int width, int height);

// Copies src to dest starting at (x,y) pixel of dest.
void nasl_buffer_blit(Buffer* dest, Buffer* src, int x, int y);

// Sets pixel (x,y) of b to color.
static inline void nasl_buffer_set_pixel(Buffer* b, int x, int y, uint32_t color)
{
#ifndef NDEBUG
    if (!(x >= 0 && x < b->width && y >= 0 && y < b->height))
    {
        //debug("Drawing outside the buffer! (%d, %d)", x, y);
    }
#endif

    b->pixels[y * b->width + x] = color;
}

// Returns the color of pixel (x,y) of b.
uint32_t nasl_buffer_get_pixel(Buffer* b, int x, int y);

#endif /* NASLBUFFER_H */
