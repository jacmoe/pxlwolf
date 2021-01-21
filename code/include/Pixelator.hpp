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

	void setSize(const sf::Vector2i size);

	// Pixel
	void setPixel(unsigned int x, unsigned int y, const sf::Color& color);
	sf::Color getPixel(unsigned int x, unsigned int y) const;
	const sf::Uint8* getPixelsPtr() const;

	void fill(sf::Color color);
	void clear();

	// Buffers
	bool addBuffer(const std::string name);
	bool removeBuffer(const std::string name);
	void setActiveBuffer(const std::string name);
	const std::string getActiveBuffer() { return m_current_buffer; }
	unsigned int getNumberOfBuffers() const { return static_cast<unsigned int>(m_buffers.size()); }

	bool swapBuffer(const std::string name);

	void copy(const std::string name, unsigned int destX, unsigned int destY, const sf::IntRect& sourceRect, bool applyAlpha = false);
	void copy(const sf::Image& source, unsigned int destX, unsigned int destY, const sf::IntRect& sourceRect, bool applyAlpha = false);
	void copy(const sf::Uint8* source_pixels, const sf::Vector2i buffer_size, unsigned int destX, unsigned int destY, const sf::IntRect& sourceRect, bool applyAlpha = false);

	sf::IntRect getSize() { return getSize(m_current_buffer); }
	sf::IntRect getSize(const std::string name);

private:
	std::string m_current_buffer;
	std::unordered_map<std::string, unsigned int> m_buffer_map;

	// buffers
	struct Buffer
	{
		sf::Vector2i size;
		std::vector<sf::Uint8> pixels;
	};
	std::vector<Buffer> m_buffers;

};
