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
#include <list>
#include <string>
#include <SFML/Graphics/Color.hpp>
#include <SFML/System/Vector2.hpp>

struct MapElement
{
    std::string name;
    sf::Color color;
};

class Map
{
public:
    Map();
    ~Map();

    bool load(const std::string& file_name, const std::string& level_name, bool from_zip = false);

    const std::vector<int>& get_walls();
    const std::vector<int>& get_floor();
    const std::vector<int>& get_ceiling();
    const bool is_initialized();

    const MapElement& get_wall_element(const unsigned int element);

    const int width() { return map_width; }
    const int height() { return map_height; }
    const sf::Vector2f player_start() { return m_player_start; }

private:
    std::vector<int> walls;
    std::vector<int> floor;
    std::vector<int> ceiling;
    std::vector<MapElement> wall_elements;
    std::list<std::string> level_names;
    int map_width;
    int map_height;
    sf::Vector2f m_player_start;
    double m_player_heading;
    bool initialized;

    int get_wall_entry(int tile_x, int tile_y);
    double deg2rad (double degrees);
};
