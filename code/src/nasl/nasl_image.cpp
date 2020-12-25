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
#include <stdlib.h>

#define STB_IMAGE_IMPLEMENTATION
#define STBI_NO_HDR
#define STBI_NO_LINEAR
#define STBI_NO_PIC
#define STBI_NO_PNM
#define STBI_NO_PSD
#define STBI_NO_TGA

#include "stb_image.h"

#include "nasl_image.h"
#include "nasl_buffer.h"


Buffer* nasl_image_load(const char *path) {
    int width, height;
    unsigned char *data = stbi_load(path, &width, &height, NULL, 4);

    Buffer *b = nasl_buffer_create(width, height);

    if (data != NULL) {
        // TODO create function to not have to free pixels
        free(b->pixels);
        b->pixels = (uint32_t *)data;
    } else {
        fprintf(stderr, "nasl_image_load(): Error loading: %s\n", path);
    }

    return b;
}

Buffer* nasl_image_load_from_memory(const unsigned char* memory, int width, int height)
{
    int n = 0;
    unsigned char *data = stbi_load_from_memory(memory, sizeof(uint32_t), &width, &height, &n, 4);

    Buffer *b = nasl_buffer_create(width, height);

    if (data != NULL) {
        // TODO create function to not have to free pixels
        free(b->pixels);
        b->pixels = (uint32_t *)data;
    } else {
        fprintf(stderr, "nasl_image_load_from_memory(): Error loading image from memory!\n");
    }

    return b;
}
