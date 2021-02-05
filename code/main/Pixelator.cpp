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

/** PixelRenderer::toPixColor
 * @brief Returns color formatted to RGBA format
 * @param r SDL_Color red component
 * @param g SDL_Color green component
 * @param b SDL_Color blue component
 * @param a SDL_Color alpha component
 **/
uint32_t Pixelator::toIntColor(SDL_Color color)
{
    return toIntColor(color.r, color.g, color.b, color.a);
}

uint32_t Pixelator::toIntColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
{
    return ((uint32_t)r << 3*8 | (uint32_t)g << 2*8 | (uint32_t)b << 8 | (uint32_t)a);
}

SDL_Color Pixelator::toSDLColor(uint32_t pixColor)
{
    int r = (int)(pixColor >> 3*8);
    int g = (int)((pixColor >> 2*8) & 0xFF);
    int b = (int)((pixColor >> 8) & 0xFF);
    int a = (int)(pixColor & 0xFF);
    SDL_Color newColor = {r, g, b, a};
    return newColor;
}

/** to8BitColor
 * @brief Paletizes 32bit color to 8bit color
 * 
 * @param colorDat Raw truecolor value to paletize
 * @return 8 bit color value
 */
uint32_t Pixelator::to8BitColor(uint32_t colorDat)
{
    int r = (int)(colorDat >> 3*8);
    int g = (int)((colorDat >> 2*8) & 0xFF);
    int b = (int)((colorDat >> 8) & 0xFF);
    int newR = (int)ceil(round((double)r / 255.0*15) * (255.0/15));
    int newG = (int)ceil(round((double)g / 255.0*15) * (255.0/15));
    int newB = (int)ceil(round((double)b / 255.0*15) * (255.0/15));
    return (uint32_t)(newR) << 3*8 | (uint32_t)(newG) << 2*8 | (uint32_t)newB << 8 | (uint32_t)0xFF;
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

void Pixelator::mergeBuffer(const std::string& target, const std::string& source)
{
    if(!check_key(m_buffer_map, target))
    {
       SPDLOG_ERROR("Attempting to merge into a buffer that doesn't exist!");
        return;
    }
    if(!check_key(m_buffer_map, source))
    {
       SPDLOG_ERROR("Attempting to merge with a buffer that doesn't exist!");
        return;
    }
    unsigned int target_index = m_buffer_map[target];
    unsigned int source_index = m_buffer_map[source];

    uint32_t sourcePix;
    for (uint32_t i = 0; i < m_buffers[source_index].height; i++)
    {
        if (i < m_buffers[target_index].height)
        {
            for (uint32_t j = 0; j < m_buffers[source_index].width; j++)
            {
                if (j < m_buffers[target_index].width)
                {
                    sourcePix = m_buffers[source_index].pixels[j+i*m_buffers[source_index].width];
                    setPixel(target, j, i, toSDLColor(sourcePix));
                }
            }
        }
    }
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

linalg::aliases::uint2 Pixelator::getSize(const std::string name)
{
    linalg::aliases::uint2 size = { m_buffers[m_buffer_map[name]].width, m_buffers[m_buffer_map[name]].height };
    return size;
}


void Pixelator::setPixel(unsigned int x, unsigned int y, const SDL_Color& color)
{
    setPixel(m_current_buffer, x, y, color);
}

void Pixelator::setPixel(const std::string& name, unsigned int x, unsigned int y, const SDL_Color& color)
{
    unsigned int index = m_buffer_map[name];

    m_buffers[index].pixels[y*m_buffers[index].width+x] = toIntColor(color);
}

uint32_t Pixelator::getPixel(const std::string& name, unsigned int x, unsigned int y)
{
    unsigned int index = m_buffer_map.at(name);
    return m_buffers[index].pixels[y*m_buffers[index].width+x];
}

uint32_t* Pixelator::getPixels(const std::string& name)
{
    if(!check_key(m_buffer_map, name))
    {
       SPDLOG_ERROR("Attempting to get the pixel data of a buffer that doesn't exist!");
        return nullptr;
    }
    unsigned int index = m_buffer_map[name];

    return &m_buffers[index].pixels[0];
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
    unsigned int index = m_buffer_map.at(name);
    if (rect.x < m_buffers[index].width)
    {
        for (uint32_t i = rect.x; i < rect.x + rect.w; i++)
        {
            if (i < m_buffers[index].width)
            {
                drawColumn(name, i, rect.y, rect.h, color);
            }
        }
    }
}

// Doom's version of Bresenham
void Pixelator::drawLine(const linalg::aliases::int2& start, const linalg::aliases::int2& end, const SDL_Color& color)
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

// https://en.wikipedia.org/wiki/Midpoint_circle_algorithm
void Pixelator::drawCircle(const linalg::aliases::int2& coord, const int radius, const SDL_Color& color)
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

// x y width height
void Pixelator::drawRect(const SDL_Rect rect, const SDL_Color& color)
{
    int right = rect.x + rect.w;
    int bottom = rect.y + rect.h;
    drawLine({rect.x, rect.y}, {right, rect.y}, color);
    drawLine({rect.x, bottom}, {right, bottom}, color);
    drawLine({rect.x, rect.y}, {rect.x, bottom}, color);
    drawLine({right, rect.y}, {right, bottom}, color);
}

void Pixelator::fill(const std::string& name, SDL_Color color)
{
    if(!check_key(m_buffer_map, name))
    {
       SPDLOG_ERROR("Attempting to fill a buffer that doesn't exist!");
        return;
    }
    unsigned int index = m_buffer_map.at(name);

    std::vector<uint32_t> newPixels(m_buffers[index].width * m_buffers[index].height);
    uint32_t* ptr = &newPixels[0];
    uint32_t* end = ptr + newPixels.size();
    while (ptr < end)
    {
        *ptr++ = color.r;
        *ptr++ = color.g;
        *ptr++ = color.b;
        *ptr++ = color.a;
    }
    // Commit the new pixel buffer
    m_buffers[index].pixels.swap(newPixels);
    m_buffers[index].width = m_buffers[index].width;
    m_buffers[index].height = m_buffers[index].height;
}

void Pixelator::randomize(const std::string& name)
{
    if(!check_key(m_buffer_map, name))
    {
       SPDLOG_ERROR("Attempting to randomize a buffer that doesn't exist!");
        return;
    }
    unsigned int index = m_buffer_map.at(name);

    std::vector<uint32_t> newPixels(m_buffers[index].width * m_buffers[index].height);
    uint32_t* ptr = &newPixels[0];
    uint32_t* end = ptr + newPixels.size();
    while (ptr < end)
    {
        *ptr++ = rand() % 255;
        *ptr++ = rand() % 255;
        *ptr++ = rand() % 255;
        *ptr++ = rand() % 255;
    }
    // Commit the new pixel buffer
    m_buffers[index].pixels.swap(newPixels);
    m_buffers[index].width = m_buffers[index].width;
    m_buffers[index].height = m_buffers[index].height;
}

void Pixelator::clear(const std::string& name)
{
    if(!check_key(m_buffer_map, name))
    {
       SPDLOG_ERROR("Attempting to fill a buffer that doesn't exist!");
        return;
    }
    unsigned int index = m_buffer_map.at(name);

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
