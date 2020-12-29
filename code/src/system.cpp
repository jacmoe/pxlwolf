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
#include "system.hpp"

    SimpleDirectLayer_System::~SimpleDirectLayer_System()
    {
        if(renderer != nullptr)
        {
            SDL_DestroyRenderer(renderer);
        }
        if(window != nullptr)
        {
            SDL_DestroyWindow(window);

        }
        SDL_Quit();
        std::cout << "I quit!" << std::endl;
    }

uint32_t SimpleDirectLayer_System::init(const char* title, const int width, const int height, const int scale)
{
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        std::cerr << "SDL_Init Error: " << SDL_GetError() << std::endl;
        return EXIT_FAILURE;
    }

    window = SDL_CreateWindow(title, 400, 200, width * scale, height * scale, SDL_WINDOW_SHOWN);
    if (window == nullptr) {
        std::cerr << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
        return EXIT_FAILURE;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (renderer == nullptr) {
        std::cerr << "SDL_CreateRenderer Error" << SDL_GetError() << std::endl;
		if (window != nullptr) {
			SDL_DestroyWindow(window);
		}
		SDL_Quit();
        return EXIT_FAILURE;
    }

    SDL_RenderSetLogicalSize(renderer, WIDTH, HEIGHT); // Actual size of canvas without the scaling factor.
	SDL_SetHint (SDL_HINT_RENDER_SCALE_QUALITY, "1"); // dont scale blurry

    return 0;
}
