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
#include "SpriteSheetLoader.hpp"
#include "spdlog/spdlog.h"

namespace utility
{
    SpriteSheetLoader::SpriteSheetLoader()
    : m_raw_frames(0)
    , m_animations(0)
    , m_rows(0)
    , m_cols(0)
    , m_width(0)
    , m_height(0)
    {}

    SpriteSheetLoader::~SpriteSheetLoader()
    {}

    bool SpriteSheetLoader::load(const std::string& path, sf::Vector2u tile_size)
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
                m_raw_frames.emplace_back(x * m_width, y * m_height, m_width, m_height);
            }
        }
        SPDLOG_INFO("Added {} raw frames.", m_raw_frames.size());

        thor::FrameAnimation animation;
        for (unsigned anis = 0; anis < 6; ++anis)
        {
            animation.addFrame(1.0f, m_raw_frames[anis]);
        }
        m_animations.insert({ "stand_back", animation});

        return true;
    }
}
