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
	const double width;
	const double height;
	int page;
	int count_pages;
	CDC& dcPrinter;
	UGC ugc;
public:
	CPrintDocument(CDC& priterDC)
		:width(210.), 
		height(297.), 
		page(0), // нумерация как и привычна для страниц принтера - с 1
		count_pages(1),
		dcPrinter(priterDC),
		ugc(dcPrinter.m_hDC, 0, 0)
	{
		NextPage();
	}
	double toInches(double mm) { return mm / 25.4; }

	inline double getInchWidth() { return toInches(width); }
	inline double getInchHeight() { return toInches(height); }
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