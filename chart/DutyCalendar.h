#pragma once
#include "ugc.h"
#include <string>
#include "VCalendar.h"


using namespace std;




class CDutyCalendar : public CDialogEx
{
	DECLARE_DYNAMIC(CDutyCalendar)

public:
	CDutyCalendar(const CRect& rect, CWnd* pParent = nullptr);   // стандартный конструктор
	virtual ~CDutyCalendar();

	
// Данные диалогового окна
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_DUTY };
#endif
	const COleDateTime& getSelectedDate() { return calendar.getSelectedDate(); }
protected:
	int X, Y;
	int Width, Height;
	DPIX dpix;
	VCalendar calendar;
	afx_msg BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);    // поддержка DDX/DDV
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnPaint();
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	DECLARE_MESSAGE_MAP()


	

};
