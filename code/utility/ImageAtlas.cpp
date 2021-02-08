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
#include "ImageAtlas.hpp"
#include "stb_image.h"
#include "spdlog/spdlog.h"

namespace utility
{
    ImageAtlas::ImageAtlas()
    : m_buffers(0)
    , m_rows(0)
    , m_cols(0)
    , m_width(0)
    , m_height(0)
    {}

    ImageAtlas::~ImageAtlas()
    {
    }

    bool ImageAtlas::load(const std::string& path, linalg::aliases::int2 tile_size)
    {
        int32_t req_format = STBI_rgb_alpha;
        int32_t orig_format;
        int32_t image_width;
        int32_t image_height;
        uint8_t* pixels = stbi_load(path.c_str(), &image_width, &image_height, &orig_format, req_format);
        if (!pixels)
        {
            SPDLOG_ERROR("Could not load {} because {}", path, stbi_failure_reason());
            return false;
        }

        m_cols = image_width / tile_size.x;
        m_rows = image_height / tile_size.y;

        m_width =  tile_size.x;
        m_height = tile_size.y;
        SPDLOG_INFO("source image dimensions : width {}, height {}", image_width, image_height);
        SPDLOG_INFO("m_width = {}, m_height = {}", m_width, m_height);
        SPDLOG_INFO("m_rows = {}, m_cols = {}", m_rows, m_cols);

        int offset = 0;
        for(int i = 0; i < m_rows; i++)
        {
            Buffer buffer;
            buffer.width = m_width;
            buffer.height = m_height;
            buffer.pixels = (uint32_t*)malloc(sizeof(uint32_t)*m_width*m_height);

            uint32_t newPix = 0;
            for (uint32_t p = 0; p < m_width * m_height; p++)
            {
                // Get each component
                for (uint8_t comp = 0; comp < 4; comp++)
                {
                    newPix |= ((uint32_t)(pixels[(p + offset)*4+comp]) << (8 * (3-comp)));
                }
                buffer.pixels[p] = newPix;
                newPix = 0;
            }
            m_buffers.push_back(buffer);
            offset += m_width * m_height;
        }

        stbi_image_free(pixels);

        SPDLOG_INFO("{} loaded succesfully.", path);

        return true;
    }
}
