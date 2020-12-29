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
#pragma once

#include <SDL.h>
#include <stdio.h>
#include <math.h>
constexpr auto WIDDERSHINS = 1024;
constexpr auto TURNWISE = 896;

typedef struct _PixBuffer {
    uint32_t* pixels;
    uint32_t width;
    uint32_t height;
} PixBuffer;

typedef struct _RayTex {
	uint32_t* pixData;
	uint32_t tileWidth;
	uint32_t tileHeight;
	uint8_t tileCount;
} RayTex;


class PixelRenderer
{
public:
	static PixBuffer* initPixBuffer(uint32_t width, uint32_t height);
	static void delPixBuffer(PixBuffer* buffer);
	static void drawColumn(PixBuffer* buffer, uint32_t x, int32_t y, int32_t h, SDL_Color color);
	static void drawRow(PixBuffer* buffer, uint32_t x, uint32_t y, uint32_t w, SDL_Color color);
	// drawTexColumn not defined/used?
	//static void drawTexColumn(PixBuffer* buffer, uint32_t x, int32_t y, int32_t h, RayTex* texture, uint8_t tileNum, double alphaNum, uint32_t column, double fadePercent, SDL_Color targetColor);
	static void drawRect(PixBuffer* buffer, SDL_Rect* rect, SDL_Color color);
	static void drawHorizGradient(PixBuffer* buffer, SDL_Rect* rect, SDL_Color colTop, SDL_Color colBottom);
	static void fillBuffer(PixBuffer* target, uint32_t color, double alpha);
	static void drawBuffOffset(PixBuffer* target, PixBuffer* source, uint32_t x, uint32_t y, int32_t xOff);
	static void clearBuffer(PixBuffer* buffer);
	static void paletteFilter(PixBuffer* buffer, SDL_Color* palette, int paletteNum);
	static void orderDither(PixBuffer* buffer, SDL_Color* palette, int paletteNum, double scaleFactor);
	static uint32_t to8BitColor(uint32_t colorDat);
	static void orderDither256(PixBuffer* buffer, double scaleFactor);
	static void monochromeFilter(PixBuffer* buffer, SDL_Color targetColor, double fadePercent);
	static void inverseFilter(PixBuffer* buffer);
	static uint32_t getNearestColor(SDL_Color* palette, int paletteNum, uint32_t colorDat);
	static uint32_t toPixColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a);
	static SDL_Color toSDLColor(uint32_t pixColor);
	static uint32_t blendAlpha(uint32_t baseColor, uint32_t addColor, double alphaNum);
	static uint32_t getPix(PixBuffer* buffer, uint32_t x, uint32_t y);
	static uint32_t getTex(RayTex* texture, uint8_t tileNum, uint32_t x, uint32_t y);
	static void drawPix(PixBuffer* buffer, uint32_t x, uint32_t y, uint32_t color);
	static void drawPixAlpha(PixBuffer* buffer, uint32_t x, uint32_t y, uint32_t color, double alphaNum);
	static void drawPixDouble(PixBuffer* buffer, double x, double y, uint32_t color, double alphaNum);
	static void mergeBuffer(PixBuffer* target, PixBuffer* source, double alpha);
};

RayTex* RayTex_initFromRGBA(uint8_t* rgbaData, uint32_t tileWidth, uint32_t tileHeight, uint8_t numTiles);
void RayTex_delRayTex(RayTex* tex);
