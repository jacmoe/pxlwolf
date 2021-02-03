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
        for (auto& img : m_buffers)
        {
            UnloadImage(img);
        }
    }

    bool ImageAtlas::load(const std::string& path, Vector2 tile_size)
    {
        Image source_image = LoadImage(path.c_str());
        if(source_image.data == nullptr)
        {
            return false;
        }

        const auto rows = source_image.width / tile_size.x;
        const auto cols = source_image.height / tile_size.y;
        unsigned int index = 0;

        m_rows = rows;
        m_cols = cols;

        m_width =  tile_size.x;
        m_height = tile_size.y;
        TraceLog(LOG_INFO,"source image dimensions : width %d, height %d", source_image.width, source_image.height);
        TraceLog(LOG_INFO,"m_width = %.f, m_height = %.f", m_width, m_height);

        for (unsigned y = 0; y < cols; ++y)
        {
            for (unsigned x = 0; x < rows; ++x)
            {
                Image image = ImageFromImage(source_image, { x * m_width, y * m_height, m_width, m_height });
                if(image.height != m_height)
                {
                    TraceLog(LOG_ERROR, "Image wasn't created!");
                    return false;
                }
                TraceLog(LOG_INFO,"Intrect is (%.f, %.f, %.f, %.f)", x * m_width, y * m_height, m_width, m_height);
                m_buffers.push_back(image);
            }
        }
        UnloadImage(source_image);
        return true;
    }

    Color* ImageAtlas::getPixels(int index)
    {
        Color* pixels = LoadImageColors(m_buffers[index]);
        return pixels;
    }
}
