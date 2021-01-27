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
#include <unordered_map>
#include <SFML/Graphics.hpp>
#include "thor/Animations/FrameAnimation.hpp"

namespace utility
{
    class SpriteSheetLoader
    {
    public:
        SpriteSheetLoader();
        ~SpriteSheetLoader();

        bool load(const std::string& path, sf::Vector2u tile_size);

        inline const std::unordered_map<std::string, thor::FrameAnimation> getAnimations() { return m_animations; }

    private:
        std::vector<sf::IntRect> m_raw_frames;
        std::unordered_map<std::string, thor::FrameAnimation> m_animations;
        int m_rows;
        int m_cols;
        int m_width;
        int m_height;
    };
}
