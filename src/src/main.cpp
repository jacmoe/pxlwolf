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


int main()
{
	PlxWolf wolf;
	if (wolf.Construct(320, 240, 4, 4))
		wolf.Start();

	return 0;
}
