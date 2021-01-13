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

#include <SFML/Graphics.hpp>

// C64 color pallette, why not
extern sf::Color commodoreColorPallette[16];

class Pixelator
{
public:
	Pixelator();

	void setSize(const sf::Vector2f size);

	// Pixel
	void setPixel(unsigned int x, unsigned int y, const sf::Color& color);
	sf::Color getPixel(unsigned int x, unsigned int y) const;
	const sf::Uint8* getPixelsPtr() const;

	void fill(sf::Color color);
	void clear();
	void randomize();

private:
	sf::Vector2f m_size;
	std::vector<sf::Uint8> m_pixels;

	// buffers
	struct Buffer
	{
		sf::Vector2u size;
		std::vector<sf::Uint8> pixels;
	};
	std::vector<Buffer> m_buffers;

};
