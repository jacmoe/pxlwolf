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
#include "raylib.h"

namespace utility
{
    class ImageAtlas
    {
    public:
        ImageAtlas();
        ~ImageAtlas();

        bool load(const std::string& path, Vector2 tile_size);

        inline const Vector2 getTileSize() { return { m_width, m_height }; }
        inline const int getCols() { return m_cols; }
        inline const int getRows() { return m_rows; }

        Color* getPixels(int index) { return m_buffers[index]; }
        Color getPixel(int index, int x , int y) { return m_buffers[index][y * static_cast<int>(m_width) + x]; }

    private:
        std::vector<Color*> m_buffers;
        float m_rows;
        float m_cols;
        float m_width;
        float m_height;
        int m_format;
    };
}
