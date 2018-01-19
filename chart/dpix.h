#pragma once
#include <windows.h>

class DPIX
{
protected:
	double dpix;
public:
	DPIX()
	{
		HDC hdcScreen = ::GetDC(NULL);
		dpix = 1; // assume failure
		if (hdcScreen)
		{
			dpix = (double)::GetDeviceCaps(hdcScreen, LOGPIXELSX);
			::ReleaseDC(NULL, hdcScreen);
			dpix /= 96;
		}
	}
	double getValue()
	{
		return dpix;
	}
	operator double() const
	{
		return dpix;
	}
	operator int() const
	{
		return static_cast<int>(dpix);
	}
};