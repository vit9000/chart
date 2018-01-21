#pragma once

#include <windows.h>

#include "ugc.h"
#include "CMainModel.h"
#include "TableView.h"
#include "IObserver.h"
#include "IObservable.h"


class CChart : public CWnd, IObserver
{
public:
	CChart();
	virtual ~CChart();
private:	
	TableView* tableView;
	CMainModel tableModel;
	int Width;
	int Height;
	
protected:
	void SetBounds();
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnLButtonUp(UINT flags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	

	DECLARE_MESSAGE_MAP();
public:
	CMainModel* getModel() { return &tableModel;}
	virtual void Update() 
	{ 
		RedrawWindow();
	};

	void setDatabase(size_t index)
	{
		
		tableModel.setDatabase(index);
	}
};