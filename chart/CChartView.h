#pragma once

#include <windows.h>
//#include <boost/shared_ptr.hpp>
//using boost::shared_ptr;
#include <memory>
using std::shared_ptr;

#include "ugc.h"
#include "CMainController.h"
#include "CMainModel.h"
#include "Observer.h"
#include "Observable.h"
#include "CTableContainer.h"
#include "ITableCommand.h"
#include "CursorHandler.h"
#include "CInPlaceEditbox.h"


class CChartView : public CWnd, public Observer, public CursorHandler
{
public:
	CChartView(bool _print_mode=false);
	virtual ~CChartView();
private:	
	CTableContainer* table_container;
	IChartController* main_controller;
	CMainModel *model;
	int Width;
	int Height;
	int cursor_type;
	bool print_mode;
	
protected:
	//void ClearTableObjects();
	
	void SetBounds(bool OnSize = false);
	void SetBounds(int width, int height, bool OnSize = false);
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnLButtonUp(UINT flags, CPoint point);
	afx_msg void OnRButtonUp(UINT flags, CPoint point);
	afx_msg void OnLButtonDown(UINT flags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	

	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	//afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);

	DECLARE_MESSAGE_MAP();
public:
	void ResetCurPos()
	{
		SetBounds(true);
		this->SetScrollPos(SB_VERT, 0);
		if (table_container)
			table_container->setScroll(0);
		RedrawWindow();
	}
	
	void PrintAll(HBITMAP *hbitmap);
	void PrintAll(UGC& ugc, CPrintDocument* pDoc);
	CMainModel* getModel() { return model;}
	IChartController* getController() { return main_controller;}
	virtual void Update(vector<TableCommand_Ptr>& table_commands);
	void SetMouseCursor(size_t index) override
	{
		current = index;
		SetCursor(cursors.at(index));
	}

	void setEditBox(const Rect& rect, function<void(const std::wstring&)> callBack, const wstring& defaultValue, bool IsDigit) override
	{
		CEdit * pEdit = new CInPlaceEditbox(callBack, defaultValue, IsDigit);
		DWORD dwStyle = ES_CENTER | WS_CHILD | WS_VISIBLE | ES_AUTOHSCROLL;
		//dwStyle |= WS_BORDER;
		RECT r;
		double dpix = DPIX();
		int border = static_cast<int>(2 * dpix);
		int h = static_cast<int>(18. * dpix);
		r.left = rect.x+border;
		r.right = rect.x + rect.width - border*2;
		int temp = (rect.height - h) / 2;
		r.top = rect.y+temp;
		r.bottom = rect.y + rect.height-temp;
		pEdit->Create(dwStyle, r, this, IDC_EDIT_VALUE);
		
	}

	void getWindowPos(int& x, int& y)
	{
		RECT r;
		GetWindowRect(&r);
		x = r.left;
		y = r.top;
	}
};