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
#   BSD 2-Clause "Simplified" License
#*/
#include <iostream>
#include <filesystem>
#include "utils.hpp"
#include "dbg_console.hpp"
#include "pixel_render.hpp"
#include <SDL.h>
#include <iostream>

#include "sdl2.hpp"

int main(int, char**)
{
    std::string path = moena::utils::get_homedir().append("/source/repos/pxlwolf/");
    std::filesystem::current_path(path); 
    CreateConsoleWindow();

    using std::cerr;
    using std::endl;

    auto sys = sdl2::make_sdlsystem(SDL_INIT_EVERYTHING);
    if (!sys) {
        cerr << "Error creating SDL2 system: " << SDL_GetError() << endl;
        return 1;
    }

    auto win = sdl2::make_window("Hello World!", 100, 100, 320, 240, SDL_WINDOW_SHOWN);
    if (!win) {
        cerr << "Error creating window: " << SDL_GetError() << endl;
        return 1;
    }

    auto ren
        = sdl2::make_renderer(win.get(), -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!ren) {
        cerr << "Error creating renderer: " << SDL_GetError() << endl;
        return 1;
    }

	const unsigned int WIDTH = 320;
	const unsigned int HEIGHT = 240;
	SDL_Texture* drawTex = NULL;
	drawTex = SDL_CreateTexture(ren.get(), SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, WIDTH, HEIGHT);
	SDL_SetTextureBlendMode(drawTex, SDL_BLENDMODE_BLEND);

	SDL_Color nightSky = {20,0,20,255};
	SDL_Color nightHorizon = {50,20,50,255};
	SDL_Color eveningSky =  {0x5c,0x57,0xff,255};
	SDL_Color eveningHorizon = {0xff,0x40,0x00,255};
	SDL_Color white = {0xff,0xff,0xff,0xff};

	// Generate background texture
	uint32_t* texPixels = (uint32_t*)calloc(sizeof(uint32_t), WIDTH * HEIGHT);
	PixBuffer background;
	background.pixels = texPixels;
	background.width = WIDTH;
	background.height = HEIGHT;
	SDL_Color colorBottom1 = {50,50,80,255};
	PixBuffer_fillBuffer(&background, 0x00FFFF, 1.0);
	drawTex = SDL_CreateTexture(ren.get(), SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, WIDTH, HEIGHT);
	SDL_SetTextureBlendMode(drawTex, SDL_BLENDMODE_NONE);

    for (int i = 0; i < 20; i++) {
        SDL_RenderClear(ren.get());
		SDL_UpdateTexture(drawTex, NULL, background.pixels, sizeof(uint32_t) * WIDTH);
        SDL_RenderCopy(ren.get(), drawTex, nullptr, nullptr);
        SDL_RenderPresent(ren.get());
        SDL_Delay(100);
    }

    return 0;
}
