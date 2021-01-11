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
#pragma once

#include <SFML/Graphics.hpp>

// C64 color pallette, why not
extern sf::Color commodoreColorPallette[16];

struct PixelBuffer
{
    std::vector<sf::Uint8> pixels;
    uint32_t width;
    uint32_t height;
};

class Pixelator
{
public:
	static PixelBuffer* CreatePixelBuffer(const uint32_t width, const uint32_t height, const sf::Color color);

	static void PutPixel(PixelBuffer* buffer, uint32_t x, uint32_t y, sf::Color color);
	static void PutPixelAlpha(PixelBuffer* buffer, uint32_t x, uint32_t y, sf::Color color, double alphaNum);
	static void FillBuffer(PixelBuffer* target, sf::Color color, double alpha = 1.0);
};
