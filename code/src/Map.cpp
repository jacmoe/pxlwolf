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

Map::Map()
: map_width(0)
, map_height(0)
, player_x(0)
, player_y(0)
, player_heading(0.0f)
{}

Map::~Map()
{}

double Map::deg2rad (double degrees) {
    return degrees * 4.0 * atan (1.0) / 180.0;
}

bool Map::load(const std::string& file_name, const std::string& level_name, bool from_zip)
{
    rapidjson::Document document;
	std::fstream fp;
	bool level_found = false;

    if(from_zip)
	{
		PhysFS::ifstream fp(file_name);
		if(!fp)
		{
			SPDLOG_ERROR("Was unable to load '{}' from zip", file_name);
			return false;
		}
		rapidjson::IStreamWrapper isw(fp);
	    document.ParseStream(isw);
	} else {
		fp.open(file_name, std::fstream::in);
		if(!fp)
		{
			SPDLOG_ERROR("Was unable to load '{}'", file_name);
			return false;
		}
		rapidjson::IStreamWrapper isw(fp);
	    document.ParseStream(isw);
	}

    const rapidjson::Value& levels = document["levels"];
    for (rapidjson::Value::ConstValueIterator itr = levels.Begin(); itr != levels.End(); ++itr)
    {
        std::string level_name_ = (*itr)["identifier"].GetString();
		bool found = (std::find(level_names.begin(), level_names.end(), level_name_) != level_names.end());
		if(!found)
		{
			SPDLOG_INFO("Adding {} to the list of level names", level_name);
			level_names.push_back(level_name);
		}
		if(level_name_ == level_name)
		{
			level_found = true;
			SPDLOG_INFO("Loading level {}", level_name);
			const rapidjson::Value& layerInstances = (*itr)["layerInstances"];
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
							SPDLOG_INFO("Parsing {} . . .", identifier);
							player_x = (*itr)["__grid"].GetArray()[0].GetFloat() + 0.5f;
							player_y = (*itr)["__grid"].GetArray()[1].GetFloat() + 0.5f;
							player_heading = static_cast<float>(deg2rad((*itr)["fieldInstances"].GetArray()[0]["__value"].GetFloat()));
							SPDLOG_INFO("PlayerStart : ({},{}), and angle is {}", player_x, player_y, player_heading);
						}
					}
				} // if layer type is Entities
				if(layer_type == "IntGrid")
				{
					map_width = (*itr)["__cWid"].GetInt();
					map_height = (*itr)["__cHei"].GetInt();

					std::string identifier = (*itr)["__identifier"].GetString();
					SPDLOG_INFO("Parsing IntGrid {} . . .", identifier);

					// Set the size of walls, floor and ceiling vectors and fill it with zeroes.
					// For debugging purposes, only zero out if found
					if(identifier == "Walls")
					{
						walls.assign(map_width * map_height, 0);
					}
					if(identifier == "Floor")
					{
						floor.assign(map_width * map_height, 0);
					}
					if(identifier == "Ceiling")
					{
						ceiling.assign(map_width * map_height, 0);
					}

					const rapidjson::Value& initGrid = (*itr)["intGrid"];

					for (rapidjson::Value::ConstValueIterator itr = initGrid.Begin(); itr != initGrid.End(); ++itr)
					{
						if(identifier == "Walls")
						{
							// Add one to wall values so that zero becomes walkable area
							walls[(*itr)["coordId"].GetInt()] = (*itr)["v"].GetInt() + 1;
						}
						if(identifier == "Floor")
						{
							// Add one to floor values because LDtk starts from zero
							floor[(*itr)["coordId"].GetInt()] = (*itr)["v"].GetInt() + 1;
						}
						if(identifier == "Ceiling")
						{
							// Add one to ceiling values because LDtk starts from zero
							ceiling[(*itr)["coordId"].GetInt()] = (*itr)["v"].GetInt() + 1;
						}
					}
				} // if layer type is IntGrid

			} // for layer instances
		} // If level_name == ...
    } // all levels
	
	SPDLOG_INFO("Map loaded");
	SPDLOG_INFO("Walls is of size {}", walls.size());
	SPDLOG_INFO("Floor is of size {}", floor.size());
	SPDLOG_INFO("Ceiling is of size {}", ceiling.size());

	if(from_zip)
	{
		// I am probably leaving resources dangling, oh no . . . ;)
	} else {
		if(fp) fp.close();
	}
    return level_found;
}

int Map::get_wall_entry(int tile_x, int tile_y)
{
    int item = int(tile_y) * map_width + int(tile_x);
    return walls[item];
}

const std::vector<int>& Map::get_walls()
{
    return walls;
}

const std::vector<int>& Map::get_floor()
{
    return floor;
}

const std::vector<int>& Map::get_ceiling()
{
    return ceiling;
}

