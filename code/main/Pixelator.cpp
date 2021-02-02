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

#include <cassert>

Color commodoreColorPallette[16] = {
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
    addBuffer("primary", 360, 240);
    m_buffer_map.insert({"primary", 0});
}

Pixelator::~Pixelator()
{
    for (auto& img : m_buffers)
    {
        UnloadImage(img);
    }
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
        TraceLog(LOG_ERROR,"Attempting to add '%s' which already exist!", name.c_str());
        return false;
    }

    const unsigned int newBufferIndex{ static_cast<unsigned int>(m_buffers.size()) };
    m_buffer_map.insert({name, newBufferIndex});
    m_buffers.push_back(GenImageColor(width, height, BLANK));
    return true;
}

bool Pixelator::removeBuffer(const std::string name)
{
    if(!check_key(m_buffer_map, name))
    {
        TraceLog(LOG_ERROR,"Attempting to remove a buffer that doesn't exist!");
        return false;
    }
    assert(m_buffer_map[name] < m_buffers.size());
    if(m_current_buffer == name)
    {
        // Can't remove current buffer! Raise error here.
        TraceLog(LOG_ERROR,"Attempting to remove active buffer!");
        return false;
    }
    unsigned int index = m_buffer_map[name];
    UnloadImage(m_buffers[index]);
    m_buffers.erase(m_buffers.begin() + m_buffer_map[name]);
    m_buffer_map.erase(name);
    return true;
}

void Pixelator::setActiveBuffer(const std::string name)
{
    if(!check_key(m_buffer_map, name))
    {
        TraceLog(LOG_ERROR,"Attempting to use a buffer name that doesn't exist!");
        return;
    }
    assert(m_buffer_map[name] < m_buffers.size());
    m_current_buffer = name;
}

bool Pixelator::swapBuffer(const std::string name)
{
    if(!check_key(m_buffer_map, name))
    {
        TraceLog(LOG_ERROR,"Attempting to swap with a buffer name that doesn't exist!");
        return false;
    }

    unsigned int own_index = m_buffer_map[m_current_buffer];
    unsigned int swap_index = m_buffer_map[name];

    Image temp = m_buffers[own_index];
    m_buffers[own_index] = m_buffers[swap_index];
    m_buffers[swap_index] = temp;

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
        TraceLog(LOG_ERROR,"Attempting to set the size of a buffer that doesn't exist!");
        return;
    }
    unsigned int index = m_buffer_map[name];
    UnloadImage(m_buffers[index]);
    m_buffers[index] = GenImageColor(width, height, BLANK);
}

void Pixelator::setPixel(unsigned int x, unsigned int y, const Color& color)
{
    setPixel(m_current_buffer, x, y, color);
}

void Pixelator::setPixel(const std::string& name, unsigned int x, unsigned int y, const Color& color)
{
    unsigned int index = m_buffer_map[name];
    ImageDrawPixel(&m_buffers[index], x, y, color);
}

void Pixelator::drawColumn(const std::string& name, unsigned int x, unsigned int y, unsigned int height, const Color& color)
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

void Pixelator::drawRow(unsigned int x, unsigned int y, unsigned int length, const Color& color)
{
    for (int32_t i = x; i < length; i++)
    {
        setPixel(i, y, color);
    }
}

void Pixelator::drawFilledRect(const std::string& name, const Rectangle& rect, const Color& color)
{
    if (rect.x < getSize().width)
    {
        for (uint32_t i = rect.x; i < rect.x + rect.width; i++)
        {
            if (i < getSize().width)
            {
                drawColumn(name, i, rect.y, rect.height, color);
            }
        }
    }
}

// Doom's version of Bresenham
void Pixelator::drawLine(const Vector2& start, const Vector2& end, const Color& color)
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
void Pixelator::drawCircle(const sf::Vector2i& coord, const int radius, const Color& color)
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

// left top width height
void Pixelator::drawRect(const Rectangle rect, const Color& color)
{
    int right = rect.left + rect.width;
    int bottom = rect.top + rect.height;
    drawLine(sf::Vector2i(rect.left, rect.top), sf::Vector2i(right, rect.top), color);
    drawLine(sf::Vector2i(rect.left, bottom), sf::Vector2i(right, bottom), color);
    drawLine(sf::Vector2i(rect.left, rect.top), sf::Vector2i(rect.left, bottom), color);
    drawLine(sf::Vector2i(right, rect.top), sf::Vector2i(right, bottom), color);
}
*/
Color Pixelator::getPixel(const std::string& name, unsigned int x, unsigned int y)
{
    unsigned int index = m_buffer_map.at(name);
    return GetPixelColor(&m_buffers[index], UNCOMPRESSED_R8G8B8A8);
}

Color* Pixelator::getPixels(const std::string& name)
{
    if(!check_key(m_buffer_map, name))
    {
        TraceLog(LOG_ERROR,"Attempting to get the pixel data of a buffer that doesn't exist!");
        return nullptr;
    }
    unsigned int index = m_buffer_map[name];

    return LoadImageColors(m_buffers[index]);
}

void Pixelator::fill(Color color)
{
    unsigned int index = m_buffer_map.at(m_current_buffer);
    ImageClearBackground(&m_buffers[index], color);
}

