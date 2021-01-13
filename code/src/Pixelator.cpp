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

Pixelator::Pixelator()
	: m_size(360.f, 240.f)
	, m_pixels()
	, m_buffers()
{
    setSize(m_size);
}

void Pixelator::setSize(const sf::Vector2f size)
{
	m_size = size;
    std::vector<sf::Uint8> newPixels(m_size.x * m_size.y * 4u);
    sf::Uint8* ptr = &newPixels[0];
    sf::Uint8* end = ptr + newPixels.size();
    while (ptr < end)
    {
        *ptr++ = sf::Color::Black.r;
        *ptr++ = sf::Color::Black.g;
        *ptr++ = sf::Color::Black.b;
        *ptr++ = sf::Color::Black.a;
    }
    // Commit the new pixel buffer
    m_pixels.swap(newPixels);
}

void Pixelator::setPixel(unsigned int x, unsigned int y, const sf::Color& color)
{
    sf::Uint8* pixel = &m_pixels[(x + y * m_size.x) * 4];
    *pixel++ = color.r;
    *pixel++ = color.g;
    *pixel++ = color.b;
    *pixel++ = color.a;
}

sf::Color Pixelator::getPixel(unsigned int x, unsigned int y) const
{
    const sf::Uint8* pixel = &m_pixels[(x + y * m_size.x) * 4];
    return sf::Color(pixel[0], pixel[1], pixel[2], pixel[3]);
}

const sf::Uint8* Pixelator::getPixelsPtr() const
{
    if (!m_pixels.empty())
    {
        return &m_pixels[0];
    }
    else
    {
        //err() << "Trying to access the pixels of an empty image" << std::endl;
        return NULL;
    }
}

void Pixelator::fill(sf::Color color)
{
    std::vector<sf::Uint8> newPixels(m_size.x * m_size.y * 4);
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
    m_pixels.swap(newPixels);
}

void Pixelator::clear()
{
    std::vector<sf::Uint8> newPixels(m_size.x * m_size.y * 4);
    sf::Uint8* ptr = &newPixels[0];
    sf::Uint8* end = ptr + newPixels.size();
    while (ptr < end)
    {
        *ptr++ = sf::Color::Transparent.r;
        *ptr++ = sf::Color::Transparent.g;
        *ptr++ = sf::Color::Transparent.b;
        *ptr++ = sf::Color::Transparent.a;
    }
    // Commit the new pixel buffer
    m_pixels.swap(newPixels);
}

