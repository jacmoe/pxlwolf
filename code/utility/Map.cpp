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
#include "Map.hpp"
#include "rapidjson/filereadstream.h"
#include "rapidjson/document.h"
#include "rapidjson/istreamwrapper.h"
#include "physfs.hpp"

#include <algorithm>
#include <iostream>
#include <fstream>

namespace utility
{
    Map::Map()
    : m_map_width(0)
    , m_map_height(0)
    , m_player_start( { 0.0, 0.0 })
    , m_player_heading(0.0)
    , m_initialized(false)
    , m_loaded(false)
    {}

    Map::~Map()
    {}

    double Map::deg2rad (double degrees) {
        return degrees * 4.0 * atan (1.0) / 180.0;
    }

    bool Map::init(const std::string& file_name, bool from_zip)
    {
        rapidjson::Document document;
        std::fstream fp;

        if(from_zip)
        {
            PhysFS::ifstream fp(file_name);
            if(!fp)
            {
                TraceLog(LOG_ERROR,"Was unable to load '%s' from zip", file_name.c_str());
                return false;
            }
            rapidjson::IStreamWrapper isw(fp);
        document.ParseStream(isw);
        } else {
            fp.open(file_name, std::fstream::in);
            if(!fp)
            {
                TraceLog(LOG_ERROR,"Was unable to load '%s'", file_name.c_str());
                return false;
            }
            rapidjson::IStreamWrapper isw(fp);
            document.ParseStream(isw);
        }

        const rapidjson::Value& layers = document["defs"]["layers"];
        for (rapidjson::Value::ConstValueIterator itr = layers.Begin(); itr != layers.End(); ++itr)
        {
            std::string layer_identifier = (*itr)["identifier"].GetString();
            TraceLog(LOG_INFO,"Layer definition for %s found.", layer_identifier.c_str());
            if(layer_identifier == "Walls")
            {
                const rapidjson::Value& wall_values = (*itr)["intGridValues"];
                for (rapidjson::Value::ConstValueIterator itr = wall_values.Begin(); itr != wall_values.End(); ++itr)
                {
                    std::string wall_identifier = (*itr)["identifier"].GetString();
                    std::string wall_color = (*itr)["color"].GetString();
                    std::replace(wall_color.begin(), wall_color.end(), '#', 'x');
                    wall_color = "0" + wall_color;
                    TraceLog(LOG_INFO,"Wall '%s' has color %s", wall_identifier.c_str(), wall_color.c_str());
                    unsigned int wall_color_int = std::stoul(wall_color, nullptr, 16);
                    unsigned char r = ((wall_color_int >> 16) & 0xFF);  // Extract the RR byte
                    unsigned char g = ((wall_color_int >> 8) & 0xFF);   // Extract the GG byte
                    unsigned char b = ((wall_color_int) & 0xFF);        // Extract the BB byte
                    TraceLog(LOG_INFO,"Color '%d' is r %d, g %d, b %d", wall_color_int, r, g, b);
                    m_wall_elements.push_back({wall_identifier, {r, g, b, 255}});
                }
            }

        }

        const rapidjson::Value& levels = document["levels"];
        for (rapidjson::Value::ConstValueIterator itr = levels.Begin(); itr != levels.End(); ++itr)
        {
            std::string level_name = (*itr)["identifier"].GetString();
            std::string level_path = (*itr)["externalRelPath"].GetString();
            TraceLog(LOG_INFO,"Found level %s in %s", level_name.c_str(), level_path.c_str());
            m_level_map.insert({ level_name, level_path});
        }

        TraceLog(LOG_INFO,"File '%s' initialized.", file_name.c_str());
        m_initialized = true;

        if(from_zip)
        {
            // I am probably leaving resources dangling, oh no . . . ;)
        } else {
            if(fp) fp.close();
        }
        return true;
    }

