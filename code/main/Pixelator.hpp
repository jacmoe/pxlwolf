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

#include <unordered_map>
#include <vector>
#include "SDL.h"
#include "linalg.h"

class Pixelator
{
public:
    Pixelator();
    ~Pixelator();

    // // Pixels
    void setPixel(unsigned int x, unsigned int y, const SDL_Color& color);
    void setPixel(const std::string& name, unsigned int x, unsigned int y, const SDL_Color& color);
    uint32_t getPixel(unsigned int x, unsigned int y) { return getPixel(m_current_buffer, x, y); }
    uint32_t getPixel(const std::string& name, unsigned int x, unsigned int y);
    uint32_t* getPixels() { return getPixels(m_current_buffer); }
    uint32_t* getPixels(const std::string& name);

    void drawColumn(unsigned int x, unsigned int y, unsigned int height, const SDL_Color& color) { drawColumn(m_current_buffer, x, y, height, color); }
    void drawColumn(const std::string& name, unsigned int x, unsigned int y, unsigned int height, const SDL_Color& color);
    void drawRow(unsigned int x, unsigned int y, unsigned int length, const SDL_Color& color);
    void drawRect(const SDL_Rect rect, const SDL_Color& color);
    inline void drawFilledRect(const SDL_Rect& rect, const SDL_Color& color) { drawFilledRect(m_current_buffer, rect, color); }
    void drawFilledRect(const std::string& name, const SDL_Rect& rect, const SDL_Color& color);
    void drawLine(const linalg::aliases::int2& start, const linalg::aliases::int2& end, const SDL_Color& color);
    void drawCircle(const linalg::aliases::int2& coord, const int radius, const SDL_Color& color);

    void fill(SDL_Color color) { fill(m_current_buffer, color); }
    void fill(const std::string& name, SDL_Color color);
    void randomize() { randomize(m_current_buffer); }
    void randomize(const std::string& name);
    inline void clear() { clear(m_current_buffer); }
    void clear(const std::string& name);

    // Buffers
    bool addBuffer(const std::string name, const int width, const int height);
    bool removeBuffer(const std::string name);
    void setActiveBuffer(const std::string name);
    const std::string getActiveBuffer() { return m_current_buffer; }
    unsigned int getNumberOfBuffers() const { return static_cast<unsigned int>(m_buffers.size()); }
    void setSize(const int width, const int height);
    void setSize(const std::string& name, const int width, const int height);
    // get the size (width and height) of a buffer
    linalg::aliases::uint2 getSize() { return getSize(m_current_buffer); }
    linalg::aliases::uint2 getSize(const std::string name);

    bool swapBuffer(const std::string name);

    void mergeBuffer(const std::string& source) { mergeBuffer(m_current_buffer, source); }
    void mergeBuffer(const std::string& target, const std::string& source);

    uint32_t toIntColor(SDL_Color color);
    uint32_t toIntColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a);
    SDL_Color toSDLColor(uint32_t pixColor);
    uint32_t to8BitColor(uint32_t colorDat);

private:
    struct Buffer
    {
        std::vector<uint32_t> pixels;
        uint32_t width;
        uint32_t height;
    };

    std::string m_current_buffer;
    std::unordered_map<std::string, unsigned int> m_buffer_map;

    std::vector<Buffer> m_buffers;
};
