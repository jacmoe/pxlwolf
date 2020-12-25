/*
*   This file is part of the
* ███╗   ██╗ █████╗ ███████╗██╗     
* ████╗  ██║██╔══██╗██╔════╝██║     
* ██╔██╗ ██║███████║███████╗██║     
* ██║╚██╗██║██╔══██║╚════██║██║     
* ██║ ╚████║██║  ██║███████║███████╗
* ╚═╝  ╚═══╝╚═╝  ╚═╝╚══════╝╚══════╝
*   project : https://github.com/jacmoe/nasl
*
*   Copyright 2017 Jacob Moen
*
*/
#ifndef NASLGRAPHICS_H
#define NASLGRAPHICS_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "nasl_buffer.h"

int nasl_graphics_init(int width, int height, const char* title, int fullscreen, int scalefactor);
void nasl_graphics_script_init();
int nasl_graphics_shutdown();

int nasl_graphics_running();

void nasl_graphics_poll_events();

uint32_t nasl_graphics_render(Buffer *buf);

void nasl_graphics_present();

GLFWwindow* nasl_graphics_get_window();

#endif /* NASLGRAPHICS_H */
