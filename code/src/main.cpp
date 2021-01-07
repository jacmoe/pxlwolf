#include <iostream>
#include <stdexcept>

#include "Game.hpp"

int main(int argc, char **argv)
{
    Game game;
    // TODO: Remove exception handling
    try
    {
        game.init();
        game.run();
    }
    catch (std::runtime_error& e)
    {
        std::cerr << e.what() << std::endl;
        return -1;
    }

    return 0;
}

