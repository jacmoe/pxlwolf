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

#include "rapidjson/filereadstream.h"
#include "rapidjson/document.h"
#include <cstdio>

#include "utils.h"

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
		std::string path = moena::utils::get_homedir().append("/source/repos/pxlwolf/");
		std::filesystem::current_path(path); 

		LoadLevel("assets/levels/level.ldtk");

		Clear(olc::DARK_GREY);

		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{
		return true;
	}
private:
	void LoadLevel(const std::string level_name)
	{
		FILE* fp = fopen(level_name.c_str(), "rb");
 
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
					vWorldSize.x = (*itr)["__cWid"].GetInt();
					vWorldSize.y = (*itr)["__cHei"].GetInt();
					for(int elems = 0; elems < vWorldSize.x * vWorldSize.y; elems++)
					{
						lvlMap.push_back(-1);
					}

					const rapidjson::Value& initGrid = (*itr)["intGrid"];

					for (rapidjson::Value::ConstValueIterator itr = initGrid.Begin(); itr != initGrid.End(); ++itr)
					{
						lvlMap[(*itr)["coordId"].GetInt()] = (*itr)["v"].GetInt();
					}
				}

			}
		}
		fclose(fp);
	}

	std::vector<int> lvlMap;
	olc::vf2d player_pos;
	float player_heading;
	olc::vi2d vWorldSize;
	float fPlayerMoveSpeed = 4.0f;
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
