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
#include "spdlog/spdlog.h"

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
    : m_current_buffer(0)
	, m_buffers()
{
    m_current_buffer = { static_cast<unsigned int>(m_buffers.size()) };
	m_buffers.emplace_back();
    setSize(sf::Vector2f(360.0f, 240.0f));
}

unsigned int Pixelator::addBuffer()
{
	const unsigned int newBufferIndex{ static_cast<unsigned int>(m_buffers.size()) };
	m_buffers.emplace_back();

    std::vector<sf::Uint8> newPixels(m_buffers[m_current_buffer].size.x * m_buffers[m_current_buffer].size.y * 4u);
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
    m_buffers[newBufferIndex].pixels.swap(newPixels);
    m_buffers[newBufferIndex].size = m_buffers[m_current_buffer].size;

    return newBufferIndex;
}

bool Pixelator::removeBuffer(const unsigned int index)
{
	assert(index < m_buffers.size());
    if(m_current_buffer == index)
    {
        // Can't remove current buffer! Raise error here.
        SPDLOG_ERROR("Attempting to remove active buffer!");
        return false;
    }
    m_buffers.erase(m_buffers.begin() + index);
    return true;
}

void Pixelator::setActiveBuffer(const unsigned int index)
{
	assert(index < m_buffers.size());
    m_current_buffer = index;
}

void Pixelator::setSize(const sf::Vector2f size)
{
	m_buffers[m_current_buffer].size = size;
    std::vector<sf::Uint8> newPixels(m_buffers[m_current_buffer].size.x * m_buffers[m_current_buffer].size.y * 4u);
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
    m_buffers[m_current_buffer].pixels.swap(newPixels);
    m_buffers[m_current_buffer].size = size;
}

void Pixelator::setPixel(unsigned int x, unsigned int y, const sf::Color& color)
{
    sf::Uint8* pixel = &m_buffers[m_current_buffer].pixels[(x + y * m_buffers[m_current_buffer].size.x) * 4];
    *pixel++ = color.r;
    *pixel++ = color.g;
    *pixel++ = color.b;
    *pixel++ = color.a;
}

sf::Color Pixelator::getPixel(unsigned int x, unsigned int y) const
{
    const sf::Uint8* pixel = &m_buffers[m_current_buffer].pixels[(x + y * m_buffers[m_current_buffer].size.x) * 4];
    return sf::Color(pixel[0], pixel[1], pixel[2], pixel[3]);
}

const sf::Uint8* Pixelator::getPixelsPtr() const
{
    if (!m_buffers[m_current_buffer].pixels.empty())
    {
        return &m_buffers[m_current_buffer].pixels[0];
    }
    else
    {
        //err() << "Trying to access the pixels of an empty image" << std::endl;
        return NULL;
    }
}

void Pixelator::fill(sf::Color color)
{
    std::vector<sf::Uint8> newPixels(m_buffers[m_current_buffer].size.x * m_buffers[m_current_buffer].size.y * 4);
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
    m_buffers[m_current_buffer].pixels.swap(newPixels);
}

void Pixelator::clear()
{
    std::vector<sf::Uint8> newPixels(m_buffers[m_current_buffer].size.x * m_buffers[m_current_buffer].size.y * 4);
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
    m_buffers[m_current_buffer].pixels.swap(newPixels);
}
