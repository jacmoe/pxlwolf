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
#include <SDL_ttf.h>
#include <iostream>
#include <vector>
#include "system.hpp"
#include <filesystem>
#include "rapidjson/filereadstream.h"
#include "rapidjson/document.h"
#include "rapidjson/istreamwrapper.h"
#define DG_MISC_IMPLEMENTATION
#include "DG_misc.h"
#include "utils.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "raycaster_engine.hpp"
#include "game_engine.hpp"

#include "lua_main.hpp"
#include "physfs.hpp"

// Completely hardcoded. I love it! :)
const unsigned int MAP_WIDTH = 16;
const unsigned int MAP_HEIGHT = 16;

unsigned char testMapChar[MAP_WIDTH*MAP_HEIGHT] = {
	0, 0, 0, 0 ,0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0 ,0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0 ,0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0 ,0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0 ,0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0 ,0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0 ,0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0 ,0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0 ,0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0 ,0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0 ,0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0 ,0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0 ,0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0 ,0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0 ,0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
	0, 0, 0, 0 ,0 ,0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

float player_x = 0.0;
float player_y = 0.0;
float player_heading = 0.0;

double deg2rad (double degrees) {
    return degrees * 4.0 * atan (1.0) / 180.0;
}

bool load_level(const std::string level_name, bool from_zip = false)
{
    char readBuffer[65536];
    rapidjson::Document document;
	FILE* fp = nullptr;

    if(from_zip)
	{
		PhysFS::ifstream fp(level_name);
		rapidjson::IStreamWrapper isw(fp);
	    document.ParseStream(isw);
	} else {
		fp = fopen(level_name.c_str(), "rb");
    	rapidjson::FileReadStream is(fp, readBuffer, sizeof(readBuffer));
	    document.ParseStream(is);
	}

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
                        player_heading = static_cast<float>(deg2rad((*itr)["fieldInstances"].GetArray()[0]["__value"].GetFloat()));
                    }
                }
            }
            if(layer_type == "IntGrid")
            {
                const rapidjson::Value& initGrid = (*itr)["intGrid"];

                for (rapidjson::Value::ConstValueIterator itr = initGrid.Begin(); itr != initGrid.End(); ++itr)
                {
					// Add one to wall values so that zero becomes walkable area
					testMapChar[(*itr)["coordId"].GetInt()] = (*itr)["v"].GetInt() + 1;
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
    return true;
}

int get_map_entry(int tile_x, int tile_y)
{
    int item = int(tile_y) * MAP_WIDTH + int(tile_x);
    return testMapChar[item];
}

void RandomStuff()
{
    printf("RandomStuff()\n");
}

int GetRandomNumber(int value)
{
    return value;
}

struct MyStruct
{
    float x, y;

    MyStruct() : x(0), y(0)
    {
    }

    MyStruct(float x, float y) : x(x), y(y)
    {
    }

    void Print() const
    {
        printf("%f, %f\n", x, y);
    }

    float GetX() const
    {
        return x;
    }

    float GetY() const
    {
        return y;
    }

    void SetX(const float& x)
    {
        this->x = x;
    }

    void SetY(const float& y)
    {
        this->y = y;
    }
};

void DoStuffToStruct(MyStruct& myStruct)
{
    myStruct.x = 200;
    myStruct.y = 200;
}

void test_lua()
{
    sol::state lua{};
    lua.open_libraries(sol::lib::base);

    // 1. Run Lua scripts
    lua.script("print('Hello World from Lua!')");
    
    // 3. Bind and call C++ functions to Lua
    lua.set_function("RandomStuff", &RandomStuff);
    lua.set_function("GetRandomNumber", &GetRandomNumber);
    lua.set_function("DoStuffToStruct", &DoStuffToStruct);

    
    // 4. Bind and use C++ data to Lua
    lua.new_usertype<MyStruct>("MyStruct",
        sol::constructors<void(), void(float, float)>(),
        "x", &MyStruct::x,
        "y", &MyStruct::y,
        "Print", &MyStruct::Print,
        "GetX", &MyStruct::GetX,
        "GetY", &MyStruct::GetY,
        "SetX", &MyStruct::SetX,
        "SetY", &MyStruct::SetY
    );
        
    // 2. Load Lua scripts from disk
    lua.script_file("assets/scripts/init.lua");
    
    // 5. Call Lua functions from C++ (With return types)
    const std::function<void()>& luaFunction = lua["LuaFunction"];
    luaFunction();

    const std::function<int(int)>& luaFunction2 = lua["LuaFunction2"];
    auto value = luaFunction2(123);
    printf("value = %d \n", value);

    MyStruct myStruct;
    const std::function<void(MyStruct&)>& luaFunction3 = lua["LuaFunction3"];
    luaFunction3(myStruct);
    myStruct.Print();
}

// From https://www.willusher.io/sdl2%20tutorials/2013/12/18/lesson-6-true-type-fonts-with-sdl_ttf
SDL_Texture* renderText(const std::string &message, const std::string &fontFile,
	SDL_Color color, int fontSize, SDL_Renderer *renderer)
{
	//Open the font
	TTF_Font *font = TTF_OpenFont(fontFile.c_str(), fontSize);
	if (font == nullptr){
		//logSDLError(std::cout, "TTF_OpenFont");
		return nullptr;
	}	
	//We need to first render to a surface as that's what TTF_RenderText
	//returns, then load that surface into a texture
	SDL_Surface *surf = TTF_RenderText_Blended(font, message.c_str(), color);
	if (surf == nullptr){
		TTF_CloseFont(font);
		//logSDLError(std::cout, "TTF_RenderText");
		return nullptr;
	}
	SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surf);
	if (texture == nullptr){
		//logSDLError(std::cout, "CreateTexture");
	}
	//Clean up the surface and font
	SDL_FreeSurface(surf);
	TTF_CloseFont(font);
	return texture;
}

