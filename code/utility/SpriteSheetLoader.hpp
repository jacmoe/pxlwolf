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

namespace utility
{
    class SpriteSheetLoader
    {
    public:
        SpriteSheetLoader();
        ~SpriteSheetLoader();

        bool load(const std::string& sprite_definition_file);

        inline const auto& getAnimations() { return m_animations; }

    private:
        std::unordered_map<std::string, std::pair<thor::FrameAnimation, sf::Time>> m_animations;
        int m_rows;
        int m_cols;
        int m_width;
        int m_height;
    };
}
