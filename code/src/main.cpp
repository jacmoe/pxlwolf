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
#include "utils.hpp"
#include "dbg_console.hpp"
#include "pixel_render.hpp"
#include <SDL.h>
#include <iostream>
#include "system.hpp"

int main(int, char**)
{
    std::string path = moena::utils::get_homedir().append("/source/repos/pxlwolf/");
    std::filesystem::current_path(path); 
    CreateConsoleWindow();

	const unsigned int WIDTH = 320 * 0.5;
	const unsigned int HEIGHT = 180 * 0.5;
	const unsigned int SCALE = 6;

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
	closeConsoleWindow();
	PixelRenderer::delPixBuffer(background);
    return 0;
}
