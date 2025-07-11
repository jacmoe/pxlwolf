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
#   (c) 2020 - 2025 Jacob Moena
#
#   MIT License
#*/
#pragma once

#include "main/Application.hpp"
#include "utility/Map.hpp"
#include <memory>

class Game final : public Application
{
public:
    Game();
    ~Game() override;

protected:
    bool OnUserCreate() override;
    bool OnUserUpdate(double deltaTime) override;
    bool OnUserRender() override;
    bool OnUserPostRender() override;
    bool OnUserInput() override;
    bool OnUserDestroy() override;

private:
    double m_delta_time;

    std::shared_ptr<utility::Map> m_map;
};