/*
 * Draw an SDL_Texture to an SDL_Renderer at some destination rect
 * taking a clip of the texture if desired
 * @param tex The source texture we want to draw
 * @param rend The renderer we want to draw too
 * @param dst The destination rectangle to render the texture too
 * @param clip The sub-section of the texture to draw (clipping rect)
 *		default of nullptr draws the entire texture
 */
void renderTexture(SDL_Texture *tex, SDL_Renderer *ren, SDL_Rect dst, SDL_Rect *clip = nullptr){
	SDL_RenderCopy(ren, tex, clip, &dst);
}
/*
 * Draw an SDL_Texture to an SDL_Renderer at position x, y, preserving
 * the texture's width and height and taking a clip of the texture if desired
 * If a clip is passed, the clip's width and height will be used instead of the texture's
 * @param tex The source texture we want to draw
 * @param rend The renderer we want to draw too
 * @param x The x coordinate to draw too
 * @param y The y coordinate to draw too
 * @param clip The sub-section of the texture to draw (clipping rect)
 *		default of nullptr draws the entire texture
 */
void renderTexture(SDL_Texture *tex, SDL_Renderer *ren, int x, int y, SDL_Rect *clip = nullptr){
	SDL_Rect dst;
	dst.x = x;
	dst.y = y;
	if (clip != nullptr){
		dst.w = clip->w;
		dst.h = clip->h;
	}
	else {
		SDL_QueryTexture(tex, NULL, NULL, &dst.w, &dst.h);
	}
	renderTexture(tex, ren, dst, clip);
}

/*
 * Log an SDL error with some error message to the output stream of our choice
 * @param os The output stream to write the message too
 * @param msg The error message to write, format will be msg error: SDL_GetError()
 */
void logSDLError(std::ostream &os, const std::string &msg){
	os << msg << " error: " << SDL_GetError() << std::endl;
}

SDL_RWops *SDL_RWFromIStream( std::istream& stream )
{
	std::streamsize size = stream.tellg();
	stream.seekg(0, std::ios::beg);

	std::vector<char> buffer(size);
	stream.read(buffer.data(), size);

	// Get an SDL_RWops struct for the file
	SDL_RWops* rwops = SDL_RWFromMem(&buffer[0], buffer.size());

    return rwops;
}

