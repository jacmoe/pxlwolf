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

class ImageAtlas
{
public:
    ImageAtlas();
    ~ImageAtlas();
    bool Init(const std::string& path, int rows, int cols);
    const sf::Uint8* GetPixels(int idx);
    const sf::Vector2<int> GetImageDimensions();
    const int GetCols();
    const int GetRows();
private:
    std::vector<sf::Image> m_buffers;
    int m_rows;
    int m_cols;
    int m_width;
    int m_height;
};
