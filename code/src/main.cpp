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
#ifdef _WIN32
#include <windows.h>
#endif
#include <filesystem>

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "nasl_graphics.h"
#include "nasl_buffer.h"
#include "utils.h"
#include <GLFW/glfw3.h>

static int init(int width, int height);
static int shutdown();
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

#ifdef _WIN32f
INT WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, INT nCmdShow)
#else
int main()
#endif
{
    std::string path = moena::utils::get_homedir().append("/source/repos/pxlwolf/");
    std::filesystem::current_path(path); 

    int buffer_width = 320;
    int buffer_height = 240;

    init(buffer_width, buffer_height);

    // Create main buffer
    Buffer* buffer = nasl_buffer_create(buffer_width, buffer_height);
    nasl_buffer_set_mainbuffer(buffer);
    // Clear main buffer to a blue color
    nasl_buffer_clear(buffer, BLUE);

    int pal_offset = (buffer_width / 5) / 2;
    int pal_width = (buffer_width - (pal_offset * 2)) / 4;
    int pal_height = (buffer_height - (pal_offset * 2)) / 4;

    // Create a palette buffer
    Buffer* palette_buffer = nasl_buffer_create(pal_width,pal_height);

    // Draw a palette by blitting 16 different palette buffers into the main buffer
    int col = 0;
    int row = pal_offset;
    for(int buf = 0; buf < 16; buf++)
    {
        nasl_buffer_clear(palette_buffer, c64_palette[buf]);
        nasl_buffer_blit(buffer, palette_buffer, pal_offset + (pal_width * col), row);
        col++;
        if(col % 4 == 0)
        {
            col = 0;
            row += pal_height;
        }
    }
    // Destroy the palette buffer
    nasl_buffer_destroy(palette_buffer);

    // Main loop
    while(nasl_graphics_running())
    {
        // Event polling
        nasl_graphics_poll_events();
        // Render the main buffer
        nasl_graphics_render(buffer);
        // Swap buffers
        nasl_graphics_present();
    }

    // Destroy the main buffer
    nasl_buffer_destroy(buffer);

    shutdown();
}

static int init(int width, int height)
{
    nasl_graphics_init(width, height, "Neogardens Palette Demo", 0, 2);

    glfwSetKeyCallback(nasl_graphics_get_window(), key_callback);

    return 1;
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
}

static int shutdown()
{
    nasl_graphics_shutdown();

    return 1;
}
