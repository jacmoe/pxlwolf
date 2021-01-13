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

	// Buffers
	unsigned int addBuffer();
	bool removeBuffer(const unsigned int index);
	void setActiveBuffer(const unsigned int index);
	const unsigned int getActiveBuffer() { return m_current_buffer; }
	unsigned int getNumberOfBuffers() const { return static_cast<unsigned int>(m_buffers.size()); }

private:
	unsigned int m_current_buffer;

	// buffers
	struct Buffer
	{
		sf::Vector2f size;
		std::vector<sf::Uint8> pixels;
	};
	std::vector<Buffer> m_buffers;

};
