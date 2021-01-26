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

namespace utility
{
    ImageAtlas::ImageAtlas()
    : m_buffers(0)
    , m_rows(0)
    , m_cols(0)
    , m_width(0)
    , m_height(0)
    {}

    ImageAtlas::~ImageAtlas()
    {}

    bool ImageAtlas::init(const std::string& path, sf::Vector2u tile_size)
    {
        sf::Image source_image;
        if(!source_image.loadFromFile(path))
        {
            return false;
        }

        const auto rows = source_image.getSize().x / tile_size.x;
        const auto cols = source_image.getSize().y / tile_size.y;
        unsigned int index = 0;

        m_rows = rows;
        m_cols = cols;

        m_width =  tile_size.x;
        m_height = tile_size.y;
        SPDLOG_INFO("source image dimensions : width {}, height {}", source_image.getSize().x, source_image.getSize().y);
        SPDLOG_INFO("m_width = {}, m_height = {}", m_width, m_height);

        for (unsigned y = 0; y < cols; ++y)
        {
            for (unsigned x = 0; x < rows; ++x)
            {
                sf::Image image;
                image.create(m_width, m_height);
                image.copy(source_image, 0, 0, sf::IntRect(x * m_width, y * m_height, m_width, m_height));
                SPDLOG_INFO("Intrect is ({}, {}, {}, {})", x * m_width, y * m_height, m_width, m_height);
                m_buffers.push_back(image);
            }
        }
        return true;
    }
}
