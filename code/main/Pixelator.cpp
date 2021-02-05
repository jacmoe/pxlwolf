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

#include <cassert>

Pixelator::Pixelator()
    : m_current_buffer("primary")
    , m_buffers()
{
    m_buffers.emplace_back();
    setSize(360, 240);
    m_buffer_map.insert({"primary", 0});
}

Pixelator::~Pixelator()
{
}

bool check_key(std::unordered_map<std::string, unsigned int> m, std::string key) 
{ 
    if (m.find(key) == m.end()) 
        return false; 
  
    return true; 
}

bool Pixelator::addBuffer(const std::string name, const int width, const int height)
{
    if(check_key(m_buffer_map, name))
    {
       SPDLOG_ERROR("Attempting to add '{}' which already exist!", name.c_str());
        return false;
    }

    const unsigned int newBufferIndex{ static_cast<unsigned int>(m_buffers.size()) };
    m_buffer_map.insert({name, newBufferIndex});
    m_buffers.emplace_back();

    unsigned int index = m_buffer_map[m_current_buffer];

    std::vector<uint32_t> newPixels(m_buffers[index].width * m_buffers[index].height);
    uint32_t* ptr = &newPixels[0];
    uint32_t* end = ptr + newPixels.size();
    while (ptr < end)
    {
        *ptr++ = 0;
        *ptr++ = 0;
        *ptr++ = 0;
        *ptr++ = 0;
    }
    // Commit the new pixel buffer
    m_buffers[newBufferIndex].pixels.swap(newPixels);
    m_buffers[newBufferIndex].width = m_buffers[index].width;
    m_buffers[newBufferIndex].height = m_buffers[index].height;

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
    unsigned int index = m_buffer_map[name];
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

void Pixelator::setSize(const int width, const int height)
{
    setSize(m_current_buffer, width, height);
}

void Pixelator::setSize(const std::string& name, const int width, const int height)
{
    if(!check_key(m_buffer_map, name))
    {
       SPDLOG_ERROR("Attempting to set the size of a buffer that doesn't exist!");
        return;
    }
    unsigned int index = m_buffer_map[name];
    m_buffers[index].width = width;
    m_buffers[index].height = height;
    std::vector<uint32_t> newPixels(m_buffers[index].width * m_buffers[index].height);
    uint32_t* ptr = &newPixels[0];
    uint32_t* end = ptr + newPixels.size();
    while (ptr < end)
    {
        *ptr++ = 0;
        *ptr++ = 0;
        *ptr++ = 0;
        *ptr++ = 0;
    }
    // Commit the new pixel buffer
    m_buffers[index].pixels.swap(newPixels);
    m_buffers[index].width = m_buffers[index].width;
    m_buffers[index].height = m_buffers[index].height;
}
/*
void Pixelator::setPixel(unsigned int x, unsigned int y, const SDL_Color& color)
{
    setPixel(m_current_buffer, x, y, color);
}

void Pixelator::setPixel(const std::string& name, unsigned int x, unsigned int y, const SDL_Color& color)
{
    unsigned int index = m_buffer_map[name];
    ImageDrawPixel(&m_buffers[index], x, y, color);
}

void Pixelator::drawColumn(const std::string& name, unsigned int x, unsigned int y, unsigned int height, const SDL_Color& color)
{
   if (y < 0)
    {
        height = height + y;
        y = 0;
    }
    if (y + height > m_buffers[m_buffer_map[m_current_buffer]].height)
    {
        height = m_buffers[m_buffer_map[m_current_buffer]].height - y;
    }
    for (int32_t i = y; i < y + height; i++)
    {
        setPixel(name, x, i, color);
    }
}

void Pixelator::drawRow(unsigned int x, unsigned int y, unsigned int length, const SDL_Color& color)
{
    for (int32_t i = x; i < length; i++)
    {
        setPixel(i, y, color);
    }
}

void Pixelator::drawFilledRect(const std::string& name, const SDL_Rect& rect, const SDL_Color& color)
{
    ImageDrawSDL_RectRec(&m_buffers[m_buffer_map[name]], rect, color);
}

// Doom's version of Bresenham
void Pixelator::drawLine(const linalg::aliases::double2& start, const linalg::aliases::double2& end, const SDL_Color& color)
{
    int dx = end.x - start.x;
    int ax = 2 * abs(dx);
    int sx = dx < 0 ? -1 : 1;

    int dy = end.y - start.y;
    int ay = 2 * abs(dy);
    int sy = dy < 0 ? -1 : 1;

    int x = start.x;
    int y = start.y;

    if (ax > ay) {
        int d = ay - ax / 2;
        while (1) {
            setPixel(x, y, color);
            if (x == end.x) return;

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
            setPixel(x, y, color);
            if (y == end.y) return;

            if (d >= 0) {
                x += sx;
                d -= ay;
            }
            y += sy;
            d += ax;
        }
    }
}
/*
// https://en.wikipedia.org/wiki/Midpoint_circle_algorithm
void Pixelator::drawCircle(const sf::linalg::aliases::double2i& coord, const int radius, const SDL_Color& color)
{
    int x = radius;
    int y = 0;
    int decisionOver2 = 1 - x;  // Decision criterion divided by 2 evaluated at x=r, y=0

    while (x >= y) {
        setPixel(x + coord.x,  y + coord.y, color);
        setPixel(y + coord.x,  x + coord.y, color);
        setPixel(-x + coord.x,  y + coord.y, color);
        setPixel(-y + coord.x,  x + coord.y, color);
        setPixel(-x + coord.x, -y + coord.y, color);
        setPixel(-y + coord.x, -x + coord.y, color);
        setPixel(x + coord.x, -y + coord.y, color);
        setPixel(y + coord.x, -x + coord.y, color);
        y++;

        if (decisionOver2 <= 0) {
            decisionOver2 += 2 * y + 1;  // Change in decision criterion for y -> y+1
        } else {
            x--;
            decisionOver2 += 2 * (y - x) + 1;  // Change for y -> y+1, x -> x-1
        }
    }
}
*/
/*
// x y width height
void Pixelator::drawRect(const SDL_Rect rect, const SDL_Color& color)
{
    float right = rect.x + rect.width;
    float bottom = rect.y + rect.height;
    drawLine({rect.x, rect.y}, {right, rect.y}, color);
    drawLine({rect.x, bottom}, {right, bottom}, color);
    drawLine({rect.x, rect.y}, {rect.x, bottom}, color);
    drawLine({right, rect.y}, {right, bottom}, color);
}

SDL_Color Pixelator::getPixel(const std::string& name, unsigned int x, unsigned int y)
{
    unsigned int index = m_buffer_map.at(name);
    return GetPixelColor(&m_buffers[index], UNCOMPRESSED_R8G8B8A8);
}

SDL_Color* Pixelator::getPixels(const std::string& name)
{
    if(!check_key(m_buffer_map, name))
    {
       SPDLOG_ERROR("Attempting to get the pixel data of a buffer that doesn't exist!");
        return nullptr;
    }
    unsigned int index = m_buffer_map[name];

    return LoadImageColors(m_buffers[index]);
}

void* Pixelator::getData(const std::string& name)
{
    if (!check_key(m_buffer_map, name))
    {
       SPDLOG_ERROR( "Attempting to get the pixel data of a buffer that doesn't exist!");
        return nullptr;
    }
    unsigned int index = m_buffer_map[name];

    return m_buffers[index].data;
}

void Pixelator::fill(const std::string& name, SDL_Color color)
{
    if(!check_key(m_buffer_map, name))
    {
       SPDLOG_ERROR("Attempting to fill a buffer that doesn't exist!");
        return;
    }
    unsigned int index = m_buffer_map.at(name);
    ImageClearBackground(&m_buffers[index], color);
}

void Pixelator::randomize()
{
    unsigned int index = m_buffer_map.at(m_current_buffer);
    SDL_Color *pixels = LoadImageColors(m_buffers[index]);
    
    for (int y = 0; y < m_buffers[index].height; y++)
    {
        for (int x = 0; x < m_buffers[index].width; x++)
        {
            pixels[y*m_buffers[index].width + x].r = static_cast<unsigned char>(GetRandomValue(0, 255));
            pixels[y*m_buffers[index].width + x].g = static_cast<unsigned char>(GetRandomValue(0, 255));
            pixels[y*m_buffers[index].width + x].b = static_cast<unsigned char>(GetRandomValue(0, 255));
            pixels[y*m_buffers[index].width + x].a = static_cast<unsigned char>(GetRandomValue(0, 255));
        }
    }
    RL_FREE(m_buffers[index].data);
    m_buffers[index].data = pixels;
}

void Pixelator::clear(const std::string& name)
{
    unsigned int index = m_buffer_map.at(name);
    ImageClearBackground(&m_buffers[index], BLANK);
}

SDL_Rect Pixelator::getSize(const std::string name)
{
    SDL_Rect rect = { 0, 0, static_cast<int>(m_buffers[m_buffer_map[name]].width), static_cast<int>(m_buffers[m_buffer_map[name]].height) };
    return rect;
}

// copies from an image
void Pixelator::copy(const std::string& name, Image source)
{
    UnloadImage(m_buffers[m_buffer_map[name]]);
    m_buffers[m_buffer_map[name]] = ImageCopy(source);
}

// copies from a buffer
void Pixelator::copy(const std::string name, unsigned int x, unsigned int y, const SDL_Rect& sourceRect)
{
    if(!check_key(m_buffer_map, name))
    {
       SPDLOG_ERROR("Attempting to copy from '{}' which doesn't exist!", name);
        return;
    }
    ImageDraw(&m_buffers[m_buffer_map[m_current_buffer]], 
                m_buffers[m_buffer_map[name]],
                {static_cast<float>(x), static_cast<float>(y),
                    static_cast<float>(m_buffers[m_buffer_map[name]].width),
                    static_cast<float>(m_buffers[m_buffer_map[name]].height)},
                sourceRect, WHITE);
}

// copies everything from another buffer
void Pixelator::copy(const std::string name, unsigned int x, unsigned int y)
{
    if(!check_key(m_buffer_map, name))
    {
       SPDLOG_ERROR("Attempting to copy from '{}' which doesn't exist!", name);
        return;
    }
    ImageDraw(&m_buffers[m_buffer_map[m_current_buffer]], 
                m_buffers[m_buffer_map[name]],
                { 0, 0, static_cast<float>(m_buffers[m_buffer_map[name]].width),
                    static_cast<float>(m_buffers[m_buffer_map[name]].height)},
                {static_cast<float>(x), static_cast<float>(y),
                    static_cast<float>(m_buffers[m_buffer_map[name]].width),
                    static_cast<float>(m_buffers[m_buffer_map[name]].height)}, WHITE);
}
*/
