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
#include "nasl_draw.h"
#define SYSFONT_IMPLEMENTATION
#include "sysfont.h"
#include <stdlib.h> // for abs
#include <stdarg.h> // for va_start, etc.
#include <string.h> // for strlen
#include <stdio.h> // for vsnprintf


// Doom's version of Bresenham
void nasl_draw_line(Buffer *b, int x0, int y0, int x1, int y1, uint32_t color)
{
    int dx = x1 - x0;
    int ax = 2 * abs(dx);
    int sx = dx < 0 ? -1 : 1;

    int dy = y1 - y0;
    int ay = 2 * abs(dy);
    int sy = dy < 0 ? -1 : 1;

    int x = x0;
    int y = y0;

    if (ax > ay) {
        int d = ay - ax / 2;
        while (1) {
            nasl_buffer_set_pixel(b, x, y, color);
            if (x == x1) return;

            if (d >= 0) {
                y += sy;
                d -= ax;
            }
            x += sx;
            d += ay;
        }
    } else {
        int d = ax - ay / 2;
        while (1) {
            nasl_buffer_set_pixel(b, x, y, color);
            if (y == y1) return;

            if (d >= 0) {
                x += sx;
                d -= ay;
            }
            y += sy;
            d += ax;
        }
    }
}

//Fast vertical line from (x,y1) to (x,y2), with rgb color
void nasl_draw_vertical_line(Buffer* buffer, int x, int y1, int y2, uint32_t color)
{
	if(y2 < y1)
	{ // swap y1 and y2
		y1 += y2;
		y2 = y1 - y2;
		y1 -= y2;
	}
	if(y2 < 0 || y1 >= buffer->height || x < 0 || x >= buffer->width)
	{ // no single point of the line is on the screen
		return;
	}
	if(y1 < 0) y1 = 0; // clip
	if(y2 >= buffer->width) y2 = buffer->height - 1; // clip
	for(int y = y1; y <= y2; y++)
	{
		nasl_buffer_set_pixel(buffer, x, y, color);
	}
}

void nasl_draw_rect(Buffer* b, int left, int top, int right, int bottom, uint32_t color)
{
	uint32_t top_offset, bottom_offset, i, temp;

	if(top > bottom)
	{
		temp = top;
		top = bottom;
		bottom = temp;
	}
	if(left > right)
	{
		temp = left;
		left = right;
		right = temp;
	}

	top_offset = (top << 8) + (top << 6);
	bottom_offset = (bottom << 8) + (bottom << 6);

	for(i = left; i <= right; i++)
	{
		b->pixels[top_offset + i] = color;
		b->pixels[bottom_offset + i] = color;
	}
	for(i = top_offset; i <= bottom_offset; i += b->width)
	{
		b->pixels[left + i] = color;
		b->pixels[right + i] = color;
	}
}

//void nasl_draw_text(Buffer* buffer, int x, int y, uint32_t color, int font_size, const char* fmt, ...)
void nasl_draw_text(Buffer* buffer, int x, int y, uint32_t color, int font_size, const char* text)
{
    /*va_list args;
    va_start(args, fmt);

    char text[128] = "";
    vsnprintf(text, 128, fmt, args); // Write formatted data from variable argument list to sized buffer

    va_end(args);*/

    Buffer* tex_buf = nasl_buffer_create(buffer->width, buffer->height);
    nasl_buffer_clear(tex_buf, TRANSPARANT);
    if(font_size == FONT_NORMAL)
    {
        sysfont_9x16_u32(tex_buf->pixels, buffer->width, buffer->height, 0, 0, text, color);
    } else {
        sysfont_8x8_u32(tex_buf->pixels, buffer->width, buffer->height, 0, 0, text, color);
    }

    for (int j = 0; j < tex_buf->height; j++)
    {
        for (int i = 0; i < tex_buf->width; i++)
        {
            uint32_t src_pixel = nasl_buffer_get_pixel(tex_buf, i, j);
            if (src_pixel != TRANSPARANT)
            {
                nasl_buffer_set_pixel(buffer, i + x, j + y, src_pixel);
            }
        }
    }
}
