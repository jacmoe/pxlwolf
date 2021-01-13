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
#include <unordered_map>

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
	unsigned int addBuffer(const std::string name);
	bool removeBuffer(const std::string name);
	void setActiveBuffer(const std::string name);
	const std::string getActiveBuffer() { return m_current_buffer; }
	unsigned int getNumberOfBuffers() const { return static_cast<unsigned int>(m_buffers.size()); }

private:
	std::string m_current_buffer;
	std::unordered_map<std::string, unsigned int> m_buffer_map;

	// buffers
	struct Buffer
	{
		sf::Vector2f size;
		std::vector<sf::Uint8> pixels;
	};
	std::vector<Buffer> m_buffers;

};
