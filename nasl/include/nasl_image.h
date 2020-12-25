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
#ifndef NASLIMAGELOAD_H
#define NASLIMAGELOAD_H

#include "nasl_buffer.h"

Buffer* nasl_image_load(const char *path);
Buffer* nasl_image_load_from_memory(const unsigned char* memory, int width, int height);
void nasl_image_script_init();

#endif // NASLIMAGELOAD_H
