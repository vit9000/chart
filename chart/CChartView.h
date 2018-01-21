#pragma once

#include <windows.h>
#include <vector>

#include "ugc.h"
#include "CTableObject.h"
#include "CMainController.h"
#include "CMainModel.h"
#include "CTableLine.h"
#include "IObserver.h"
#include "IObservable.h"


class CChartView : public CWnd, IObserver
{
public:
	CChartView();
	virtual ~CChartView();
private:	
	private:
	std::vector<CTableObject*> table_lines;
	IChartController * main_controller;
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