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

#include <SFML/Config.hpp>
#include <SFML/Graphics.hpp>

#include "Pixelator.hpp"

int main()
{
    PixelBuffer* buffer = Pixelator::CreatePixelBuffer(800, 600, sf::Color::Red);
    const unsigned int W = 400;
    const unsigned int H = 300;
    // create the window
    sf::RenderWindow window(sf::VideoMode(800, 600), "My window");

    sf::Texture texture;
    texture.create(W, H);

    texture.update(&buffer->pixels[0]);

    sf::Sprite sprite;
    
    sprite.setTexture(texture);
    sprite.setScale(2,2);

    Pixelator::FillBuffer(buffer, commodoreColorPallette[2]);
    texture.update(&buffer->pixels[0]);

    // run the program as long as the window is open
    while (window.isOpen())
    {
        // check all the window's events that were triggered since the last iteration of the loop
        sf::Event event;
        while (window.pollEvent(event))
        {
            // "close requested" event: we close the window
            if (event.type == sf::Event::Closed)
                window.close();
        }

        // clear the window with black color
        window.clear(sf::Color::Black);

        sf::Color color;

        for (uint32_t i = 0; i < buffer->height; i++)
        {
            if (i < buffer->height)
            {
                for (uint32_t j = 0; j < buffer->width; j++)
                {
                    if (j < buffer->width)
                    {
                        Pixelator::PutPixelAlpha(buffer, j, i, sf::Color(rand() % 255, rand() % 255, rand()% 255, rand()% 255), 1.0);
                    }
                }
            }
        }


        texture.update(&buffer->pixels[0]);

        window.draw(sprite);

        // end the current frame
        window.display();
    }

    return 0;
}
