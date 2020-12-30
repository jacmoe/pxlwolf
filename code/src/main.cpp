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

int test_1(int WIDTH, int HEIGHT, int SCALE)
{
    SimpleDirectLayer_System system;
    if(system.init("PixelWolf", WIDTH, HEIGHT, SCALE) == EXIT_FAILURE) return EXIT_FAILURE;
    SDL_Renderer* renderer = system.get_renderer();

	SDL_Texture* drawTex = NULL;
	drawTex = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, WIDTH, HEIGHT);
	SDL_SetTextureBlendMode(drawTex, SDL_BLENDMODE_BLEND);

	// Generate background texture
	PixBuffer* background = PixelRenderer::initPixBuffer(WIDTH, HEIGHT);
	drawTex = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, WIDTH, HEIGHT);
    for (int i = 0; i < 20; i++) {
        SDL_RenderClear(renderer);

		for (int x = 0; x < WIDTH; x++)
			for (int y = 0; y < HEIGHT; y++)
				PixelRenderer::drawPix(background, x, y, PixelRenderer::toPixColor(rand() % 255, rand() % 255, rand()% 255, rand() % 255));

		SDL_UpdateTexture(drawTex, NULL, background->pixels, sizeof(uint32_t) * WIDTH);
        SDL_RenderCopy(renderer, drawTex, nullptr, nullptr);
        SDL_RenderPresent(renderer);
        SDL_Delay(100);
    }
	PixelRenderer::delPixBuffer(background);
	return 0;
}

std::vector<int> lvlMap;
double posX = 22.0, posY = 11.5;    //x and y start position
double dirX = -1.0, dirY = 0.0;     //initial direction vector
double planeX = 0.0, planeY = 0.66; //the 2d raycaster version of camera plane

double curr_time = 0;    //time of current frame
double oldTime = 0; //time of previous frame
double frameTime = 0;

//Vector player_pos;
int player_pos_x;
int player_pos_y;
float player_heading;
//Vector world_size;
int world_size_x;
int world_size_y;

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
                        player_pos_x = (*itr)["__grid"].GetArray()[0].GetInt();
                        player_pos_y = (*itr)["__grid"].GetArray()[1].GetInt();
                        posX = player_pos_x;
                        posY = player_pos_y;
                        player_heading = (*itr)["fieldInstances"].GetArray()[0]["__value"].GetFloat();
                        //dirX = sin(deg2rad(player_heading));
                        //dirY = cos(deg2rad(player_heading));
                    }
                }
            }
            if(layer_type == "IntGrid")
            {
                world_size_x = (*itr)["__cWid"].GetInt();
                world_size_y = (*itr)["__cHei"].GetInt();
                for(int elems = 0; elems < world_size_x * world_size_y; elems++)
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
    std::cout << "Parsed the following: " << std::endl;
    std::cout << "World size : " << world_size_x << "x" << world_size_y << std::endl;
    std::cout << "Player position : (" << player_pos_x << "," << player_pos_y << ")" << std::endl;
    std::cout << "Player heading : " << player_heading << std::endl;
    return true;
}

