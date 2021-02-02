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
    m_buffers.emplace_back();
    setSize(sf::Vector2i(360, 240));
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
        TraceLog(LOG_ERROR,"Attempting to add '{}' which already exist!", name.c_str());
        return false;
    }

    const unsigned int newBufferIndex{ static_cast<unsigned int>(m_buffers.size()) };
    m_buffer_map.insert({name, newBufferIndex});
    m_buffers.emplace_back();

    unsigned int index = m_buffer_map[m_current_buffer];

    std::vector<unsigned char> newPixels(m_buffers[index].size.x * m_buffers[index].size.y * 4u);
    unsigned char* ptr = &newPixels[0];
    unsigned char* end = ptr + newPixels.size();
    while (ptr < end)
    {
        *ptr++ = BLANK.r;
        *ptr++ = BLANK.g;
        *ptr++ = BLANK.b;
        *ptr++ = BLANK.a;
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

    m_buffers[swap_index].pixels.swap(m_buffers[own_index].pixels);

    return true;
}

void Pixelator::setSize(const sf::Vector2i size)
{
    setSize(m_current_buffer, size);
}

void Pixelator::setSize(const std::string& name, const sf::Vector2i size)
{
    unsigned int index = m_buffer_map[name];
    m_buffers[index].size = size;
    std::vector<unsigned char> newPixels(m_buffers[index].size.x * m_buffers[index].size.y * 4u);
    unsigned char* ptr = &newPixels[0];
    unsigned char* end = ptr + newPixels.size();
    while (ptr < end)
    {
        *ptr++ = BLANK.r;
        *ptr++ = BLANK.g;
        *ptr++ = BLANK.b;
        *ptr++ = BLANK.a;
    }
    // Commit the new pixel buffer
    m_buffers[index].pixels.swap(newPixels);
    m_buffers[index].size = size;
}

void Pixelator::setPixel(unsigned int x, unsigned int y, const Color& color)
{
    setPixel(m_current_buffer, x, y, color);
}

void Pixelator::setPixel(const std::string& name, unsigned int x, unsigned int y, const Color& color)
{
    unsigned int index = m_buffer_map[name];
    unsigned char* pixel = &m_buffers[index].pixels[(x + y * m_buffers[index].size.x) * 4];
    *pixel++ = color.r;
    *pixel++ = color.g;
    *pixel++ = color.b;
    *pixel++ = color.a;
}

void Pixelator::drawColumn(unsigned int x, unsigned int y, unsigned int height, const Color& color)
{
    drawColumn(m_current_buffer, x, y, height, color);
}

void Pixelator::drawColumn(const std::string& name, unsigned int x, unsigned int y, unsigned int height, const Color& color)
{
   if (y < 0)
    {
        height = height + y;
        y = 0;
    }
    if (y + height > m_buffers[m_buffer_map[m_current_buffer]].size.y)
    {
        height = m_buffers[m_buffer_map[m_current_buffer]].size.y - y;
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

// draw a rect defined by left, top, width, height
void Pixelator::drawFilledRect(const Rectangle& rect, const Color& color)
{
    drawFilledRect(m_current_buffer, rect, color);
}

void Pixelator::drawFilledRect(const std::string& name, const Rectangle& rect, const Color& color)
{
    if (rect.left < getSize().width)
    {
        for (uint32_t i = rect.left; i < rect.left + rect.width; i++)
        {
            if (i < getSize().width)
            {
                drawColumn(name, i, rect.top, rect.height, color);
            }
        }
    }
}

// Doom's version of Bresenham
void Pixelator::drawLine(const sf::Vector2i& start, const sf::Vector2i& end, const Color& color)
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

Color Pixelator::getPixel(unsigned int x, unsigned int y) const
{
    return getPixel(m_current_buffer, x, y);
}

Color Pixelator::getPixel(const std::string& name, unsigned int x, unsigned int y) const
{
    unsigned int index = m_buffer_map.at(name);
    const unsigned char* pixel = &m_buffers[index].pixels[(x + y * m_buffers[index].size.x) * 4];
    return Color(pixel[0], pixel[1], pixel[2], pixel[3]);
}

const unsigned char* Pixelator::getPixelsPtr() const
{
    return getPixelsPtr(m_current_buffer);
}

const unsigned char* Pixelator::getPixelsPtr(const std::string& name) const
{
    unsigned int index = m_buffer_map.at(name);
    if (!m_buffers[index].pixels.empty())
    {
        return &m_buffers[index].pixels[0];
    }
    else
    {
        TraceLog(LOG_ERROR,"Trying to access the pixels of an empty image");
        return NULL;
    }
}

void Pixelator::fill(Color color)
{
    unsigned int index = m_buffer_map.at(m_current_buffer);
    std::vector<unsigned char> newPixels(m_buffers[index].size.x * m_buffers[index].size.y * 4);
    unsigned char* ptr = &newPixels[0];
    unsigned char* end = ptr + newPixels.size();
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

void Pixelator::randomize()
{
    unsigned int index = m_buffer_map.at(m_current_buffer);
    std::vector<unsigned char> newPixels(m_buffers[index].size.x * m_buffers[index].size.y * 4);
    unsigned char* ptr = &newPixels[0];
    unsigned char* end = ptr + newPixels.size();
    while (ptr < end)
    {
        *ptr++ = rand() % 255;
        *ptr++ = rand() % 255;
        *ptr++ = rand() % 255;
        *ptr++ = rand() % 255;
    }
    // Commit the new pixel buffer
    m_buffers[index].pixels.swap(newPixels);
}

void Pixelator::clear()
{
    clear(m_current_buffer);
}

void Pixelator::clear(const std::string& name)
{
    unsigned int index = m_buffer_map.at(name);
    std::vector<unsigned char> newPixels(m_buffers[index].size.x * m_buffers[index].size.y * 4);
    unsigned char* ptr = &newPixels[0];
    unsigned char* end = ptr + newPixels.size();
    while (ptr < end)
    {
        *ptr++ = BLANK.r;
        *ptr++ = BLANK.g;
        *ptr++ = BLANK.b;
        *ptr++ = BLANK.a;
    }
    // Commit the new pixel buffer
    m_buffers[index].pixels.swap(newPixels);
}

Rectangle Pixelator::getSize(const std::string name)
{
    Rectangle rect(0, 0, static_cast<int>(m_buffers[m_buffer_map[name]].size.x), static_cast<int>(m_buffers[m_buffer_map[name]].size.y));
    return rect;
}

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
