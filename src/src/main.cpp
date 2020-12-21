/*# This file is part of the
  #   ___ _       __    __      _  __ 
  #  / _ \ |_  __/ / /\ \ \___ | |/ _|
  # / /_)/ \ \/ /\ \/  \/ / _ \| | |_ 
  #/ ___/| |>  <  \  /\  / (_) | |  _|
  #\/    |_/_/\_\  \/  \/ \___/|_|_|  
  # project
  # https://github.com/jacmoe/pxlwolf
  #
  # (c) 2020 Jacob Moena
  #
  # BSD 2-Clause "Simplified" License
  #
*/
#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"

class PlxWolf : public olc::PixelGameEngine
{
private:
	int pal_offset;
	int pal_width;
	int pal_height;

public:
	PlxWolf()
	{
		sAppName = "PlxWolf";
	}

public:
	bool OnUserCreate() override
	{
		Clear(olc::DARK_GREY);

		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{
		return true;
	}
};


#ifdef _WIN32
INT WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR lpCmdLine, INT nCmdShow)
#else
int main()
#endif
{
	PlxWolf wolf;
	if (wolf.Construct(640, 480, 2, 2))
		wolf.Start();

	return 0;
}
