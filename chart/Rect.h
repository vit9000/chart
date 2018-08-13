#pragma once

struct Rect
{
	Rect(int X, int Y, int Width, int Height, int Reserved=0)
		: x(X), y(Y), width(Width), height(Height), reserved(Reserved)
	{	}
	int x;
	int y;
	int width;
	int height;
	int reserved;
	int page;
	//int Width() { return width - x; }
	//int Height() { return height - y; }
};