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

#include "Pixelator.hpp"
#include <SFML/System/Vector2.hpp>

namespace utility
{
    class ImageAtlas
    {
    public:
        ImageAtlas();
        ~ImageAtlas();

        bool init(const std::string& path, sf::Vector2u tile_size);

        inline const sf::Uint8* getPixels(int idx) { return m_buffers[idx].getPixelsPtr(); }
        inline const sf::Vector2i getTileSize() { return sf::Vector2i(m_width, m_height); }
        inline const int getCols() { return m_cols; }
        inline const int getRows() { return m_rows; }

    private:
        std::vector<sf::Image> m_buffers;
        int m_rows;
        int m_cols;
        int m_width;
        int m_height;
    };
}
