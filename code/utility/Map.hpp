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
#include <allegro5/allegro.h>

#include "main/types.hpp"

enum class EntityType;

namespace utility
{
    struct WallElement
    {
        std::string name;
        ALLEGRO_COLOR color;
    };

    struct MapItem
    {
        std::string type;
        enum EntityType e_type;
        int map_x;
        int map_y;
        double angle;
    };

    class Map
    {
    public:
        Map();
        ~Map();

        bool init(const std::string& file_name, bool from_zip = false);
        bool load(const std::string& level_name, bool from_zip = false);

        inline const std::vector<int>& walls() { return m_walls; }
        inline const std::vector<int>& floor() { return m_floor; }
        inline const std::vector<int>& ceiling() { return m_ceiling; }
        inline const std::vector<MapItem>& statics() { return m_map_statics; }
        inline const std::vector<MapItem>& pickups() { return m_map_pickups; }
        inline const std::vector<MapItem>& enemies() { return m_map_enemies; }
        inline const std::vector<MapItem>& keys() { return m_map_keys; }
        inline const bool initialized() { return m_initialized; }
        inline const bool loaded() { return m_loaded; }

        inline const WallElement& wall_element(const unsigned int element) { return m_wall_elements[element - 1]; }
        int get_wall_entry(int tile_x, int tile_y);

        inline const int width() { return m_map_width; }
        inline const int height() { return m_map_height; }
        inline const Vector2d player_start() { return m_player_start; }
        inline const double player_heading() { return m_player_heading; }

    private:
        std::vector<int> m_walls;
        std::vector<int> m_floor;
        std::vector<int> m_ceiling;
        std::vector<WallElement> m_wall_elements;
        std::vector<MapItem> m_map_statics;
        std::vector<MapItem> m_map_pickups;
        std::vector<MapItem> m_map_enemies;
        std::vector<MapItem> m_map_keys;
        std::unordered_map<std::string, std::string> m_level_map;
        int m_map_width;
        int m_map_height;
        Vector2d m_player_start;
        double m_player_heading;
        bool m_initialized;
        bool m_loaded;

        double deg2rad (double degrees);
    };
}
