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

#include <vector>
#include <string>
#include "SDL.h"
#include "linalg.h"

namespace utility
{
    class ImageAtlas
    {
    public:
        ImageAtlas();
        ~ImageAtlas();

        bool load(const std::string& path, linalg::aliases::int2 tile_size);

        inline const linalg::aliases::int2 getTileSize() { return { m_width, m_height }; }
        inline const int getCols() { return m_cols; }
        inline const int getRows() { return m_rows; }

        uint32_t* getPixels(int index) { return &m_buffers[index].pixels[0]; }
        uint32_t getPixel(int index, int x , int y) { return m_buffers[index].pixels[(y*m_buffers[index].width+x)]; }

    private:
        struct Buffer
        {
            uint32_t* pixels;
            uint32_t width;
            uint32_t height;
        };
        std::vector<Buffer> m_buffers;
        int m_rows;
        int m_cols;
        int m_width;
        int m_height;
    };
}
