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

struct Camera;

class RayCaster
{
public:
    RayCaster();
    ~RayCaster();

    void init(uint32_t width, uint32_t height, std::shared_ptr<utility::Map> map, std::shared_ptr<Pixelator> pixelator);

    void drawMinimap(const std::string& buffer_name, const Camera& camera, int blockSize);

    void raycastCeilingFloor(const Camera& camera);
    void raycast(const Camera& camera);

private:
    std::shared_ptr<utility::Map> m_map;
    std::shared_ptr<Pixelator> m_pixelator;

    utility::ImageAtlas m_atlas;
    utility::ImageAtlas m_wall_ceil_atlas;

    uint32_t m_width;
    uint32_t m_height;

    std::vector<double> m_depth_buffer;

    uint32_t pixelGradientShader(uint32_t pixel, double amount, SDL_Color target);
};
