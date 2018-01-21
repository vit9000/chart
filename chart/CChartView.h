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

	std::vector<CTableObject*> table_lines;
	IChartController * main_controller;
	IModel *model;
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
	IModel* getModel() { return model;}
	IChartController* getController() { return main_controller;}
	virtual void Update() { RedrawWindow();};

	
};