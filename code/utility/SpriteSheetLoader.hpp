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
#include "Thor/Animations/FrameAnimation.hpp"
#include "Thor/Animations/AnimationMap.hpp"

namespace utility
{
    class SpriteSheetLoader
    {
    public:
        SpriteSheetLoader();
        ~SpriteSheetLoader();

        bool load(const std::string& sprite_definition_file);

        inline const thor::AnimationMap<sf::Sprite, std::string> getAnimations() { return m_animations; }

    private:
        thor::AnimationMap<sf::Sprite, std::string> m_animations;
        int m_rows;
        int m_cols;
        int m_width;
        int m_height;
    };
}