int get_map_entry(int tile_x, int tile_y)
{
    int item = int(tile_y) * world_size_x + int(tile_x);
    return lvlMap[item];
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

	//const unsigned int WIDTH = 320 * 2;
	//const unsigned int HEIGHT = 180 * 2;
	//const unsigned int SCALE = 2;

    //load_level("assets/levels/level.ldtk");

	//int result = test_1(WIDTH, HEIGHT, SCALE);

	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);

    SDL_Renderer* renderer = nullptr;
    SDL_Texture* drawTex = nullptr;
    SDL_Window* window = nullptr;
    SDL_Event event;

	const uint8_t* keys = SDL_GetKeyboardState(nullptr);

    const unsigned int SCALE = 4;
    const unsigned int WIDTH = WIDDERSHINS/SCALE;
    const unsigned int HEIGHT = TURNWISE/SCALE;
    const unsigned int MAP_SCALE = 1;
    const unsigned int MAP_WIDTH = 10;
    const unsigned int MAP_HEIGHT = 13;

	unsigned char testMapChar[MAP_WIDTH*MAP_HEIGHT] = {
		3,1,1,1,3,4,5,5,5,4,
		1,0,0,0,1,5,0,0,0,5,
		1,0,2,0,1,5,0,0,0,5,
		1,0,0,0,0,0,0,0,0,5,
		3,1,1,1,3,4,5,0,5,4,
		6,6,6,6,6,0,3,0,4,0,
		6,0,0,0,6,0,3,0,4,0,
		6,0,0,0,6,0,3,0,4,0,
		0,0,0,0,6,3,3,0,3,3,
		6,0,0,0,6,3,0,0,0,3,
		0,0,0,0,0,0,0,0,0,3,
		6,0,0,0,6,3,0,0,0,3,
		6,6,0,6,6,3,3,3,3,3
	};

	window = SDL_CreateWindow(
		"Raycaster Thing",
		30, 30,
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
	double depth = 20;
	// Demo player
	Player testPlayer;
	GameEngine::initPlayer(&testPlayer, 1.5, 1.5, 0, 1, M_PI/2, depth, WIDTH);

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
	uint32_t pixels[WIDTH * HEIGHT];
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

		////PixBuffer_drawBuffOffset(&buffer, &background, WIDTH, HEIGHT, testPlayer.angle*scrollConst);
		RaycasterEngine::resetDepthBuffer(buffer);
		//memcpy(buffer->pixelBuffer->pixels, background.pixels, sizeof(uint32_t)*WIDTH*HEIGHT);
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
			GameEngine::initPlayer(&testPlayer, 1.5, 1.5, 0, 1, M_PI/2, depth, WIDTH);
		}
		//RaycasterEngine::draw2DSprite(buffer->pixelBuffer, cursorSprite, 2*runTimeF);
		////PixelRenderer::fillBuffer(&buffer, PixelRenderer::toPixColor(50, 50, 50, 255), 0.2);

		////PixelRenderer::fillBuffer(&buffer, PixelRenderer::toPixColor(150,0,20,255), 1);
		////PixelRenderer::monochromeFilter(&buffer, white, toggleSaturation);
		SDL_Color sepiaPink = {221,153,153,255};
		////PixelRenderer::monochromeFilter(&buffer, sepiaPink, 1);
		////PixelRenderer::monochromeFilter(&buffer, white, 4.442);
		if (paused)
		{
			SDL_Color monoGrey = {233,214,255,255};//{153, 140, 168, 255};
			PixelRenderer::monochromeFilter(buffer->pixelBuffer, sepiaPink, 1);
		}

		////PixBuffer_orderDither(&buffer, gameboyColorPalette, 4, 5);
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
	//free(background.pixels);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	renderer = nullptr;
	window = nullptr;
	SDL_Quit();

	closeConsoleWindow();

    return 0;
}
/*
#ifdef _WIN32
#include <windows.h>
#endif
#include <filesystem>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include <iostream>
#include "rapidjson/filereadstream.h"
#include "rapidjson/document.h"

#include "nasl_graphics.h"
#include "nasl_buffer.h"
#include "nasl_image.h"
#include "nasl_draw.h"
#include "nasl_sprite.h"
#include "nasl_geometry.h"
#include "utils.h"

int move_forwards = 0;
int move_backwards = 0;
int turn_left = 0;
int turn_right = 0;

int mouse_x = 0;
int mouse_y = 0;
int xdelta = 0;
int ydelta = 0;

static void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods);
void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);
void handle_keypress(int key, int action);

class PlxWolf
{
	std::vector<int> lvlMap;
    double posX = 22.0, posY = 11.5;    //x and y start position
    double dirX = -1.0, dirY = 0.0;     //initial direction vector
    double planeX = 0.0, planeY = 0.66; //the 2d raycaster version of camera plane

    double curr_time = 0;    //time of current frame
    double oldTime = 0; //time of previous frame
    double frameTime = 0;

    SpriteSheet textures;
    SpriteSheet ascii;
    Buffer *buffer = nullptr;
    Vector player_pos;
    float player_heading;
    Vector world_size;
public:
	PlxWolf()
	{
	}

    bool init(int width, int height)
    {
        std::string path = moena::utils::get_homedir().append("/source/repos/pxlwolf/");
        std::filesystem::current_path(path); 

        load_level("assets/levels/level.ldtk");

        nasl_graphics_init(width, height, "Neogardens Lightsourced Raycast Maze Demo", 0, 2);

        glfwSetKeyCallback(nasl_graphics_get_window(), key_callback);
        //glfwSetCursorPosCallback(nasl_graphics_get_window(), cursor_position_callback);
        glfwSetInputMode(nasl_graphics_get_window(), GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

        glfwSwapInterval(1);

        // Create main buffer
        buffer = nasl_buffer_create(width, height);
        nasl_buffer_set_mainbuffer(buffer);

        // Load textures
        textures = nasl_sprite_load("assets/textures/sjswalls2.bmp", 4, 3);

        // Load font
        ascii = nasl_sprite_load("assets/fonts/ascii.png", 16, 16);

        int win_width, win_height;
        glfwGetWindowSize(nasl_graphics_get_window(), &win_width, &win_height);
        framebuffer_size_callback(nasl_graphics_get_window(), win_width, win_height);

        return true;
    }

    bool shutdown()
    {
        nasl_sprite_delete(textures);
        nasl_sprite_delete(ascii);
        // Destroy the main buffer
        nasl_buffer_destroy(buffer);
        nasl_graphics_shutdown();
        return true;
    }

    bool main_loop()
    {
        // Main loop
        while (nasl_graphics_running())
        {
            oldTime = curr_time;
            curr_time = glfwGetTime();
            frameTime = curr_time - oldTime; //frameTime is the time this frame has taken, in seconds

            //speed modifiers
            double moveSpeed = frameTime * 3.0; //the constant value is in squares/second
            double rotSpeed = frameTime * 1.0;   //the constant value is in radians/second

            //
            double delta = xdelta * (frameTime * 0.1);
            if(xdelta < 0)
            {
                //both camera direction and camera plane must be rotated
                double oldDirX = dirX;
                dirX = dirX * cos(delta) - dirY * sin(delta);
                dirY = oldDirX * sin(delta) + dirY * cos(delta);
                double oldPlaneX = planeX;
                planeX = planeX * cos(delta) - planeY * sin(delta);
                planeY = oldPlaneX * sin(delta) + planeY * cos(delta);
                xdelta = 0.0;
            } else if(xdelta > 0) {
                //both camera direction and camera plane must be rotated
                double oldDirX = dirX;
                dirX = dirX * cos(delta) - dirY * sin(delta);
                dirY = oldDirX * sin(delta) + dirY * cos(delta);
                double oldPlaneX = planeX;
                planeX = planeX * cos(delta) - planeY * sin(delta);
                planeY = oldPlaneX * sin(delta) + planeY * cos(delta);
                xdelta = 0.0;
            }
            //
            if(move_forwards)
            {
                if (!get_map_entry((int)(posX + dirX * moveSpeed), (int)(posY)) > -1)
                    posX += dirX * moveSpeed;
                if (!get_map_entry((int)(posX), (int)(posY + dirY * moveSpeed)) > -1)
                    posY += dirY * moveSpeed;
            }
            if(move_backwards)
            {
                if (!get_map_entry((int)(posX - dirX * moveSpeed), (int)(posY)) > -1)
                    posX -= dirX * moveSpeed;
                if (!get_map_entry((int)(posX), (int)(posY - dirY * moveSpeed)) > -1)
                    posY -= dirY * moveSpeed;
            }
            if(turn_left)
            {
                //both camera direction and camera plane must be rotated
                double oldDirX = dirX;
                dirX = dirX * cos(rotSpeed) - dirY * sin(rotSpeed);
                dirY = oldDirX * sin(rotSpeed) + dirY * cos(rotSpeed);
                double oldPlaneX = planeX;
                planeX = planeX * cos(rotSpeed) - planeY * sin(rotSpeed);
                planeY = oldPlaneX * sin(rotSpeed) + planeY * cos(rotSpeed);
            }
            if(turn_right)
            {
                //both camera direction and camera plane must be rotated
                double oldDirX = dirX;
                dirX = dirX * cos(-rotSpeed) - dirY * sin(-rotSpeed);
                dirY = oldDirX * sin(-rotSpeed) + dirY * cos(-rotSpeed);
                double oldPlaneX = planeX;
                planeX = planeX * cos(-rotSpeed) - planeY * sin(-rotSpeed);
                planeY = oldPlaneX * sin(-rotSpeed) + planeY * cos(-rotSpeed);
            }

            // Event polling
            nasl_graphics_poll_events();

            // Clear buffer
            nasl_buffer_clear(buffer, GREY1);
            
            // Draw maze
            draw_maze(buffer, textures);
            
            // Print instructions
            nasl_draw_text(buffer, 20, buffer->height - 14, Color::GREY1, 10, "WASD to move. ESC to quit");

            // Render the main buffer
            nasl_graphics_render(buffer);
            
            // Swap buffers
            nasl_graphics_present();

            // Center mouse cursor
            //glfwSetCursorPos(nasl_graphics_get_window(), win_width / 2, win_height / 2);
        }
        return true;
    }

    void draw_maze(Buffer *buffer, SpriteSheet textures)
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
            int texNum = get_map_entry(mapX, mapY) + 1; //1 added to it so that texture 0 can be used!
            Buffer* texture = get_texture(textures, texNum);

            //calculate value of wallX
            double wallX; //where exactly the wall was hit
            if (side == 0)
                wallX = rayPosY + perpWallDist * rayDirY;
            else
                wallX = rayPosX + perpWallDist * rayDirX;
            wallX -= floor((wallX));

            //x coordinate on the texture
            int texX = (int)(wallX * (double)textures.width);
            if (side == 0 && rayDirX > 0)
                texX = textures.width - texX - 1;
            if (side == 1 && rayDirY < 0)
                texX = textures.width - texX - 1;
            for (int y = drawStart; y < drawEnd; y++)
            {
                int d = y * 256 - buffer->height * 128 + lineHeight * 128; //256 and 128 factors to avoid floats
                int texY = ((d * textures.height) / lineHeight) / 256;
                uint32_t color = nasl_buffer_get_pixel(texture, texX, texY);
                    
                double scalefactor = 1.0 - ((perpWallDist / 576) * 100);
                if(scalefactor > 1.0) scalefactor = 1.0;
                if(scalefactor < 0.01) scalefactor = 0.01;
                if(perpWallDist > 0.1)
                    color = nasl_color_scale(color, scalefactor);
                
                color = nasl_color_scale(color, 1.5);

                nasl_buffer_set_pixel(buffer, x, y, color);
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

                int floorTexX, floorTexY;
                floorTexX = (int)(currentFloorX * textures.width) % textures.width;
                floorTexY = (int)(currentFloorY * textures.height) % textures.height;

                //floor
                texture = get_texture(textures, 5);
                uint32_t color = (texture->pixels[textures.width * floorTexY + floorTexX] >> 1) & 8355711;
                color = nasl_color_scale(color, 0.3);
                nasl_buffer_set_pixel(buffer, x, y, color);
                //ceiling (symmetrical!)
                texture = get_texture(textures, 10);
                color = texture->pixels[textures.width * floorTexY + floorTexX];
                color = nasl_color_scale(color, 0.3);
                nasl_buffer_set_pixel(buffer, x, buffer->height - y, color);
            }
        }
    }

    Buffer *get_texture(SpriteSheet textures, int idx)
    {
        int col = idx % textures.cols;
        int row = idx / textures.cols;
        return nasl_sprite_get(textures, row, col);
    }

    bool load_level(const std::string level_name)
    {
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
                            posX = player_pos.x;
                            posY = player_pos.y;
                            player_heading = (*itr)["fieldInstances"].GetArray()[0]["__value"].GetFloat();
                            //dirX = sin(deg2rad(player_heading));
                            //dirY = cos(deg2rad(player_heading));
                        }
                    }
                }
                if(layer_type == "IntGrid")
                {
                    world_size.x = (*itr)["__cWid"].GetInt();
                    world_size.y = (*itr)["__cHei"].GetInt();
                    for(int elems = 0; elems < world_size.x * world_size.y; elems++)
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
        //std::cout << "Parsed the following: " << std::endl;
        //std::cout << "World size : " << world_size.x << "x" << world_size.y << std::endl;
        //std::cout << "Player position : (" << player_pos.x << "," << player_pos.y << ")" << std::endl;
        //std::cout << "Player heading : " << player_heading << std::endl;
        return true;
    }

	int get_map_entry(int tile_x, int tile_y)
	{
		int item = int(tile_y) * world_size.x + int(tile_x);
		return lvlMap[item];
	}

    double deg2rad (double degrees) {
        return degrees * 4.0 * atan (1.0) / 180.0;
    }
};

#ifdef _WIN32f
INT WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, INT nCmdShow)
#else
int main()
#endif
{
    PlxWolf wolf;
    wolf.init(640, 480);
    wolf.main_loop();
    wolf.shutdown();
}

void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
    int mouse_x = 0;
    int mouse_y = 0;
    int xdelta = 0;
    int ydelta = 0;

    xdelta = mouse_x - xpos;
    ydelta = mouse_y - ypos;

    mouse_x = xpos;
    mouse_y = ypos;
}

void key_callback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
    else
    {
        handle_keypress(key, action);
    }
}

void handle_keypress(int key, int action)
{
    // Do we want to move forward?
    if (key == GLFW_KEY_W)
    {
        if(action == GLFW_PRESS)
            move_forwards = 1;
        else if(action == GLFW_RELEASE)
            move_forwards = 0;
    }
    // or do we want to go backward?
    else if (key == GLFW_KEY_S)
    {
        if(action == GLFW_PRESS)
            move_backwards = 1;
        else if(action == GLFW_RELEASE)
            move_backwards = 0;
    }
    // Do we want to turn left?
    if (key == GLFW_KEY_A)
    {
        if(action == GLFW_PRESS)
            turn_left = 1;
        else if(action == GLFW_RELEASE)
            turn_left = 0;
    }
    // or do we want to turn right?
    else if (key == GLFW_KEY_D)
    {
        if(action == GLFW_PRESS)
            turn_right = 1;
        else if(action == GLFW_RELEASE)
            turn_right = 0;
    }
}
*/