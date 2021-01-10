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
#   (c) 2020 - 2021 Jacob Moena
#
#   MIT License
#*/
#include "Pixelator.hpp"

sf::Color commodoreColorPallette[16] = {
	{0,0,0,255},		// Black
	{255,255,255,255},	// White
	{136,0,0,255},		// Red
	{170,255,238,255},	// Cyan
	{204,68,204,255},	// Purple
	{0,204,85,255},		// Green
	{0,0,170,255},		// Blue
	{238,238,119,255},	// Yellow
	{221,136,85,255},	// Orange
	{102,68,0,255},		// Brown
	{255,119,119,255},	// Light red
	{51,51,51,255},		// Dark grey
	{119,119,119,255},	// Grey
	{170,255,102,255},	// Light green
	{0,136,255,255},	// Light blue
	{187,187,187,255}	// Light grey
};


PixelBuffer* Pixelator::CreatePixelBuffer(uint32_t width, uint32_t height, sf::Color color)
{
    PixelBuffer* newBuffer = new PixelBuffer;
    std::vector<sf::Uint8> newPixels(width * height * 4);
    // Fill it with the specified color
    sf::Uint8* ptr = &newPixels[0];
    sf::Uint8* end = ptr + newPixels.size();
    while (ptr < end)
    {
        *ptr++ = color.r;
        *ptr++ = color.g;
        *ptr++ = color.b;
        *ptr++ = color.a;
    }

    // Commit the new pixel buffer
    newBuffer->pixels.swap(newPixels);
    newBuffer->width = width;
    newBuffer->height = height;
    return newBuffer;
}

void Pixelator::PutPixel(PixelBuffer* buffer, uint32_t x, uint32_t y, sf::Color color)
{
    sf::Uint8* pixel = &buffer->pixels[(x + y * buffer->width) * 4];
    *pixel++ = color.r;
    *pixel++ = color.g;
    *pixel++ = color.b;
    *pixel++ = color.a;
}

void Pixelator::PutPixelAlpha(PixelBuffer* buffer, uint32_t x, uint32_t y, sf::Color color, double alphaNum)
{
    int r = color.r;
    int g = color.g;
    int b = color.b;
    int a = color.a;
    if (a)
    {
        if (alphaNum*a != 0 && alphaNum*a != 255) // Alpha transparency, compute alpha based on array colors
        {
            double alpha = ((double)a)/255.0 * (alphaNum);
            uint32_t oldPix = buffer->pixels[y*buffer->width+x];
            int oldR = (int)(oldPix >> 3*8);
            int oldG = (int)((oldPix >> 2*8) & 0xFF);
            int oldB = (int)((oldPix >> 8) & 0xFF);
            int oldA = (int)(oldPix & 0xFF);
            r = (int)((double)r * alpha + (double)oldR * (1-alpha));
            g = (int)((double)g * alpha + (double)oldG * (1-alpha));
            b = (int)((double)b * alpha + (double)oldB * (1-alpha));
            a = (int)((double)a * alpha + (double)oldA * (1-alpha));
        }
        PutPixel(buffer, x, y, sf::Color(r,g,b,a));
    }
}

void Pixelator::FillBuffer(PixelBuffer* target, sf::Color color, double alpha)
{
    for (uint32_t i = 0; i < target->height; i++)
    {
        if (i < target->height)
        {
            for (uint32_t j = 0; j < target->width; j++)
            {
                if (j < target->width)
                {
                    PutPixelAlpha(target, j, i, color, alpha);
                }
            }
        }
    }
}
