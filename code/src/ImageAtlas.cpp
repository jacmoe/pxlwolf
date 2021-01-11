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
#include "ImageAtlas.hpp"
#include "spdlog/spdlog.h"

ImageAtlas::ImageAtlas()
{}

ImageAtlas::~ImageAtlas()
{}

bool ImageAtlas::Init(const std::string& path, int rows, int cols)
{
    sf::Image source_image;
    if(!source_image.loadFromFile(path))
    {
        return false;
    }

    m_rows = rows;
    m_cols = cols;
    m_width =  source_image.getSize().x / cols;
    m_height = source_image.getSize().y / rows;
    SPDLOG_INFO("source image dimensions : width {}, height {}", source_image.getSize().x, source_image.getSize().y);
    SPDLOG_INFO("m_width = {}, m_height = {}", m_width, m_height);

    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            sf::Image image;
            image.create(m_width, m_height);
            image.copy(source_image, 0, 0, sf::IntRect(j * m_width, i * m_height, m_width, m_height));
            SPDLOG_INFO("Intrect is ({}, {}, {}, {})", j * m_width, i * m_height, m_width, m_height);
            m_buffers.push_back(image);
        }
    }
    return true;
}

const sf::Uint8* ImageAtlas::GetPixels(int idx)
{
    return m_buffers[idx].getPixelsPtr();
}

const sf::Vector2<int> ImageAtlas::GetImageDimensions()
{
    return sf::Vector2(m_width, m_height);
}

const int ImageAtlas::GetCols()
{
    return m_cols;
}

const int ImageAtlas::GetRows()
{
    return m_rows;
}
