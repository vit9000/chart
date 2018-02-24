#pragma once

class IShowHide
{
public:
	virtual void setVisible(bool visible) = 0;
	virtual bool getVisible() = 0;
};