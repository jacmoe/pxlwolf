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
#include <unordered_map>
#include <SFML/Graphics/Color.hpp>
#include <SFML/System/Vector2.hpp>

struct WallElement
{
    std::string name;
    sf::Color color;
};

class Map
{
public:
    Map();
    ~Map();

    bool init(const std::string& file_name, bool from_zip = false);
    bool load_level(const std::string& level_name, bool from_zip = false);

    const std::vector<int>& get_walls();
    const std::vector<int>& get_floor();
    const std::vector<int>& get_ceiling();
    inline const bool initialized() { return m_initialized; }
    inline const bool loaded() { return m_loaded; }

    const WallElement& get_wall_element(const unsigned int element);

    const int width() { return m_map_width; }
    const int height() { return m_map_height; }
    inline const sf::Vector2f player_start() { return m_player_start; }

private:
    std::vector<int> m_walls;
    std::vector<int> m_floor;
    std::vector<int> m_ceiling;
    std::vector<WallElement> m_wall_elements;
    std::unordered_map<std::string, std::string> m_level_map;
    int m_map_width;
    int m_map_height;
    sf::Vector2f m_player_start;
    double m_player_heading;
    bool m_initialized;
    bool m_loaded;

    int get_wall_entry(int tile_x, int tile_y);
    double deg2rad (double degrees);
};
