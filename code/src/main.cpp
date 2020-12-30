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
#   (c) 2020 Jacob Moena
#
#   MIT License
#*/
#include <iostream>
#include <filesystem>
#include "dbg_console.hpp"
#include "pixel_render.hpp"
#include <SDL.h>
#include <iostream>
#include <vector>
#include "system.hpp"
#include <filesystem>
#include "rapidjson/filereadstream.h"
#include "rapidjson/document.h"
#define DG_MISC_IMPLEMENTATION
#include "DG_misc.h"
#include "utils.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "raycaster_engine.hpp"
#include "game_engine.hpp"

#include "lua_main.hpp"

const unsigned int MAP_WIDTH = 16;
const unsigned int MAP_HEIGHT = 16;

unsigned char testMapChar[MAP_WIDTH*MAP_HEIGHT] = {
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
};

float player_x = 0.0;
float player_y = 0.0;
float player_heading = 0.0;

float deg2rad (float degrees) {
    return degrees * 4.0 * atan (1.0) / 180.0;
}

bool load_level(const std::string level_name)
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
                        player_x = (*itr)["__grid"].GetArray()[0].GetFloat() + 0.5f;
                        player_y = (*itr)["__grid"].GetArray()[1].GetFloat() + 0.5f;
                        player_heading = deg2rad((*itr)["fieldInstances"].GetArray()[0]["__value"].GetFloat());
                    }
                }
            }
            if(layer_type == "IntGrid")
            {
                const rapidjson::Value& initGrid = (*itr)["intGrid"];

                for (rapidjson::Value::ConstValueIterator itr = initGrid.Begin(); itr != initGrid.End(); ++itr)
                {
					testMapChar[(*itr)["coordId"].GetInt()] = (*itr)["v"].GetInt() + 1;
                }
            }

        }
    }
    fclose(fp);
    std::cout << "Parsed the following: " << std::endl;
    std::cout << "Player position : (" << player_x << "," << player_y << ")" << std::endl;
    std::cout << "Player heading : " << player_heading << std::endl;
    return true;
}

int get_map_entry(int tile_x, int tile_y)
{
    int item = int(tile_y) * MAP_WIDTH + int(tile_x);
    return testMapChar[item];
}

