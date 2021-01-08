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
    char readBuffer[65536];
    rapidjson::Document document;
	FILE* fp = nullptr;
	bool level_found = false;

    if(from_zip)
	{
		PhysFS::ifstream fp(file_name);
		rapidjson::IStreamWrapper isw(fp);
	    document.ParseStream(isw);
	} else {
		fp = fopen(file_name.c_str(), "rb");
    	rapidjson::FileReadStream is(fp, readBuffer, sizeof(readBuffer));
	    document.ParseStream(is);
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
				}
				if(layer_type == "IntGrid")
				{
					map_width = (*itr)["__cWid"].GetInt();
					map_height = (*itr)["__cHei"].GetInt();

					// set the size of levelMap and fill it with zeroes.
					map_.assign(map_width * map_height, 0);

					const rapidjson::Value& initGrid = (*itr)["intGrid"];

					for (rapidjson::Value::ConstValueIterator itr = initGrid.Begin(); itr != initGrid.End(); ++itr)
					{
						// Add one to wall values so that zero becomes walkable area
						map_[(*itr)["coordId"].GetInt()] = (*itr)["v"].GetInt() + 1;
					}
				}

			}
		}
    }
	if(from_zip)
	{
		// I am probably leaving resources dangling, oh no . . . ;)
	} else {
		fclose(fp);
	}
    return level_found;
}

int Map::get_map_entry(int tile_x, int tile_y)
{
    int item = int(tile_y) * map_width + int(tile_x);
    return map_[item];
}

const std::vector<int>& Map::get_map()
{
    return map_;
}
