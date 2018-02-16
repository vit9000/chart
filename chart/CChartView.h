#pragma once

#include <windows.h>
//#include <boost/shared_ptr.hpp>
//using boost::shared_ptr;
#include <memory>
using std::shared_ptr;

#include "ugc.h"
#include "CTableObject.h"
#include "CMainController.h"
#include "CMainModel.h"
#include "Observer.h"
#include "Observable.h"
#include "CTableContainer.h"
#include "ITableCommand.h"


class CChartView : public CWnd, Observer
{
public:
	CChartView();
	virtual ~CChartView();
private:	
	CTableContainer* table_container;
	IChartController* main_controller;
	CMainModel *model;
	int Width;
	int Height;

	
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
	virtual void Update(vector<TableCommand_Ptr>& table_commands);
	
};