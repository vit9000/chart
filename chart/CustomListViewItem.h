#pragma once
#include <functional>
#include "ugc.h"

using namespace std;
class CCustomListViewItem
{
	int height;
	function<void()> callBack;
public:
	CCustomListViewItem(int Height, const function<void()>& CallBack) : height(Height), callBack(CallBack) {}
	virtual void OnPaint(UGC& ugc, int y, int width) = 0;

	inline int getHeight() const { return height; }
	inline bool isExecutable() const { return callBack ? true : false; }
	inline void execute() { if (callBack) callBack(); }
};