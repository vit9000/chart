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

	function<void(UGC&)> drawColontitle;
	int colontitleHeight_mm;
	int colontitleBorder_mm;

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
		colontitleHeight_mm(0),
		colontitleBorder_mm(0),
		borders_mm({0,0,0,0})
	{
		NextPage();
	}

	inline void setBorders(const PrintBorders& pBorders) { borders_mm = pBorders; }
	void setColontitle(int height_mm, const function<void(UGC&)>& DrawColontitle) { colontitleHeight_mm = height_mm; colontitleBorder_mm = 4; drawColontitle = DrawColontitle; }
	inline void setCountPages(int count) { count_pages = count; }
	inline UGC& getUGC() { return ugc; }

	inline double toInches(double mm) const		{ return mm / 25.4; }
	inline double getInchWidth() const			{ return toInches(width_mm - borders_mm.right - borders_mm.left); }
	inline double getInchHeight() const			{ return toInches(height_mm - borders_mm.top - borders_mm.bottom - colontitleHeight_mm - colontitleBorder_mm); }
	inline int getPxX() const					{ return int(DPIX::GetDPI()*toInches(borders_mm.left)); }
	inline int getPxY() const					{ return int(DPIX::GetDPI()*toInches(borders_mm.top + colontitleHeight_mm + colontitleBorder_mm));}
	inline int getPxPageY() const				{ return int(DPIX::GetDPI()*toInches(borders_mm.top)); }
	inline int getPxWidth() const				{ return int(DPIX::GetDPI()*getInchWidth()); }
	inline int getPxHeight() const				{ return int(DPIX::GetDPI()*getInchHeight()); }
	inline int getCountPages() const			{ return count_pages; }
	inline int getCurrPage() const				{ return page; }
	inline int getPxColontitleHeight() const	{ return int(DPIX::GetDPI()*toInches(colontitleHeight_mm));; }

	bool NextPage()
	{
		Finish();
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
		if (page > 0)
		{
			if (drawColontitle)
			{
				drawColontitle(ugc);
				ugc.SetTextSize(10);
				ugc.SetAlign(UGC::CENTER);
				ugc.DrawNumber(page, getPxX() + getPxWidth() / 2, getPxY() + getPxHeight());
				ugc.SetAlign(UGC::LEFT);
			}
			dcPrinter.EndPage();
		}
	}
	
};