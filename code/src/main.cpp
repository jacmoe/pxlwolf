/*# This file is part of the
  #   ___ _       __    __      _  __ 
  #  / _ \ |_  __/ / /\ \ \___ | |/ _|
  # / /_)/ \ \/ /\ \/  \/ / _ \| | |_ 
  #/ ___/| |>  <  \  /\  / (_) | |  _|
  #\/    |_/_/\_\  \/  \/ \___/|_|_|  
  # project
  # https://github.com/jacmoe/pxlwolf
  #
  # (c) 2020 Jacob Moena
  #
  # BSD 2-Clause "Simplified" License
  #
*/
#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"
#define OLC_PGEX_RAYCASTWORLD
#include "olcPGEX_RayCastWorld.h"
#include "rapidjson/filereadstream.h"
#include "rapidjson/document.h"
#include <cstdio>
#include "utils.h"

class PlxWolfGame : public olc::rcw::Engine
{
public:
	PlxWolfGame(const int screen_w, const int screen_h, const float fov)
		: olc::rcw::Engine(screen_w, screen_h, fov)
	{ 
	}
	
protected:
	// User implementation to retrieve appropriate graphics for scenery
	olc::Pixel SelectSceneryPixel(const int tile_x, const int tile_y, const olc::rcw::Engine::CellSide side, const float sample_x, const float sample_y, const float distance) override
	{
		olc::Pixel p;

		// Choose appropriate colour
		switch (side)
		{
			case olc::rcw::Engine::CellSide::Top: // Location is "Sky"
				p = olc::CYAN; 
				break;

			case olc::rcw::Engine::CellSide::Bottom: // Location is "Ground"
				p =  olc::DARK_GREEN;
				break;

			default: // Location is "Wall"
				p = olc::WHITE; 
				if (sample_x < 0.05f || sample_x > 0.95f || sample_y < 0.05f || sample_y > 0.95f)
					p = olc::BLACK;
				break;			
		}
	
		// Apply directional lighting, by first creating a shadow scalar...
		float fShadow = 1.0f;
		switch (side)
		{
			case olc::rcw::Engine::CellSide::South: fShadow = 0.3f; break;
			case olc::rcw::Engine::CellSide::East:  fShadow = 0.3f; break;
		}

		// ...also shade by distance...
		float fDistance = 1.0f - std::min(distance / 32.0f, 1.0f);

		// ...and applying it to sampled pixel
		p.r = uint8_t(float(p.r) * fDistance);
		p.g = uint8_t(float(p.g) * fDistance);
		p.b = uint8_t(float(p.b) * fDistance);

		return p;
	}

	// User implementation to retrieve if a particular tile is solid
	bool IsLocationSolid(const float tile_x, const float tile_y) override
	{
		if (int(tile_x) >= 0 && int(tile_x) < vWorldSize.x && int(tile_y) >= 0 && int(tile_y) < vWorldSize.y)
			return false;
			//return sMap[int(tile_y) * vWorldSize.x + int(tile_x)] == '#';
		else
			return true;
	}


	// NOTE! Objects are not used in this demonstration ===================

	// User implementation to retrieve dimensions of an in game object
	float GetObjectWidth(const uint32_t id) override
	{ return 1; }

	float GetObjectHeight(const uint32_t id) override
	{ return 1; }

	// User implementation to retrieve appropriate graphics for objects
	olc::Pixel SelectObjectPixel(const uint32_t id, const float sample_x, const float sample_y, const float distance, const float angle) override
	{ return olc::BLACK; }

private:
	std::vector<int> wMap;
	std::vector<int> tMap;
	olc::vf2d player_pos;
	float player_heading;
	olc::vi2d vWorldSize = { 16, 16 };
};


class PlxWolf : public olc::PixelGameEngine
{
public:
	PlxWolf()
	{
		sAppName = "PlxWolf";
	}

public:
	bool OnUserCreate() override
	{
		std::vector<int> wMap;
		std::vector<int> tMap;
		olc::vf2d player_pos;
		float player_heading;
		olc::vi2d world_size;

		std::string path = moena::utils::get_homedir().append("/source/repos/pxlwolf/");
		std::filesystem::current_path(path); 

		FILE* fp = fopen("assets/levels/level.ldtk", "rb");
 
		char readBuffer[65536];
		rapidjson::FileReadStream is(fp, readBuffer, sizeof(readBuffer));
		rapidjson::Document document;
		document.ParseStream(is);

		const rapidjson::Value& levels = document["levels"];
        for (rapidjson::Value::ConstValueIterator itr = levels.Begin(); itr != levels.End(); ++itr)
		{
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
							player_pos.x = (*itr)["__grid"].GetArray()[0].GetFloat();
							player_pos.y = (*itr)["__grid"].GetArray()[1].GetFloat();
							player_heading = (*itr)["fieldInstances"].GetArray()[0]["__value"].GetFloat();
						}
					}
				}
				if(layer_type == "IntGrid")
				{
					world_size.x = (*itr)["__cWid"].GetInt();
					world_size.y = (*itr)["__cHei"].GetInt();
					for(int elems = 0; elems < world_size.x * world_size.y; elems++)
					{
						wMap.push_back(-1);
					}

					const rapidjson::Value& initGrid = (*itr)["intGrid"];

					for (rapidjson::Value::ConstValueIterator itr = initGrid.Begin(); itr != initGrid.End(); ++itr)
					{
						wMap[(*itr)["coordId"].GetInt()] = (*itr)["v"].GetInt();
					}
				}

			}
		}
		fclose(fp);
		//std::cout << "Parsed the following: " << std::endl;
		//std::cout << "World size : " << world_size.x << "x" << world_size.y << std::endl;
		//std::cout << "Player position : (" << player_pos.x << "," << player_pos.y << ")" << std::endl;
		//std::cout << "Player heading : " << player_heading << std::endl;

		Clear(olc::DARK_GREY);

		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{
		return true;
	}
};


#ifdef _WIN325
INT WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, INT nCmdShow)
#else
int main()
#endif
{
	PlxWolf wolf;
	if (wolf.Construct(640, 480, 2, 2))
		wolf.Start();

	return 0;
}
