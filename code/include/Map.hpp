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

class Map
{
public:
    Map();
    ~Map();

    bool load(const std::string& file_name, const std::string& level_name, bool from_zip = false);

    const std::vector<int>& get_walls();
    const std::vector<int>& get_floor();
    const std::vector<int>& get_ceiling();

private:
    std::vector<int> walls;
    std::vector<int> floor;
    std::vector<int> ceiling;
    std::list<std::string> level_names;
    int map_width;
    int map_height;
    int player_x;
    int player_y;
    float player_heading;

    int get_wall_entry(int tile_x, int tile_y);
    double deg2rad (double degrees);
};
