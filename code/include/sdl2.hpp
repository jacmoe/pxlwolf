#pragma once

#include <SDL.h>
#include <SDL_image.h>
#include <memory>

namespace sdl2 {

// Very useful function from Eric Scott Barr:
// https://eb2.co/blog/2014/04/c-plus-plus-14-and-sdl2-managing-resources/
template <typename Creator, typename Destructor, typename... Arguments>
auto make_resource(Creator c, Destructor d, Arguments&&... args)
{
    using std::decay_t;
    using std::forward;
    using std::unique_ptr;

    auto r = c(forward<Arguments>(args)...);
    return unique_ptr<decay_t<decltype(*r)>, decltype(d)>(r, d);
}

// The "internal type" of the SDL System
using SDL_System = int;

// The "internal type" of the SDL_image System
using SDL_Image_System = int;


// SDL_CreateSDL initiates the use of SDL.
// The given flags are passed to SDL_Init.
// The returned value contains the exit code.
inline SDL_System* SDL_CreateSDL(Uint32 flags)
{
    auto init_status = new SDL_System;
    *init_status = SDL_Init(flags);
    return init_status;
}

// SDL_DestroySDL ends the use of SDL
inline void SDL_DestroySDL(SDL_System* init_status)
{
    delete init_status; // Delete the int that contains the return value from SDL_Init
    SDL_Quit();
}

// SDL_CreateSDLImage initiates the use of SDL_image.
// The given flags are passed to IMG_Init.
// The returned value contains the exit code.
inline SDL_Image_System* SDL_CreateSDLImage(Uint32 flags)
{
    auto img_init_status = new SDL_Image_System;
    *img_init_status = IMG_Init(flags);
    return img_init_status;
}

// SDL_DestroySDLImage ends the use of SDL_image
inline void SDL_DestroySDLImage(SDL_Image_System* img_init_status)
{
    delete img_init_status; // Delete the int that contains the return value from IMG_Init
    IMG_Quit();
}

using sdlsystem_ptr_t = std::unique_ptr<SDL_System, decltype(&SDL_DestroySDL)>;
using sdlimage_ptr_t = std::unique_ptr<SDL_Image_System, decltype(&SDL_DestroySDLImage)>;
using window_ptr_t = std::unique_ptr<SDL_Window, decltype(&SDL_DestroyWindow)>;
using renderer_ptr_t = std::unique_ptr<SDL_Renderer, decltype(&SDL_DestroyRenderer)>;
using surf_ptr_t = std::unique_ptr<SDL_Surface, decltype(&SDL_FreeSurface)>;
using texture_ptr_t = std::unique_ptr<SDL_Texture, decltype(&SDL_DestroyTexture)>;

// Initialize SDL (the returned int* contains the return value from SDL_Init)
inline sdlsystem_ptr_t make_sdlsystem(Uint32 flags)
{
    return make_resource(SDL_CreateSDL, SDL_DestroySDL, flags);
}

// Initialize SDL_image (the returned int* contains the return value from IMG_Init)
inline sdlimage_ptr_t make_sdlimage(Uint32 flags)
{
    return make_resource(SDL_CreateSDLImage, SDL_DestroySDLImage, flags);
}

// Create a window (that contains both a SDL_Window and the destructor for SDL_Windows)
inline window_ptr_t make_window(const char* title, int x, int y, int w, int h, Uint32 flags)
{
    return make_resource(SDL_CreateWindow, SDL_DestroyWindow, title, x, y, w, h, flags);
}

// Create a renderer given a window, containing both the renderer and the destructor
inline renderer_ptr_t make_renderer(SDL_Window* win, int x, Uint32 flags)
{
    return make_resource(SDL_CreateRenderer, SDL_DestroyRenderer, win, x, flags);
}

// Create a surface from a bmp file, containing both the surface and the destructor
inline surf_ptr_t make_bmp(SDL_RWops* sdlfile)
{
    // May throw an exception if sdlfile is nullptr
    return make_resource(SDL_LoadBMP_RW, SDL_FreeSurface, sdlfile, 1);
}

// Create a texture from a renderer and a surface
inline texture_ptr_t make_texture(SDL_Renderer* ren, SDL_Surface* surf)
{
    return make_resource(SDL_CreateTextureFromSurface, SDL_DestroyTexture, ren, surf);
}

} // namespace sdl2
