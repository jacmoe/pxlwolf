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
#include "utility/Map.hpp"
#include "main/Level.hpp"

#include "rapidjson/filereadstream.h"
#include "rapidjson/document.h"
#include "rapidjson/istreamwrapper.h"
#include "spdlog/spdlog.h"

#include <algorithm>
#include <iostream>
#include <fstream>
#include <cmath>

#include <allegro5/allegro_color.h>

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

            fp.open(file_name, std::fstream::in);
            if(!fp)
            {
                SPDLOG_ERROR("Was unable to load '{}'", file_name.c_str());
                return false;
            }
            rapidjson::IStreamWrapper isw(fp);
            document.ParseStream(isw);

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
                    ALLEGRO_COLOR color = al_color_html(wall_color.c_str());
                    m_wall_elements.push_back({wall_identifier, color});
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

            fp.open(level_file, std::fstream::in);
            if(!fp)
            {
                SPDLOG_ERROR("Was unable to load '{}'", level_file);
                return false;
            }
            rapidjson::IStreamWrapper isw(fp);
            document.ParseStream(isw);

        SPDLOG_INFO("Loading level '{}'", level_name);

        m_map_width = document["pxWid"].GetInt();
        m_map_height = document["pxHei"].GetInt();

        Level level;

        const rapidjson::Value& layerInstances = document["layerInstances"];
        for (rapidjson::Value::ConstValueIterator itr = layerInstances.Begin(); itr != layerInstances.End(); ++itr)
        {
            std::string layer_type = (*itr)["__type"].GetString();
            if(layer_type == "Entities")
            {
                SPDLOG_INFO("Parsing entities . . .");
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
                        if((*itr)["fieldInstances"].GetArray()[0]["__value"].IsArray())
                        {
                            item.type = (*itr)["fieldInstances"].GetArray()[0]["__value"].GetArray()[0].GetString();
                        }
                        else
                        {
                            item.type = "unknown";
                        }
                        item.e_type = level.getEntityType(item.type);
                        m_map_statics.push_back(item);
                        // SPDLOG_INFO("Added static item of type '{}' to the map at position ({},{})", item.type, item.map_x, item.map_y);
                    }
                    if(identifier == "Pickup")
                    {
                        MapItem item;
                        item.map_x = (*itr)["__grid"].GetArray()[0].GetFloat() + 0.5f;
                        item.map_y = (*itr)["__grid"].GetArray()[1].GetFloat() + 0.5f;
                        if((*itr)["fieldInstances"].GetArray()[0]["__value"].IsArray())
                        {
                            item.type = (*itr)["fieldInstances"].GetArray()[0]["__value"].GetArray()[0].GetString();
                        }
                        else
                        {
                            item.type = "unknown";
                        }
                        item.e_type = level.getEntityType(item.type);
                        m_map_pickups.push_back(item);
                        // SPDLOG_INFO("Added item of type '{}' to the map at position ({},{})", item.type, item.map_x, item.map_y);
                    }
                    if(identifier == "Enemy")
                    {
                        MapItem item;
                        item.map_x = (*itr)["__grid"].GetArray()[0].GetFloat() + 0.5f;
                        item.map_y = (*itr)["__grid"].GetArray()[1].GetFloat() + 0.5f;
                        if((*itr)["fieldInstances"].GetArray()[0]["__value"].IsArray())
                        {
                            item.type = (*itr)["fieldInstances"].GetArray()[0]["__value"].GetArray()[0].GetString();
                        }
                        else
                        {
                            item.type = "unknown";
                        }
                        if((*itr)["fieldInstances"].GetArray()[1]["__value"].IsArray())
                        {
                            item.angle = (*itr)["fieldInstances"].GetArray()[1]["__value"].GetArray()[0].GetFloat();
                        }
                        else
                        {
                            item.angle = 0.0;
                        }
                        item.e_type = level.getEntityType(item.type);
                        m_map_enemies.push_back(item);
                        // SPDLOG_INFO("Added item of type '{}' to the map at position ({},{})", item.type, item.map_x, item.map_y);
                    }
                    if(identifier == "Key")
                    {
                        MapItem item;
                        item.map_x = (*itr)["__grid"].GetArray()[0].GetFloat() + 0.5f;
                        item.map_y = (*itr)["__grid"].GetArray()[1].GetFloat() + 0.5f;
                        if((*itr)["fieldInstances"].GetArray()[0]["__value"].IsArray())
                        {
                            item.type = (*itr)["fieldInstances"].GetArray()[0]["__value"].GetArray()[0].GetString();
                        }
                        else
                        {
                            item.type = "unknown";
                        }
                        item.e_type = level.getEntityType(item.type);
                        m_map_keys.push_back(item);
                        // SPDLOG_INFO("Added item of type '{}' to the map at position ({},{})", item.type, item.map_x, item.map_y);
                    }
                }
            } // if layer type is Entities
            if(layer_type == "IntGrid")
            {
                m_map_width = (*itr)["__cWid"].GetInt();
                m_map_height = (*itr)["__cHei"].GetInt();

                std::string identifier = (*itr)["__identifier"].GetString();
                SPDLOG_INFO("Parsing IntGrid '{}' . . .", identifier);

                for (auto const& int_grid : (*itr)["intGridCsv"].GetArray())
                {
                    if (identifier == "Walls")
                    {
                        m_walls.push_back(int_grid.GetInt());
                    }
                    if (identifier == "Floor")
                    {
                        m_floor.push_back(int_grid.GetInt());
                    }
                    if (identifier == "Ceiling")
                    {
                        m_ceiling.push_back(int_grid.GetInt());
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