int main(int, char**)
{
    std::string path = DG_GetExecutableDir();
    std::vector<std::string> strList;
    strList.push_back("/build/code/");
    strList.push_back("\\build\\code\\");
    strList.push_back("Release");
    strList.push_back("Debug");
    utils::eraseSubStrings(path, strList);

    std::filesystem::current_path(path);

    CreateConsoleWindow();

	sol::state lua;
	// open some common libraries
	lua.open_libraries(sol::lib::base, sol::lib::package);
	lua.script("print('bark bark bark!')");
	std::cout << std::endl;

    load_level("assets/levels/level.ldtk");

	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);

    SDL_Renderer* renderer = nullptr;
    SDL_Texture* drawTex = nullptr;
    SDL_Window* window = nullptr;
    SDL_Event event;

	const uint8_t* keys = SDL_GetKeyboardState(nullptr);

    const unsigned int SCALE = 2;
    const unsigned int WIDTH = 320 * 2;//WIDDERSHINS/SCALE;
    const unsigned int HEIGHT = 180 * 2;//TURNWISE/SCALE;

	window = SDL_CreateWindow(
		"PixelWolf",
		300, 100,
		WIDTH*SCALE, HEIGHT*SCALE,
		SDL_WINDOW_OPENGL
	);

	SDL_Color colorKey[4] = {
		{255, 255, 255, 255},
		{220,50,50,255},	// Light red
		{100,255,70,255},	// Light green
		{70,70,255,255} 	// Light blue
	};

	//Demo map
	Map testMap;
	RaycasterEngine::generateMap(&testMap, testMapChar, MAP_WIDTH, MAP_HEIGHT, 2, colorKey, 4);

	// Demo texture
	int32_t mPixWidth;
	int32_t mPixHeight;
	uint8_t* mapPixDat = stbi_load("assets/textures/cobblebrick.png", &mPixWidth, &mPixHeight, NULL, 0);
	if (!mapPixDat)
	{
		fprintf(stderr, "FATAL: Could not load textures. Exiting...\n");
		return -1;
	}
	RayTex* worldTex = RayTex_initFromRGBA(mapPixDat, (uint32_t)mPixWidth, (uint32_t)mPixWidth, mPixHeight/mPixWidth);
	stbi_image_free(mapPixDat);

	// View depth
	double depth = 6;
	// Demo player
	Player testPlayer;
	GameEngine::initPlayer(&testPlayer, player_x, player_y, player_heading, 1, M_PI/2, depth, WIDTH);

	// Init keymapping
	KeyMap testKeys;
	uint8_t keyList[] = {
		PK_FORWARD, SDL_SCANCODE_W, 0,
		PK_BACKWARD, SDL_SCANCODE_S, 0,
		PK_LSTRAFE, SDL_SCANCODE_A, 0,
		PK_RSTRAFE, SDL_SCANCODE_D, 0,
		PK_TC, SDL_SCANCODE_Q, SDL_SCANCODE_RIGHT,
		PK_TCC, SDL_SCANCODE_E, SDL_SCANCODE_LEFT,
		PK_JUMP, SDL_SCANCODE_SPACE, 0,
		PK_CROUCH, SDL_SCANCODE_LCTRL, 0,
		PK_SPRINT, SDL_SCANCODE_LSHIFT, 0,
		PK_PAUSE, SDL_SCANCODE_DELETE, 0,
		PK_KILL, SDL_SCANCODE_K, 0,
		PK_RESPAWN, SDL_SCANCODE_R, 0,
		TERMINATE_PK
	};
	GameEngine::bindKeys(&testKeys, keyList);

	// SDL renderer initialization
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	SDL_RenderSetScale(renderer, SCALE, SCALE);
	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

	// Render target initialization
	drawTex = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, WIDTH, HEIGHT);
	SDL_SetTextureBlendMode(drawTex, SDL_BLENDMODE_BLEND);

	// Depth buffer initialization
	DepthBuffer* buffer = RaycasterEngine::initDepthBuffer(WIDTH, HEIGHT);
	SDL_Rect screenRect = {0,0,WIDTH,HEIGHT};

	// State variables
	uint8_t quit = 0;
	uint8_t paused = 0;
	uint8_t pauseKeyPressed = 0;
	uint8_t frameCounter = 0;
	uint32_t realRunTime = 0;
	double realRunTimeF = 0;
	double dt = 0;
	uint32_t runTime = SDL_GetTicks();
	double runTimeF = (double)runTime/1000;

	double toggleSaturation = 0;
	SDL_SetRelativeMouseMode(SDL_TRUE);
	while(!quit)
	{
		GameEngine::updateKeys(&testKeys);
		const uint8_t* keys = SDL_GetKeyboardState(NULL);
		// **Update Routine**
		realRunTime = SDL_GetTicks();		
		realRunTimeF = (double)realRunTime/1000;
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT || (event.type == SDL_KEYDOWN && event.key.keysym.scancode == SDL_SCANCODE_ESCAPE))
			{
				quit = 1;
			}
			else if (event.type == SDL_KEYDOWN && event.key.keysym.scancode == SDL_SCANCODE_DELETE && !pauseKeyPressed)
			{
				paused = !paused;
				pauseKeyPressed = 1;
				SDL_WarpMouseInWindow(window, 0, 0);
                bool mouse_mode = SDL_GetRelativeMouseMode();
				SDL_SetRelativeMouseMode(SDL_bool(!mouse_mode));
			}
			else if (event.type == SDL_KEYUP && event.key.keysym.scancode == SDL_SCANCODE_DELETE)
			{
				pauseKeyPressed = 0;
			}
		}
		// Update if not paused
		if (!paused)
		{
			GameEngine::updatePlayer(&testPlayer, &testMap, &testKeys, dt);
		}

		// **Render Routine**
		// Clear, draw line and update
		SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);
		SDL_RenderClear(renderer);
		SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);

		RaycasterEngine::resetDepthBuffer(buffer);
		RaycasterEngine::texRenderFloor(buffer->pixelBuffer, &testPlayer.camera, WIDTH, HEIGHT, NULL, 0, worldTex, 6);
		RaycasterEngine::texRenderCeiling(buffer->pixelBuffer, &(testPlayer.camera), WIDTH, HEIGHT, NULL, worldTex, 7);
		RaycasterEngine::raycastRender(buffer, &(testPlayer.camera), WIDTH, HEIGHT, &testMap, 0.01, worldTex);
		RaycasterEngine::drawMinimap(buffer->pixelBuffer, &(testPlayer.camera), WIDTH, HEIGHT, &testMap, 2);
        RaycasterEngine::renderBuffer(buffer);
		// Player death animation
		if (!testPlayer.state && testPlayer.timer < 2)
		{
			PixelRenderer::fillBuffer(buffer->pixelBuffer, PixelRenderer::toPixColor(150, 0, 20, 255), 1-(testPlayer.timer/2));
		}
		else if (testPlayer.state == 2)
		{
			GameEngine::initPlayer(&testPlayer, player_x, player_y, player_heading, 1, M_PI/2, depth, WIDTH);
		}
		SDL_Color sepiaPink = {221,153,153,255};
		if (paused)
		{
			PixelRenderer::monochromeFilter(buffer->pixelBuffer, sepiaPink, 1);
		}

		// Note: between 4 & 10 is good for 16 color palette
		PixelRenderer::orderDither256(buffer->pixelBuffer, 5);
		SDL_UpdateTexture(drawTex, NULL, buffer->pixelBuffer->pixels, sizeof(uint32_t) * WIDTH);
		SDL_RenderCopy(renderer, drawTex, NULL, NULL);
		SDL_RenderPresent(renderer);
		dt = 0.001 * (double)(SDL_GetTicks() - realRunTime);
		if (!paused)
		{
			runTimeF += dt;
			runTime += SDL_GetTicks() - realRunTime;
		}
	}

	// Clean up and quit
	RaycasterEngine::delDepthBuffer(buffer);
	RayTex_delRayTex(worldTex);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	renderer = nullptr;
	window = nullptr;
	SDL_Quit();

	closeConsoleWindow();

    return 0;
}
