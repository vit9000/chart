#pragma once

#include <windows.h>
#include <vector>

#include "ugc.h"
#include "CTableObject.h"
#include "CMainController.h"
#include "CMainModel.h"
#include "CTableLine.h"
#include "Observer.h"
#include "Observable.h"




class CChartView : public CWnd, Observer
{
public:
	CChartView();
	virtual ~CChartView();
private:	

	std::vector<CTableObject*> table_lines;
	IChartController * main_controller;
	CMainModel *model;
	int Width;
	int Height;
	int LineHeight;
	int HeaderWidth;
	
protected:
	void ClearTableObjects();
	void SetBounds();
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnLButtonUp(UINT flags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	

	DECLARE_MESSAGE_MAP();
public:
	CMainModel* getModel() { return model;}
	IChartController* getController() { return main_controller;}
	virtual void Update(); 
	
};