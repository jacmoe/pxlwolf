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
    RayCaster(std::shared_ptr<utility::Map> map, std::shared_ptr<Pixelator> pixelator);

    void generateAngleValues(uint32_t width, Camera* camera);

    void drawMinimap(const std::string& owner, const std::string& name, const Camera& camera, int blockSize);

private:
    std::shared_ptr<utility::Map> m_map;
    std::shared_ptr<Pixelator> m_pixelator;

    std::vector<double> m_pixel_depth;
    std::vector<double> m_alpha_depth;

    uint32_t m_width;
    uint32_t m_height;

    void initDepthBuffer(uint32_t width, uint32_t height);
    void resetDepthBuffer();
    double getDepth(uint32_t x, uint32_t y, uint8_t layer);
    void setDepth(uint32_t x, uint32_t y, uint8_t layer, double depth);

    double getInterDist(double dx, double dy, double xi, double yi, double coordX, double coordY, double* newX, double* newY, uint8_t* side);
};