int main(int, char**)
{
    // Get executable pah
	std::string path = DG_GetExecutableDir();
    // Remove the build directory, so that we land on appropriate direcotory for asset loading
	std::vector<std::string> strList;
    strList.push_back("/build/code/");
    strList.push_back("\\build\\code\\");
    strList.push_back("Release");
    strList.push_back("Debug");
    utils::eraseSubStrings(path, strList);

    // Set a proper working directory
	std::filesystem::current_path(path);

    CreateConsoleWindow();

	test_lua();

	PhysFS::init (nullptr);
	PhysFS::mount("assets.zip", "", 1);

    load_level("assets/levels/level.ldtk", true);

	SDL_Init(SDL_INIT_VIDEO);

	if (TTF_Init() != 0){
		logSDLError(std::cout, "TTF_Init");
		SDL_Quit();
		return 1;
	}

    SDL_Renderer* renderer = nullptr;
    SDL_Texture* drawTex = nullptr;
    SDL_Window* window = nullptr;
    SDL_Event event;

	const uint8_t* keys = SDL_GetKeyboardState(nullptr);

    const unsigned int SCALE = 2;
    const unsigned int WIDTH = 320 * 2;// 640 WIDDERSHINS/SCALE;
    const unsigned int HEIGHT = 180 * 2;//360 TURNWISE/SCALE;

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
	uint8_t* mapPixDat = stbi_load("assets/textures/walls.png", &mPixWidth, &mPixHeight, NULL, 0);
	if (!mapPixDat)
	{
		fprintf(stderr, "FATAL: Could not load textures. Exiting...\n");
		return -1;
	}
	RayTex* worldTex = RayTex_initFromRGBA(mapPixDat, (uint32_t)mPixWidth, (uint32_t)mPixWidth, mPixHeight/mPixWidth);
	stbi_image_free(mapPixDat);

	// View depth
	double view_depth = 6;
	// Demo player
	Player testPlayer;
	GameEngine::initPlayer(&testPlayer, player_x, player_y, player_heading, 1, M_PI/2, view_depth, WIDTH);

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

	SDL_Color text_color = { 255, 255, 255, 255 };
	SDL_Texture* text_image = renderText("PixelWolf", "assets/fonts/MedievalSharp-Bold.ttf",
		text_color, 16, renderer);
	if (text_image == nullptr){
		//cleanup(renderer, window);
		TTF_Quit();
		SDL_Quit();
		return 1;
	}

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

		SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xFF);
		SDL_RenderClear(renderer);
		SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);

		testPlayer.camera.x = testPlayer.x;
		testPlayer.camera.y = testPlayer.y;
		RaycasterEngine::resetDepthBuffer(buffer);
		RaycasterEngine::texRenderFloor(buffer->pixelBuffer, &testPlayer.camera, WIDTH, HEIGHT, NULL, 0, worldTex, 6);
		RaycasterEngine::texRenderCeiling(buffer->pixelBuffer, &(testPlayer.camera), WIDTH, HEIGHT, NULL, worldTex, 7);
		RaycasterEngine::raycastRender(buffer, &(testPlayer.camera), WIDTH, HEIGHT, &testMap, 0.01, worldTex);
		RaycasterEngine::drawMinimap(buffer->pixelBuffer, &(testPlayer.camera), WIDTH, HEIGHT, &testMap, 4);
        RaycasterEngine::renderBuffer(buffer);
		// Player death animation
		if (!testPlayer.state && testPlayer.timer < 2)
		{
			PixelRenderer::fillBuffer(buffer->pixelBuffer, PixelRenderer::toPixColor(150, 0, 20, 255), 1-(testPlayer.timer/2));
		}
		else if (testPlayer.state == 2)
		{
			GameEngine::initPlayer(&testPlayer, player_x, player_y, player_heading, 1, M_PI/2, view_depth, WIDTH);
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
		renderTexture(text_image, renderer, 10, 10);
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
	SDL_DestroyTexture(text_image);
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	renderer = nullptr;
	window = nullptr;
	TTF_Quit();
	SDL_Quit();
	PhysFS::deinit();

	closeConsoleWindow();

    return 0;
}
