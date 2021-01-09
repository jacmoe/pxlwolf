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
#include "Pixelator.hpp"
#include <vector>

typedef struct _Camera {
	double x;
	double y;
	double h;
	double angle;
	double dist;
	double fov;
	double angleValues[WIDDERSHINS];
} Camera;

typedef struct _Map {
	std::vector<int> data;
	SDL_Color* colorData;
	int numColor;
	int width;
	int height;
	int border;
} Map;

enum BufferLayer {
	BL_BASE,
	BL_ALPHA
};

typedef struct _DepthBuffer {
	PixBuffer* pixelBuffer;
	PixBuffer* alphaBuffer;
	double* pixelDepth;
	double* alphaDepth;
} DepthBuffer;

typedef struct _RaySprite {
	RayTex* texture;
	uint8_t frameNum;
	double alphaNum;
	double scaleFactor;
	double x;
	double y;
	double h;
} RaySprite;

class RaycasterEngine
{
public:
    static DepthBuffer* initDepthBuffer(uint32_t width, uint32_t height);
    static double getDepth(DepthBuffer* buffer, uint32_t x, uint32_t y, uint8_t layer);
    static void setDepth(DepthBuffer* buffer, uint32_t x, uint32_t y, uint8_t layer, double depth);
    static void drawPix(DepthBuffer* buffer, uint32_t x, uint32_t y, uint32_t color, double alphaNum, double depth);
    static void drawTexColumn(DepthBuffer* buffer, uint32_t x, int32_t y, int32_t h, double depth, RayTex* texture, uint8_t tileNum, double alphaNum, uint32_t column, double fadePercent, SDL_Color targetColor);
    static void renderBuffer(DepthBuffer* buffer);
    static void resetDepthBuffer(DepthBuffer* buffer);
    static void delDepthBuffer(DepthBuffer* buffer);
    static void generateMap(Map* newMap, const std::vector<int>& map_data, int width, int height, int border, SDL_Color* colorData, int numColor);
    static void initSprite(RaySprite* newSprite, RayTex* texture, double scaleFactor, double alphaNum, double x, double y, double h);
    static void draw2DSprite(PixBuffer* buffer, RaySprite sprite, double angle);
    static void generateAngleValues(uint32_t width, Camera* camera);
    static void draw3DSprite(DepthBuffer* rayBuffer, Camera* camera, uint32_t width, uint32_t height, double resolution, RaySprite sprite);
    static void raycastRender(DepthBuffer* rayBuffer, Camera* camera, uint32_t width, uint32_t height, Map* map, double resolution, RayTex* texData);
    static void texRenderFloor(PixBuffer* buffer, Camera* camera, uint32_t width, uint32_t height, Map* groundMap, double resolution, RayTex* texData, uint8_t tileNum);
    static void texRenderCeiling(PixBuffer* buffer, Camera* camera, uint32_t width, uint32_t height, Map* ceilingMap, RayTex* texData, uint8_t tileNum);
    static uint32_t pixGradientShader(uint32_t pixel, double percent, SDL_Color target);
    static void drawMinimap(PixBuffer* buffer, Camera* camera, unsigned int width, unsigned int height, Map* map, int blockSize);
    static void deleteMap(unsigned char** map, int width, int height);
};
