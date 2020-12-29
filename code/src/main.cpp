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

	const unsigned int WIDTH = 320 * 2;
	const unsigned int HEIGHT = 180 * 2;
	const unsigned int SCALE = 2;

    auto sys = sdl2::make_sdlsystem(SDL_INIT_EVERYTHING);
    if (!sys) {
        cerr << "Error creating SDL2 system: " << SDL_GetError() << endl;
        return 1;
    }

    auto win = sdl2::make_window("PixelWolf", 400, 100, WIDTH * SCALE, HEIGHT * SCALE, SDL_WINDOW_SHOWN);
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

	SDL_RenderSetLogicalSize(ren.get(), WIDTH, HEIGHT); // Actual size of canvas without the scaling factor.
	SDL_SetHint (SDL_HINT_RENDER_SCALE_QUALITY, "1"); // dont scale blurry
	SDL_Texture* drawTex = NULL;
	drawTex = SDL_CreateTexture(ren.get(), SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, WIDTH, HEIGHT);
	SDL_SetTextureBlendMode(drawTex, SDL_BLENDMODE_BLEND);

	// Generate background texture
	PixBuffer* background = PixelRenderer::initPixBuffer(WIDTH, HEIGHT);
	drawTex = SDL_CreateTexture(ren.get(), SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, WIDTH, HEIGHT);
    for (int i = 0; i < 20; i++) {
        SDL_RenderClear(ren.get());

		for (int x = 0; x < WIDTH; x++)
			for (int y = 0; y < HEIGHT; y++)
				PixelRenderer::drawPix(background, x, y, PixelRenderer::toPixColor(rand() % 255, rand() % 255, rand()% 255, rand() % 255));

		SDL_UpdateTexture(drawTex, NULL, background->pixels, sizeof(uint32_t) * WIDTH);
        SDL_RenderCopy(ren.get(), drawTex, nullptr, nullptr);
        SDL_RenderPresent(ren.get());
        SDL_Delay(200);
    }
	closeConsoleWindow();
	PixelRenderer::delPixBuffer(background);
    return 0;
}
