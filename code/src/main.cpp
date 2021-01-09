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
    PixelBuffer* buffer = Pixelator::CreatePixelBuffer(800, 600, sf::Color::Magenta);
    Pixelator::FillBuffer(buffer, sf::Color::Cyan);
    const unsigned int W = 800;
    const unsigned int H = 600;
    // create the window
    sf::RenderWindow window(sf::VideoMode(800, 600), "My window");

    sf::Texture texture;
    texture.create(W, H);

    texture.update(&buffer->pixels[0]);

    sf::Sprite sprite;
    
    sprite.setTexture(texture);

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

        window.draw(sprite);

        // end the current frame
        window.display();
    }

    return 0;
}