void Pixelator::randomize()
{
    unsigned int index = m_buffer_map.at(m_current_buffer);
    Color *pixels = LoadImageColors(m_buffers[index]);
    
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

Rectangle Pixelator::getSize(const std::string name)
{
    Rectangle rect = { 0, 0, static_cast<int>(m_buffers[m_buffer_map[name]].width), static_cast<int>(m_buffers[m_buffer_map[name]].height) };
    return rect;
}
/*
// copies from an image
void Pixelator::copy(const sf::Image& source, unsigned int destX, unsigned int destY, const Rectangle& sourceRect, bool applyAlpha)
{
    copy(source.getPixelsPtr(), sf::Vector2i(source.getSize().x, source.getSize().y), destX, destY, sourceRect, applyAlpha);
}

// copies from a buffer
void Pixelator::copy(const std::string name, unsigned int destX, unsigned int destY, const Rectangle& sourceRect, bool applyAlpha)
{
    if(!check_key(m_buffer_map, name))
    {
        TraceLog(LOG_ERROR,"Attempting to copy from '{}' which doesn't exist!", name.c_str());
        return;
    }
    copy(&m_buffers[m_buffer_map[name]].pixels[0], m_buffers[m_buffer_map[name]].size, destX, destY, sourceRect, applyAlpha);
}

// copies everything from another buffer
void Pixelator::copy(const std::string name, unsigned int x, unsigned int y, bool applyAlpha)
{
    if(!check_key(m_buffer_map, name))
    {
        TraceLog(LOG_ERROR,"Attempting to copy from '{}' which doesn't exist!", name.c_str());
        return;
    }
    Rectangle sourceRect;
    sourceRect.left = sourceRect.top = 0;
    sourceRect.width = m_buffers[m_buffer_map[name]].size.x;
    sourceRect.height = m_buffers[m_buffer_map[name]].size.y;
    copy(&m_buffers[m_buffer_map[name]].pixels[0], m_buffers[m_buffer_map[name]].size, x, y, sourceRect, applyAlpha);
}

// copies from raw pixels
void Pixelator::copy(const unsigned char* source_pixels, const sf::Vector2i buffer_size, unsigned int destX, unsigned int destY, const Rectangle& sourceRect, bool applyAlpha)
{
    // Make sure that both images are valid
    if ((buffer_size.x == 0) || (buffer_size.y == 0) || (m_buffers[m_buffer_map[m_current_buffer]].size.x == 0) || (m_buffers[m_buffer_map[m_current_buffer]].size.y == 0))
        return;

    // Adjust the source rectangle
    Rectangle srcRect = sourceRect;
    if (srcRect.width == 0 || (srcRect.height == 0))
    {
        srcRect.left   = 0;
        srcRect.top    = 0;
        srcRect.width  = buffer_size.x;
        srcRect.height = buffer_size.y;
    }
    else
    {
        if (srcRect.left   < 0) srcRect.left = 0;
        if (srcRect.top    < 0) srcRect.top  = 0;
        if (srcRect.width  > static_cast<int>(buffer_size.x)) srcRect.width  = buffer_size.x;
        if (srcRect.height > static_cast<int>(buffer_size.y)) srcRect.height = buffer_size.y;
    }

    // Then find the valid bounds of the destination rectangle
    int width  = srcRect.width;
    int height = srcRect.height;
    if (destX + width  > m_buffers[m_buffer_map[m_current_buffer]].size.x) width  = m_buffers[m_buffer_map[m_current_buffer]].size.x - destX;
    if (destY + height > m_buffers[m_buffer_map[m_current_buffer]].size.y) height = m_buffers[m_buffer_map[m_current_buffer]].size.y - destY;

    // Make sure the destination area is valid
    if ((width <= 0) || (height <= 0))
        return;

    // Precompute as much as possible
    int          pitch     = width * 4;
    int          rows      = height;
    int          srcStride = static_cast<int>(buffer_size.x * 4u);
    int          dstStride = static_cast<int>(m_buffers[m_buffer_map[m_current_buffer]].size.x * 4u);
    const unsigned char* srcPixels = source_pixels;
    unsigned char*       dstPixels = &m_buffers[m_buffer_map[m_current_buffer]].pixels[0] + (destX + destY * static_cast<int>(m_buffers[m_buffer_map[m_current_buffer]].size.x)) * 4u;

    // Copy the pixels
    if (applyAlpha)
    {
        // Interpolation using alpha values, pixel by pixel (slower)
        for (int i = 0; i < rows; ++i)
        {
            for (int j = 0; j < width; ++j)
            {
                // Get a direct pointer to the components of the current pixel
                const unsigned char* src = srcPixels + j * 4;
                unsigned char*       dst = dstPixels + j * 4;

                // Interpolate RGBA components using the alpha value of the source pixel
                unsigned char alpha = src[3];
                dst[0] = (src[0] * alpha + dst[0] * (255 - alpha)) / 255;
                dst[1] = (src[1] * alpha + dst[1] * (255 - alpha)) / 255;
                dst[2] = (src[2] * alpha + dst[2] * (255 - alpha)) / 255;
                dst[3] = alpha + dst[3] * (255 - alpha) / 255;
            }

            srcPixels += srcStride;
            dstPixels += dstStride;
        }
    }
    else
    {
        // Optimized copy ignoring alpha values, row by row (faster)
        for (int i = 0; i < rows; ++i)
        {
            std::memcpy(dstPixels, srcPixels, pitch);
            srcPixels += srcStride;
            dstPixels += dstStride;
        }
    }
}
*/