    bool Map::load(const std::string& level_name, bool from_zip)
    {
        if (m_level_map.find(level_name) == m_level_map.end())
        {
            TraceLog(LOG_ERROR,"Level '%s' does not exist!", level_name.c_str());
            return false;
        }

        std::string level_file = "assets/levels/" + m_level_map[level_name];
        rapidjson::Document document;
        std::fstream fp;

        if(from_zip)
        {
            PhysFS::ifstream fp(level_file);
            if(!fp)
            {
                TraceLog(LOG_ERROR,"Was unable to load '%s' from zip", level_file.c_str());
                return false;
            }
            rapidjson::IStreamWrapper isw(fp);
        document.ParseStream(isw);
        } else {
            fp.open(level_file, std::fstream::in);
            if(!fp)
            {
                TraceLog(LOG_ERROR,"Was unable to load '%s'", level_file.c_str());
                return false;
            }
            rapidjson::IStreamWrapper isw(fp);
            document.ParseStream(isw);
        }

        TraceLog(LOG_INFO,"Loading level '%s'", level_name.c_str());

        m_map_width = document["pxWid"].GetInt();
        m_map_height = document["pxHei"].GetInt();

        const rapidjson::Value& layerInstances = document["layerInstances"];
        for (rapidjson::Value::ConstValueIterator itr = layerInstances.Begin(); itr != layerInstances.End(); ++itr)
        {
            std::string layer_type = (*itr)["__type"].GetString();
            if(layer_type == "Entities")
            {
                const rapidjson::Value& entityInstances = (*itr)["entityInstances"];
                for (rapidjson::Value::ConstValueIterator itr = entityInstances.Begin(); itr != entityInstances.End(); ++itr)
                {
                    std::string identifier = (*itr)["__identifier"].GetString();
                    if(identifier == "PlayerStart")
                    {
                        m_player_start.x = (*itr)["__grid"].GetArray()[0].GetFloat() + 0.5f;
                        m_player_start.y = (*itr)["__grid"].GetArray()[1].GetFloat() + 0.5f;
                        m_player_heading = static_cast<double>(deg2rad((*itr)["fieldInstances"].GetArray()[0]["__value"].GetDouble()));
                        TraceLog(LOG_INFO,"PlayerStart : (%.2f, %.2f), and angle is %.2f", m_player_start.x, m_player_start.y, m_player_heading);
                    }
                }
            } // if layer type is Entities
            if(layer_type == "IntGrid")
            {
                m_map_width = (*itr)["__cWid"].GetInt();
                m_map_height = (*itr)["__cHei"].GetInt();

                std::string identifier = (*itr)["__identifier"].GetString();
                TraceLog(LOG_INFO,"Parsing IntGrid '%s' . . .", identifier.c_str());

                // Set the size of walls, floor and ceiling vectors and fill it with zeroes.
                // For debugging purposes, only zero out if found
                if(identifier == "Walls")
                {
                    m_walls.assign(m_map_width * m_map_height, 0);
                }
                if(identifier == "Floor")
                {
                    m_floor.assign(m_map_width * m_map_height, 0);
                }
                if(identifier == "Ceiling")
                {
                    m_ceiling.assign(m_map_width * m_map_height, 0);
                }

                const rapidjson::Value& initGrid = (*itr)["intGrid"];

                for (rapidjson::Value::ConstValueIterator itr = initGrid.Begin(); itr != initGrid.End(); ++itr)
                {
                    if(identifier == "Walls")
                    {
                        // Add one to wall values so that zero becomes walkable area
                        m_walls[(*itr)["coordId"].GetInt()] = (*itr)["v"].GetInt() + 1;
                    }
                    if(identifier == "Floor")
                    {
                        // Add one to floor values because LDtk starts from zero
                        m_floor[(*itr)["coordId"].GetInt()] = (*itr)["v"].GetInt() + 1;
                    }
                    if(identifier == "Ceiling")
                    {
                        // Add one to ceiling values because LDtk starts from zero
                        m_ceiling[(*itr)["coordId"].GetInt()] = (*itr)["v"].GetInt() + 1;
                    }
                }
            } // if layer type is IntGrid

        } // for layer instances
        
        TraceLog(LOG_INFO,"Level '%s' loaded.", level_name.c_str());
        m_loaded = true;

        if(from_zip)
        {
            // I am probably leaving resources dangling, oh no . . . ;)
        } else {
            if(fp) fp.close();
        }
        return true;
    }

    int Map::get_wall_entry(int tile_x, int tile_y)
    {
        int item = int(tile_y) * m_map_width + int(tile_x);
        return m_walls[item];
    }
}
