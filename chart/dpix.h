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
	double getValue(double val) const
	{
		return dpix*val;
	}
	int getIntegerValue(double val)
	{
		return static_cast<int>(dpix*val);
	}
	operator double() const
	{
		return dpix;
	}
	
};