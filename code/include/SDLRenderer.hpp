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
#   (c) 2020 - 2021 Jacob Moena
#
#   MIT License
#*/
#pragma once

#include <SDL.h>
#include <memory>
#include <string>
#include "SDLDeleter.hpp"

struct SDL_Texture;

class SDLRenderer
{
    public:
        SDLRenderer();

        virtual ~SDLRenderer();
        virtual bool initialize(
                const uint16_t screen_width,
                const uint16_t screen_height,
                const std::string& window_title);
        virtual void clearScreen();
        virtual void refreshScreen();
        virtual void setDrawColor(
                const uint8_t red,
                const uint8_t green,
                const uint8_t blue,
                const uint8_t alpha = 255);
        virtual void drawLine(
                const int x_start,
                const int y_start,
                const int x_end,
                const int y_end);
        virtual void drawRectangle(
                const int x_position,
                const int y_position,
                const int width,
                const int height);
        virtual void fillRectangle(
                const int x_position,
                const int y_position,
                const int width,
                const int height);
        virtual SDL_Texture* loadTexture(const std::string& path);
        virtual void drawTexture(SDL_Texture* texture);
        virtual std::string errorMessage() const;

    private:
        using SDLWindowPtr = std::unique_ptr<SDL_Window, SDLDeleter>;
        using SDLRendererPtr = std::unique_ptr<SDL_Renderer, SDLDeleter>;

        SDLWindowPtr window_;
        SDLRendererPtr renderer_;
        std::string errorMessage_;
};
