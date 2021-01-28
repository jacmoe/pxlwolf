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
#include "toml.hpp"

namespace utility
{
    SpriteSheetLoader::SpriteSheetLoader()
    : m_animations()
    , m_rows(0)
    , m_cols(0)
    , m_width(0)
    , m_height(0)
    {}

    SpriteSheetLoader::~SpriteSheetLoader()
    {}

    bool SpriteSheetLoader::load(const std::string& sprite_definition_file)
    {
        auto sprite_definition = toml::parse(sprite_definition_file);
        const auto& image_info = toml::find(sprite_definition, "image");
        toml::table image_table = toml::get<toml::table>(image_info);

        sf::Vector2u tile_size;
        tile_size.x = image_table["tile_width"].as_integer();
        tile_size.y = image_table["tile_height"].as_integer();

        std::string image_path = image_table["filename"].as_string();

        sf::Image source_image;
        if(!source_image.loadFromFile(image_path))
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

        std::vector<sf::IntRect> raw_frames;

        // unsigned int count = 0;
        for (unsigned y = 0; y < cols; y++)
        {
            for (unsigned x = 0; x < rows; x++)
            {
                raw_frames.emplace_back(x * m_width, y * m_height, m_width, m_height);
                // SPDLOG_INFO("Adding raw frame {} : {},{} with rect : {},{},{},{}", count, x, y, x * m_width, y * m_height, m_width, m_height);
                // count++;
            }
        }

        const auto animation_table = toml::find<std::vector<toml::table>>(sprite_definition, "animation");

        for(const auto& animations: animation_table)
        {
            thor::FrameAnimation animation;
            std::string animation_name = animations.at("name").as_string();
            const auto& frames = animations.at("frames").as_array();
            for(const auto& frame :  frames)
            {
                animation.addFrame(1.0f, raw_frames[frame.as_integer()]);
                // SPDLOG_INFO("Frame with intrect {},{},{},{} added.", raw_frames[frame.as_integer()].left, raw_frames[frame.as_integer()].top, raw_frames[frame.as_integer()].width, raw_frames[frame.as_integer()].height);
            }
            m_animations.insert( { animation_name, std::make_pair(animation, sf::seconds(animations.at("duration").as_floating())) } );
            SPDLOG_INFO("Added '{}'. Duration : {} seconds. {} frames.", animation_name , animations.at("duration").as_floating(), frames.size());
        }
        return true;
    }
}
