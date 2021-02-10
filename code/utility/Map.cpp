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
#include "spdlog/spdlog.h"

#include <algorithm>
#include <iostream>
#include <fstream>
#include <cmath>

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

    // From https://codereview.stackexchange.com/a/172521
    SDL_Color hex2sdl(std::string input)
    {
        if (input[0] == '#')
            input.erase(0, 1);

        unsigned long value = stoul(input, nullptr, 16);

        SDL_Color color;

        color.a = (value >> 24) & 0xff;
        color.r = (value >> 16) & 0xff;
        color.g = (value >> 8) & 0xff;
        color.b = (value >> 0) & 0xff;
        color.a = 255;
        return color;
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
                SPDLOG_ERROR("Was unable to load '{}' from zip", file_name.c_str());
                return false;
            }
            rapidjson::IStreamWrapper isw(fp);
        document.ParseStream(isw);
        } else {
            fp.open(file_name, std::fstream::in);
            if(!fp)
            {
                SPDLOG_ERROR("Was unable to load '{}'", file_name.c_str());
                return false;
            }
            rapidjson::IStreamWrapper isw(fp);
            document.ParseStream(isw);
        }

        const rapidjson::Value& layers = document["defs"]["layers"];
        for (rapidjson::Value::ConstValueIterator itr = layers.Begin(); itr != layers.End(); ++itr)
        {
            std::string layer_identifier = (*itr)["identifier"].GetString();
            SPDLOG_INFO("Layer definition for {} found.", layer_identifier);
            if(layer_identifier == "Walls")
            {
                const rapidjson::Value& wall_values = (*itr)["intGridValues"];
                for (rapidjson::Value::ConstValueIterator itr = wall_values.Begin(); itr != wall_values.End(); ++itr)
                {
                    std::string wall_identifier = (*itr)["identifier"].GetString();
                    std::string wall_color = (*itr)["color"].GetString();
                    SDL_Color color = hex2sdl(wall_color);
                    uint32_t the_color = ((uint32_t)color.r << 3*8 | (uint32_t)color.g << 2*8 | (uint32_t)color.b << 8 | (uint32_t)color.a);
                    m_wall_elements.push_back({wall_identifier, the_color});
                }
            }

        }

        const rapidjson::Value& levels = document["levels"];
        for (rapidjson::Value::ConstValueIterator itr = levels.Begin(); itr != levels.End(); ++itr)
        {
            std::string level_name = (*itr)["identifier"].GetString();
            std::string level_path = (*itr)["externalRelPath"].GetString();
            SPDLOG_INFO("Found level {} in {}", level_name, level_path);
            m_level_map.insert({ level_name, level_path});
        }

        SPDLOG_INFO("File '{}' initialized.", file_name);
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
        if(!m_initialized)
        {
            SPDLOG_ERROR("The map should be initialized before loading a level!");
            return false;
        }
        if (m_level_map.find(level_name) == m_level_map.end())
        {
            SPDLOG_ERROR("Level '{}' does not exist!", level_name);
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
                SPDLOG_ERROR("Was unable to load '{}' from zip", level_file);
                return false;
            }
            rapidjson::IStreamWrapper isw(fp);
        document.ParseStream(isw);
        } else {
            fp.open(level_file, std::fstream::in);
            if(!fp)
            {
                SPDLOG_ERROR("Was unable to load '{}'", level_file);
                return false;
            }
            rapidjson::IStreamWrapper isw(fp);
            document.ParseStream(isw);
        }

        SPDLOG_INFO("Loading level '{}'", level_name);

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
                        SPDLOG_INFO("PlayerStart : ({}, {}), and angle is {}", m_player_start.x, m_player_start.y, m_player_heading);
                    }
                    if(identifier == "Static")
                    {
                        MapItem item;
                        item.map_x = (*itr)["__grid"].GetArray()[0].GetFloat() + 0.5f;
                        item.map_y = (*itr)["__grid"].GetArray()[1].GetFloat() + 0.5f;
                        item.type = (*itr)["fieldInstances"].GetArray()[0]["__value"].GetString();
                        m_map_statics.push_back(item);
                        SPDLOG_INFO("Added static item of type '{}' to the map at position ({},{})", item.type, item.map_x, item.map_y);
                    }
                }
            } // if layer type is Entities
            if(layer_type == "IntGrid")
            {
                m_map_width = (*itr)["__cWid"].GetInt();
                m_map_height = (*itr)["__cHei"].GetInt();

                std::string identifier = (*itr)["__identifier"].GetString();
                SPDLOG_INFO("Parsing IntGrid '{}' . . .", identifier);

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

        SPDLOG_INFO("Level '{}' loaded.", level_name.c_str());
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
