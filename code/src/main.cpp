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

#define GETR(c) (((c) >> 0) & 0xFF)
#define GETG(c) (((c) >> 8) & 0xFF)
#define GETB(c) (((c) >> 16) & 0xFF)

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
		fAccumulatedTime += fElapsedTime;
		if (fAccumulatedTime >= fTargetFrameTime)
		{
			fAccumulatedTime -= fTargetFrameTime;
			fElapsedTime = fTargetFrameTime;
			if (HandleInput(fElapsedTime))
			{
				Clear(olc::BLACK);
				draw_maze(GetDrawTarget());
			}
			else {
				return false;
			}
		}
		else {
			return true;
		}
		// we should never be here
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
							posX = player_pos.x;
							posY = player_pos.y;
							dirX = cos(player_heading);
							dirY = sin(player_heading);
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

	void draw_maze(olc::Sprite* buffer)
	{
		for (int x = 0; x < buffer->width; x++)
		{
			//calculate ray position and direction
			double cameraX = 2 * x / (double)buffer->width - 1; //x-coordinate in camera space
			double rayPosX = posX;
			double rayPosY = posY;
			double rayDirX = dirX + planeX * cameraX;
			double rayDirY = dirY + planeY * cameraX;

			//which box of the map we're in
			int mapX = (int)rayPosX;
			int mapY = (int)rayPosY;

			//length of ray from current position to next x or y-side
			double sideDistX;
			double sideDistY;

			//length of ray from one x or y-side to next x or y-side
			double deltaDistX = sqrt(1 + (rayDirY * rayDirY) / (rayDirX * rayDirX));
			double deltaDistY = sqrt(1 + (rayDirX * rayDirX) / (rayDirY * rayDirY));
			double perpWallDist;

			//what direction to step in x or y-direction (either +1 or -1)
			int stepX;
			int stepY;

			int hit = 0; //was there a wall hit?
			int side;    //was a NS or a EW wall hit?

			//calculate step and initial sideDist
			if (rayDirX < 0)
			{
				stepX = -1;
				sideDistX = (rayPosX - mapX) * deltaDistX;
			}
			else
			{
				stepX = 1;
				sideDistX = (mapX + 1.0 - rayPosX) * deltaDistX;
			}
			if (rayDirY < 0)
			{
				stepY = -1;
				sideDistY = (rayPosY - mapY) * deltaDistY;
			}
			else
			{
				stepY = 1;
				sideDistY = (mapY + 1.0 - rayPosY) * deltaDistY;
			}
			//perform DDA
			while (hit == 0)
			{
				//jump to next map square, OR in x-direction, OR in y-direction
				if (sideDistX < sideDistY)
				{
					sideDistX += deltaDistX;
					mapX += stepX;
					side = 0;
				}
				else
				{
					sideDistY += deltaDistY;
					mapY += stepY;
					side = 1;
				}
				//Check if ray has hit a wall
				if (get_map_entry(mapX, mapY) > -1)
					hit = 1;
			}

			//Calculate distance of perpendicular ray (oblique distance will give fisheye effect!)
			if (side == 0)
				perpWallDist = (mapX - rayPosX + (1 - stepX) / 2) / rayDirX;
			else
				perpWallDist = (mapY - rayPosY + (1 - stepY) / 2) / rayDirY;

			//Calculate height of line to draw on screen
			int lineHeight = (int)(buffer->height / perpWallDist);

			//calculate lowest and highest pixel to fill in current stripe
			int drawStart = -lineHeight / 2 + buffer->height / 2;
			if (drawStart < 0)
				drawStart = 0;
			int drawEnd = lineHeight / 2 + buffer->height / 2;
			if (drawEnd >= buffer->height)
				drawEnd = buffer->height - 1;
			//texturing calculations
			int texNum = get_map_entry(mapX, mapY) - 1; //1 subtracted from it so that texture 0 can be used!
			//FIXME Buffer* texture = get_texture(textures, texNum);

			//calculate value of wallX
			double wallX; //where exactly the wall was hit
			if (side == 0)
				wallX = rayPosY + perpWallDist * rayDirY;
			else
				wallX = rayPosX + perpWallDist * rayDirX;
			wallX -= floor((wallX));

			//x coordinate on the texture
			//FIXME int texX = (int)(wallX * (double)textures.width);
			int texX = 1;
			//FIXME if (side == 0 && rayDirX > 0)
			//FIXME 	texX = textures.width - texX - 1;
			//FIXME if (side == 1 && rayDirY < 0)
			//FIXME 	texX = textures.width - texX - 1;
			for (int y = drawStart; y < drawEnd; y++)
			{
				int d = y * 256 - buffer->height * 128 + lineHeight * 128; //256 and 128 factors to avoid floats
				//FIXME int texY = ((d * textures.height) / lineHeight) / 256;
				int texY = 1;
				uint32_t color = 0;//FIXME nasl_buffer_get_pixel(texture, texX, texY);
					
				double scalefactor = 1.0 - ((perpWallDist / 576) * 100);
				if(scalefactor > 1.0) scalefactor = 1.0;
				if(scalefactor < 0.01) scalefactor = 0.01;
				if(perpWallDist > 0.1)
					color = 0;//FIXME nasl_color_scale(color, scalefactor);
				
				//FIXME color = nasl_color_scale(color, 1.5);

				buffer->SetPixel(x, y, olc::RED);
				//FIXME nasl_buffer_set_pixel(buffer, x, y, color);
			}

			//FLOOR CASTING
			double floorXWall, floorYWall; //x, y position of the floor texel at the bottom of the wall

			//4 different wall directions possible
			if (side == 0 && rayDirX > 0)
			{
				floorXWall = mapX;
				floorYWall = mapY + wallX;
			}
			else if (side == 0 && rayDirX < 0)
			{
				floorXWall = mapX + 1.0;
				floorYWall = mapY + wallX;
			}
			else if (side == 1 && rayDirY > 0)
			{
				floorXWall = mapX + wallX;
				floorYWall = mapY;
			}
			else
			{
				floorXWall = mapX + wallX;
				floorYWall = mapY + 1.0;
			}

			double distWall, distPlayer, currentDist;

			distWall = perpWallDist;
			distPlayer = 0.0;

			if (drawEnd < 0)
				drawEnd = buffer->height; //becomes < 0 when the integer overflows

			//draw the floor from drawEnd to the bottom of the screen
			for (int y = drawEnd; y < buffer->height; y++)
			{
				currentDist = buffer->height / (2.0 * y - buffer->height); //you could make a small lookup table for this instead

				double weight = (currentDist - distPlayer) / (distWall - distPlayer);

				double currentFloorX = weight * floorXWall + (1.0 - weight) * posX;
				double currentFloorY = weight * floorYWall + (1.0 - weight) * posY;

				//FIXME int floorTexX, floorTexY;
				//FIXME floorTexX = (int)(currentFloorX * textures.width) % textures.width;
				//FIXME floorTexY = (int)(currentFloorY * textures.height) % textures.height;

				//floor
				//FIXME texture = get_texture(textures, 5);
				//FIXME uint32_t color = (texture->pixels[textures.width * floorTexY + floorTexX] >> 1) & 8355711;
				//FIXME color = nasl_color_scale(color, 0.3);
				buffer->SetPixel(x, y, olc::DARK_MAGENTA);
				//ceiling (symmetrical!)
				//FIXME texture = get_texture(textures, 10);
				//FIXME color = texture->pixels[textures.width * floorTexY + floorTexX];
				//FIXME color = nasl_color_scale(color, 0.3);
				buffer->SetPixel(x, buffer->height - y, olc::DARK_GREY);
			}
		}
	}
	bool HandleInput(float fElapsedTime)
	{
		double moveSpeed = fElapsedTime * 2.0; //the constant value is in squares/second
		double rotSpeed = fElapsedTime * 1.0; //the constant value is in radians/second

		if (GetKey(olc::Key::ESCAPE).bHeld) return false;

		if (GetKey(olc::Key::UP).bHeld)
		{
			if(get_map_entry(int(posX + dirX * moveSpeed), int(posY)) < 0) posX += dirX * moveSpeed;
			if(get_map_entry(int(posX), int(posY + dirY * moveSpeed)) < 0) posY += dirY * moveSpeed;
		}
		// or do we want to go backward?
		else if (GetKey(olc::Key::DOWN).bHeld)
		{
			if(get_map_entry(int(posX - dirX * moveSpeed), int(posY)) < 0) posX -= dirX * moveSpeed;
			if(get_map_entry(int(posX), int(posY - dirY * moveSpeed)) < 0) posY -= dirY * moveSpeed;
		}
		// Do we want to turn left?
		if (GetKey(olc::Key::LEFT).bHeld)
		{
            //both camera direction and camera plane must be rotated
            double oldDirX = dirX;
            dirX = dirX * cos(rotSpeed) - dirY * sin(rotSpeed);
            dirY = oldDirX * sin(rotSpeed) + dirY * cos(rotSpeed);
            double oldPlaneX = planeX;
            planeX = planeX * cos(rotSpeed) - planeY * sin(rotSpeed);
            planeY = oldPlaneX * sin(rotSpeed) + planeY * cos(rotSpeed);
		}
		// or do we want to turn right?
		else if (GetKey(olc::Key::RIGHT).bHeld)
		{
            //both camera direction and camera plane must be rotated
            double oldDirX = dirX;
            dirX = dirX * cos(-rotSpeed) - dirY * sin(-rotSpeed);
            dirY = oldDirX * sin(-rotSpeed) + dirY * cos(-rotSpeed);
            double oldPlaneX = planeX;
            planeX = planeX * cos(-rotSpeed) - planeY * sin(-rotSpeed);
            planeY = oldPlaneX * sin(-rotSpeed) + planeY * cos(-rotSpeed);
		}

		return true;
	}

	int get_map_entry(int tile_x, int tile_y)
	{
		int item = int(tile_y) * vWorldSize.x + int(tile_x);
		return lvlMap[item];
	}

	//Fast vertical line from (x,y1) to (x,y2), with rgb color
	void nasl_draw_vertical_line(olc::Sprite* buffer, int x, int y1, int y2, olc::Pixel color)
	{
		if(y2 < y1)
		{ // swap y1 and y2
			y1 += y2;
			y2 = y1 - y2;
			y1 -= y2;
		}
		if(y2 < 0 || y1 >= buffer->height || x < 0 || x >= buffer->width)
		{ // no single point of the line is on the screen
			return;
		}
		if(y1 < 0) y1 = 0; // clip
		if(y2 >= buffer->width) y2 = buffer->height - 1; // clip
		for(int y = y1; y <= y2; y++)
		{
			buffer->SetPixel(x, y, color);
		}
	}

	std::vector<int> lvlMap;
	olc::vf2d player_pos;
	float player_heading;
	olc::vi2d vWorldSize;
	float fPlayerMoveSpeed = 4.0f;

	double posX = 0, posY = 0;  //x and y start position
	double dirX = 0, dirY = 0; //initial direction vector
	double planeX = 0, planeY = 0.66; //the 2d raycaster version of camera plane

	int screenHeight = 240;
	int screenWidth = 320;
	int texWidth = 64;
	int texHeight = 64;

	float fTargetFrameTime = 1.0f / 30.0f; // Virtual FPS of 30fps
	float fAccumulatedTime = 0.0f;
};


#ifdef _WIN32
INT WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, INT nCmdShow)
#else
int main()
#endif
{
	PlxWolf wolf;
	if (wolf.Construct(320, 240, 2, 2))
		wolf.Start();

	return 0;
}
