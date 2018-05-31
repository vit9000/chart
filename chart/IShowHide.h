#pragma once

class IShowHide
{
	bool visible;
public:
	IShowHide() :visible(true) {}
	virtual void setVisible(bool Visible) { visible = Visible; };
	bool getVisible() { return visible; };
};