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

#include "Map.hpp"
#include "Pixelator.hpp"
#include "ImageAtlas.hpp"
#include <memory>
#include <unordered_map>
#include <set>

struct Camera;
struct Texture;

struct Tile
{
    int x;
    int y;
};

class RayCaster
{
public:
    RayCaster();
    ~RayCaster();

    void init(uint32_t width, uint32_t height, std::shared_ptr<utility::Map> map, std::shared_ptr<Pixelator> pixelator);

    void resetDepthBuffer();

    void generateAngleValues(uint32_t width, Camera* camera);

    void drawMinimap(const std::string& buffer_name, const Camera& camera, int blockSize);

    void raycastRender(Camera* camera, double resolution);
    void renderFloor(Camera* camera, double resolution);
    void renderCeiling(Camera* camera, double resolution);
    void renderBuffer();

    void drawTextureColumn(uint32_t x, int32_t y,
                              int32_t h, double depth,
                             uint8_t tileNum, double alphaNum, 
                             uint32_t column, double fadePercent, 
                             SDL_Color targetColor);
    void drawTextureColumnEx(uint32_t x, int32_t y,
                              int32_t h, double depth,
                              Texture texture,
                             uint8_t tileNum, double alphaNum, 
                             uint32_t column, double fadePercent, 
                             SDL_Color targetColor);
    const std::set<uint32_t> getVisited() { return m_visited; }
private:
    std::shared_ptr<utility::Map> m_map;
    std::shared_ptr<Pixelator> m_pixelator;
    std::vector<uint32_t> m_pixels;
    std::vector<uint32_t> m_wall_ceil_pixels;
    
    std::set<uint32_t> m_visited;
    std::set<uint32_t> m_global_visited;

    enum BufferLayer
    {
        BL_BASE,
        BL_ALPHA
    };

    std::vector<double> m_pixel_depth;
    std::vector<double> m_alpha_depth;

    uint32_t m_width;
    uint32_t m_height;
    int tex_tile_width;
    int tex_tile_height;
    int ceil_tile_width;
    int ceil_tile_height;

    void initDepthBuffer();
    double getDepth(uint32_t x, uint32_t y, uint8_t layer);
    void setDepth(uint32_t x, uint32_t y, uint8_t layer, double depth);

    void setPixelAlphaDepth(uint32_t x, uint32_t y, uint32_t color, double alphaNum, double depth);

    double getInterDist(double dx, double dy, double xi, double yi, double coordX, double coordY, double* newX, double* newY, uint8_t* side);
    uint32_t pixelGradientShader(uint32_t pixel, double percent, SDL_Color target);
    void initRayTexture(const std::string& path, int tile_width, int tile_height, int num_tiles);
    void initWallCeilTexture(const std::string& path, int tile_width, int tile_height, int num_tiles);
};
