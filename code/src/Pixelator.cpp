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
    setSize(sf::Vector2i(360.0f, 240.0f));
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

void Pixelator::setSize(const sf::Vector2i size)
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

sf::IntRect Pixelator::getSize(const std::string name)
{
    sf::IntRect rect(0, 0, static_cast<int>(m_buffers[m_buffer_map[name]].size.x), static_cast<int>(m_buffers[m_buffer_map[name]].size.y));
    return rect;
}

void Pixelator::copy(const sf::Image& source, unsigned int destX, unsigned int destY, const sf::IntRect& sourceRect, bool applyAlpha)
{
    // Make sure that both images are valid
    if ((source.getSize().x == 0) || (source.getSize().y == 0) || (m_buffers[m_buffer_map[m_current_buffer]].size.x == 0) || (m_buffers[m_buffer_map[m_current_buffer]].size.y == 0))
        return;

    // Adjust the source rectangle
    sf::IntRect srcRect = sourceRect;
    if (srcRect.width == 0 || (srcRect.height == 0))
    {
        srcRect.left   = 0;
        srcRect.top    = 0;
        srcRect.width  = source.getSize().x;
        srcRect.height = source.getSize().y;
    }
    else
    {
        if (srcRect.left   < 0) srcRect.left = 0;
        if (srcRect.top    < 0) srcRect.top  = 0;
        if (srcRect.width  > static_cast<int>(source.getSize().x)) srcRect.width  = source.getSize().x;
        if (srcRect.height > static_cast<int>(source.getSize().y)) srcRect.height = source.getSize().y;
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
    int          srcStride = source.getSize().x * 4;
    int          dstStride = m_buffers[m_buffer_map[m_current_buffer]].size.x * 4;
    const sf::Uint8* srcPixels = source.getPixelsPtr() + (srcRect.left + srcRect.top * source.getSize().x) * 4;
    sf::Uint8*       dstPixels = &m_buffers[m_buffer_map[m_current_buffer]].pixels[0] + (destX + destY * m_buffers[m_buffer_map[m_current_buffer]].size.x) * 4;

    // Copy the pixels
    if (applyAlpha)
    {
        // Interpolation using alpha values, pixel by pixel (slower)
        for (int i = 0; i < rows; ++i)
        {
            for (int j = 0; j < width; ++j)
            {
                // Get a direct pointer to the components of the current pixel
                const sf::Uint8* src = srcPixels + j * 4;
                sf::Uint8*       dst = dstPixels + j * 4;

                // Interpolate RGBA components using the alpha value of the source pixel
                sf::Uint8 alpha = src[3];
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

void Pixelator::copy(const sf::Uint8* source_pixels, const sf::Vector2i buffer_size, unsigned int destX, unsigned int destY, const sf::IntRect& sourceRect, bool applyAlpha)
{
    // Make sure that both images are valid
    if ((buffer_size.x == 0) || (buffer_size.y == 0) || (m_buffers[m_buffer_map[m_current_buffer]].size.x == 0) || (m_buffers[m_buffer_map[m_current_buffer]].size.y == 0))
        return;

    // Adjust the source rectangle
    sf::IntRect srcRect = sourceRect;
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
    const sf::Uint8* srcPixels = source_pixels;
    sf::Uint8*       dstPixels = &m_buffers[m_buffer_map[m_current_buffer]].pixels[0] + (destX + destY * static_cast<int>(m_buffers[m_buffer_map[m_current_buffer]].size.x)) * 4u;

    // Copy the pixels
    if (applyAlpha)
    {
        // Interpolation using alpha values, pixel by pixel (slower)
        for (int i = 0; i < rows; ++i)
        {
            for (int j = 0; j < width; ++j)
            {
                // Get a direct pointer to the components of the current pixel
                const sf::Uint8* src = srcPixels + j * 4;
                sf::Uint8*       dst = dstPixels + j * 4;

                // Interpolate RGBA components using the alpha value of the source pixel
                sf::Uint8 alpha = src[3];
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

void Pixelator::copy(const std::string name, unsigned int destX, unsigned int destY, const sf::IntRect& sourceRect, bool applyAlpha)
{
    // Make sure that both images are valid
    if ((m_buffers[m_buffer_map[name]].size.x == 0) || (m_buffers[m_buffer_map[name]].size.y == 0) || (m_buffers[m_buffer_map[m_current_buffer]].size.x == 0) || (m_buffers[m_buffer_map[m_current_buffer]].size.y == 0))
        return;

    // Adjust the source rectangle
    sf::IntRect srcRect = sourceRect;
    if (srcRect.width == 0 || (srcRect.height == 0))
    {
        srcRect.left   = 0;
        srcRect.top    = 0;
        srcRect.width  = m_buffers[m_buffer_map[name]].size.x;
        srcRect.height = m_buffers[m_buffer_map[name]].size.y;
    }
    else
    {
        if (srcRect.left   < 0) srcRect.left = 0;
        if (srcRect.top    < 0) srcRect.top  = 0;
        if (srcRect.width  > static_cast<int>(m_buffers[m_buffer_map[name]].size.x)) srcRect.width  = m_buffers[m_buffer_map[name]].size.x;
        if (srcRect.height > static_cast<int>(m_buffers[m_buffer_map[name]].size.y)) srcRect.height = m_buffers[m_buffer_map[name]].size.y;
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
    int          srcStride = static_cast<int>(m_buffers[m_buffer_map[name]].size.x * 4u);
    int          dstStride = static_cast<int>(m_buffers[m_buffer_map[m_current_buffer]].size.x * 4u);
    const sf::Uint8* srcPixels = &m_buffers[m_buffer_map[name]].pixels[0] + (srcRect.left + srcRect.top * static_cast<int>(m_buffers[m_buffer_map[name]].size.x)) * 4u;
    sf::Uint8*       dstPixels = &m_buffers[m_buffer_map[m_current_buffer]].pixels[0] + (destX + destY * static_cast<int>(m_buffers[m_buffer_map[m_current_buffer]].size.x)) * 4u;

    // Copy the pixels
    if (applyAlpha)
    {
        // Interpolation using alpha values, pixel by pixel (slower)
        for (int i = 0; i < rows; ++i)
        {
            for (int j = 0; j < width; ++j)
            {
                // Get a direct pointer to the components of the current pixel
                const sf::Uint8* src = srcPixels + j * 4;
                sf::Uint8*       dst = dstPixels + j * 4;

                // Interpolate RGBA components using the alpha value of the source pixel
                sf::Uint8 alpha = src[3];
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
