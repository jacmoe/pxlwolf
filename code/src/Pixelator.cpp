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
    : m_current_buffer("primary")
	, m_buffers()
{
	m_buffers.emplace_back();
    setSize(sf::Vector2f(360.0f, 240.0f));
    m_buffer_map.insert({"primary", 0});
}

bool check_key(std::unordered_map<std::string, unsigned int> m, std::string key) 
{ 
    if (m.find(key) == m.end()) 
        return false; 
  
    return true; 
}

bool Pixelator::addBuffer(const std::string name)
{
    if(check_key(m_buffer_map, name))
    {
        SPDLOG_ERROR("Attempting to add '{}' which already exist!", name);
        return false;
    }

	const unsigned int newBufferIndex{ static_cast<unsigned int>(m_buffers.size()) };
    m_buffer_map.insert({name, newBufferIndex});
	m_buffers.emplace_back();

    unsigned int index = m_buffer_map[m_current_buffer];

    std::vector<sf::Uint8> newPixels(m_buffers[index].size.x * m_buffers[index].size.y * 4u);
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
    m_buffers[newBufferIndex].size = m_buffers[index].size;

    return true;
}

bool Pixelator::removeBuffer(const std::string name)
{
    if(!check_key(m_buffer_map, name))
    {
        SPDLOG_ERROR("Attempting to remove a buffer that doesn't exist!");
        return false;
    }
	assert(m_buffer_map[name] < m_buffers.size());
    if(m_current_buffer == name)
    {
        // Can't remove current buffer! Raise error here.
        SPDLOG_ERROR("Attempting to remove active buffer!");
        return false;
    }
    m_buffers.erase(m_buffers.begin() + m_buffer_map[name]);
    m_buffer_map.erase(name);
    return true;
}

void Pixelator::setActiveBuffer(const std::string name)
{
    if(!check_key(m_buffer_map, name))
    {
        SPDLOG_ERROR("Attempting to use a buffer name that doesn't exist!");
        return;
    }
	assert(m_buffer_map[name] < m_buffers.size());
    m_current_buffer = name;
}

bool Pixelator::swapBuffer(const std::string name)
{
    if(!check_key(m_buffer_map, name))
    {
        SPDLOG_ERROR("Attempting to swap with a buffer name that doesn't exist!");
        return false;
    }

    unsigned int own_index = m_buffer_map[m_current_buffer];
    unsigned int swap_index = m_buffer_map[name];

    m_buffers[swap_index].pixels.swap(m_buffers[own_index].pixels);

    return true;
}

void Pixelator::setSize(const sf::Vector2f size)
{
    unsigned int index = m_buffer_map[m_current_buffer];
	m_buffers[index].size = size;
    std::vector<sf::Uint8> newPixels(m_buffers[index].size.x * m_buffers[index].size.y * 4u);
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
    m_buffers[index].pixels.swap(newPixels);
    m_buffers[index].size = size;
}

void Pixelator::setPixel(unsigned int x, unsigned int y, const sf::Color& color)
{
    unsigned int index = m_buffer_map[m_current_buffer];
    sf::Uint8* pixel = &m_buffers[index].pixels[(x + y * m_buffers[index].size.x) * 4];
    *pixel++ = color.r;
    *pixel++ = color.g;
    *pixel++ = color.b;
    *pixel++ = color.a;
}

sf::Color Pixelator::getPixel(unsigned int x, unsigned int y) const
{
    unsigned int index = m_buffer_map.at(m_current_buffer);
    const sf::Uint8* pixel = &m_buffers[index].pixels[(x + y * m_buffers[index].size.x) * 4];
    return sf::Color(pixel[0], pixel[1], pixel[2], pixel[3]);
}

const sf::Uint8* Pixelator::getPixelsPtr() const
{
    unsigned int index = m_buffer_map.at(m_current_buffer);
    if (!m_buffers[index].pixels.empty())
    {
        return &m_buffers[index].pixels[0];
    }
    else
    {
        //err() << "Trying to access the pixels of an empty image" << std::endl;
        return NULL;
    }
}

void Pixelator::fill(sf::Color color)
{
    unsigned int index = m_buffer_map.at(m_current_buffer);
    std::vector<sf::Uint8> newPixels(m_buffers[index].size.x * m_buffers[index].size.y * 4);
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
    m_buffers[index].pixels.swap(newPixels);
}

void Pixelator::clear()
{
    unsigned int index = m_buffer_map.at(m_current_buffer);
    std::vector<sf::Uint8> newPixels(m_buffers[index].size.x * m_buffers[index].size.y * 4);
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
    m_buffers[index].pixels.swap(newPixels);
}
