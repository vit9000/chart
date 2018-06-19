#pragma once
#include "ugc.h"

// Диалоговое окно CDutyCalendar

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

protected:
	int X, Y;
	int Width, Height;
	afx_msg BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);    // поддержка DDX/DDV
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnPaint();
	void DrawCalendar(UGC& ugc, int x, int y, int width, int height);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	DECLARE_MESSAGE_MAP()


	int GetDaysInMonth(COleDateTime dt)
	{
		dt.SetDate(dt.GetYear(), dt.GetMonth(), 1);
		COleDateTime dtNextMonth(dt);
		GetNextMonth(dtNextMonth);
		COleDateTimeSpan dtspan = dtNextMonth - dt;
		return dtspan.GetTotalDays();
	}

	void GetNextMonth(COleDateTime& dt)
	{
		if (dt.GetMonth() == 12)
			dt.SetDate(dt.GetYear() + 1, 1, 1);
		else
			dt.SetDate(dt.GetYear(), dt.GetMonth() + 1, 1);
	}

	void GetPrevMonth(COleDateTime& dt)
	{
		if (dt.GetMonth() == 1)
			dt.SetDate(dt.GetYear() - 1, 12, 1);
		else
			dt.SetDate(dt.GetYear(), dt.GetMonth() - 1, 1);
	}

};
