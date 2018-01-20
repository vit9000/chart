#pragma once

#include <windows.h>

#include "ugc.h"
#include "TableView.h"

class CChart : public CWnd
{
public:
	CChart();
	virtual ~CChart();
private:	
	TableView* tableView;
	int Width;
	int Height;
	
protected:
	void SetBounds();
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnLButtonUp(UINT flags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	

	DECLARE_MESSAGE_MAP();

};