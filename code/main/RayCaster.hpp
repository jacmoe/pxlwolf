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
#include <memory>

struct Camera;

class RayCaster
{
public:
    RayCaster() = default;

    void init(uint32_t width, uint32_t height, std::shared_ptr<utility::Map> map, std::shared_ptr<Pixelator> pixelator);

    void resetDepthBuffer();

    void generateAngleValues(uint32_t width, Camera* camera);

    void drawMinimap(const std::string& buffer_name, const Camera& camera, int blockSize);

    void raycastRender(Camera* camera, double resolution);

    void renderBuffer();

private:
    std::shared_ptr<utility::Map> m_map;
    std::shared_ptr<Pixelator> m_pixelator;

    enum BufferLayer
    {
        BL_BASE,
        BL_ALPHA
    };

    std::vector<double> m_pixel_depth;
    std::vector<double> m_alpha_depth;

    uint32_t m_width;
    uint32_t m_height;

    void initDepthBuffer();
    double getDepth(uint32_t x, uint32_t y, uint8_t layer);
    void setDepth(uint32_t x, uint32_t y, uint8_t layer, double depth);

    void setPixelAlphaDepth(uint32_t x, uint32_t y, uint32_t color, double alphaNum, double depth);

    void drawTextureColumn(uint32_t x, int32_t y,
                              int32_t h, double depth,
                             uint8_t tileNum, double alphaNum, 
                             uint32_t column, double fadePercent, 
                             sf::Color targetColor);

    double getInterDist(double dx, double dy, double xi, double yi, double coordX, double coordY, double* newX, double* newY, uint8_t* side);
    uint32_t toIntColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a);
    sf::Color toSFMLColor(uint32_t pixColor);
    uint32_t pixelGradientShader(uint32_t pixel, double percent, sf::Color target);
};
