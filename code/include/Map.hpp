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

private:
    std::vector<int> walls;
    std::vector<int> floor;
    std::vector<int> ceiling;
    std::vector<MapElement> wall_elements;
    std::list<std::string> level_names;
    int map_width;
    int map_height;
    float player_x;
    float player_y;
    float player_heading;
    bool initialized;

    int get_wall_entry(int tile_x, int tile_y);
    double deg2rad (double degrees);
};

struct RGB
{
    double r;
    double g;
    double b;
};
RGB colorConverter(int hexValue);
