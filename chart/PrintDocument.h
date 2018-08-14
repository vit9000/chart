#pragma once
#include <functional>
#include "dpix.h"
#include "ugc.h"
#include "stdafx.h"
using std::function;

class CPrintDocument
{
private:
protected:
	const double width_mm;
	const double height_mm;
	int page;
	int count_pages;
	CDC& dcPrinter;
	UGC ugc;

	struct PrintBorders
	{
		int left;
		int top;
		int right;
		int bottom;
	} borders_mm;
public:
	CPrintDocument(CDC& priterDC)
		:width_mm(210.), 
		height_mm(297.),
		page(0), // нумерация как и привычна для страниц принтера - с 1
		count_pages(1),
		dcPrinter(priterDC),
		ugc(dcPrinter.m_hDC, 0, 0),
		borders_mm({0,0,0,0})
	{
		NextPage();
	}

	inline void setBorders(const PrintBorders& pBorders) { borders_mm = pBorders; }

	double toInches(double mm) { return mm / 25.4; }

	inline double getInchWidth() { return toInches(width_mm - borders_mm.right - borders_mm.left); }
	inline double getInchHeight() { return toInches(height_mm - borders_mm.top - borders_mm.bottom); }
	inline int getPxX() { return int(DPIX::GetDPI()*toInches(borders_mm.left)); }
	inline int getPxY() { return int(DPIX::GetDPI()*toInches(borders_mm.top));}
	inline int getPxWidth() { return int(DPIX::GetDPI()*getInchWidth()); }
	inline int getPxHeight() { return int(DPIX::GetDPI()*getInchHeight()); }
	inline UGC& getUGC() { return ugc; }
	inline void setCountPages(int count) { count_pages = count; }
	inline int getCountPages() { return count_pages; }
	inline int getCurrPage() { return page; }
	bool NextPage()
	{
		if (page > 0)
			dcPrinter.EndPage();
		page++;

		if (dcPrinter.StartPage() < 0)
		{
			dcPrinter.AbortDoc();
			return false;
		}
		return true;
	}
	
	void Finish()
	{
		if (page >= 0)
			dcPrinter.EndPage();
	}
	
};