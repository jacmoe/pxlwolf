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

#include "Application.hpp"
#include "Pixelator.hpp"
#include <SFML/Graphics.hpp>
#include <cereal/types/vector.hpp>
#include <cereal/types/string.hpp>
#include <cereal/types/complex.hpp>
#include <cereal/types/memory.hpp>
#include <cereal/archives/binary.hpp>

namespace cereal {
	template <class Archive>
	void load(Archive & ar, sf::Color & c)
	{
		ar(c.r, c.g, c.b, c.a);
	}

	template <class Archive>
	void save(Archive & ar, const sf::Color & c)
	{
		ar(c.r, c.g, c.b, c.a);
	}
}

struct PaletteStruct
{
	std::vector<sf::Color> palette_colors;

 	template<class Archive>
 	void serialize(Archive & ar)
	{
		ar( palette_colors );
	}
};

struct ColorStruct
{
	std::vector<unsigned int> colors;

 	template<class Archive>
	void serialize(Archive & ar)
	{
		ar( colors );
	}
};

class Game : public Application
{
public:
	Game();
	~Game();

	bool OnUserCreate() override;
	bool OnUserUpdate(sf::Time elapsedTime) override;
	bool OnUserRender() override;
	bool OnUserDestroy() override;
private:
	PixelBuffer* buffer;
};
