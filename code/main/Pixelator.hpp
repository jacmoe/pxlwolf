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
#include "raylib.h"

// C64 color pallette, why not
extern Color commodoreColorPallette[16];

class Pixelator
{
public:
    Pixelator();
    ~Pixelator();

    void setSize(const int width, const int height);
    void setSize(const std::string& name, const int width, const int height);

    // Pixels
    void setPixel(unsigned int x, unsigned int y, const Color& color);
    void setPixel(const std::string& name, unsigned int x, unsigned int y, const Color& color);
    // Color getPixel(unsigned int x, unsigned int y) const;
    // Color getPixel(const std::string& name, unsigned int x, unsigned int y) const;
    Color* getPixels() { return getPixels(m_current_buffer); }
    Color* getPixels(const std::string& name);

    // void drawColumn(unsigned int x, unsigned int y, unsigned int height, const Color& color);
    // void drawColumn(const std::string& name, unsigned int x, unsigned int y, unsigned int height, const Color& color);
    // void drawRow(unsigned int x, unsigned int y, unsigned int length, const Color& color);
    // void drawRect(const Rectangle rect, const Color& color);
    // void drawFilledRect(const Rectangle& rect, const Color& color);
    // void drawFilledRect(const std::string& name, const Rectangle& rect, const Color& color);
    void drawLine(const Vector2& start, const Vector2& end, const Color& color);
    // void drawCircle(const Vector2& coord, const int radius, const Color& color);

    void fill(Color color);
    void randomize();
    inline void clear() { clear(m_current_buffer); }
    void clear(const std::string& name);

    // Buffers
    bool addBuffer(const std::string name, const int width, const int height);
    bool removeBuffer(const std::string name);
    void setActiveBuffer(const std::string name);
    const std::string getActiveBuffer() { return m_current_buffer; }
    unsigned int getNumberOfBuffers() const { return static_cast<unsigned int>(m_buffers.size()); }

    bool swapBuffer(const std::string name);

    // copy pixels from a source
    // void copy(const std::string name, unsigned int x = 0, unsigned int y = 0, bool applyAlpha = false);
    // void copy(const std::string name, unsigned int destX, unsigned int destY, const Rectangle& sourceRect, bool applyAlpha = false);
    // void copy(const Image& source, unsigned int destX, unsigned int destY, const Rectangle& sourceRect, bool applyAlpha = false);
    // void copy(const unsigned char* source_pixels, const Vector2 buffer_size, unsigned int destX, unsigned int destY, const Rectangle& sourceRect, bool applyAlpha = false);

    // get the size (width and height) of a buffer
    Rectangle getSize() { return getSize(m_current_buffer); }
    Rectangle getSize(const std::string name);

private:
    std::string m_current_buffer;
    std::unordered_map<std::string, unsigned int> m_buffer_map;

    std::vector<Image> m_buffers;
};
