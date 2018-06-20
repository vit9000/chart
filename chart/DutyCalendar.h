#pragma once
#include "ugc.h"
#include <string>
#include <vector>

using namespace std;

// Диалоговое окно CDutyCalendar
class VCalendar
{
	int x, y, width, height; 
	int const lines, rows;
	int bitW, bitH;
	COleDateTime currentDate;
	DPIX dpix;
	int currentDay;
	vector<wstring> week_days;
	int startDutyHour;
public:
	VCalendar(int X, int Y, int Width, int Height)
		: x(X), y(Y), width(Width), height(Height),
		lines(6), rows(7),
		bitW(width / rows), bitH(height / lines),
		currentDate(COleDateTime::GetCurrentTime()),
		week_days({L"Пн", L"Вт", L"Ср", L"Чт", L"Пт", L"Сб", L"Вс" }),
		startDutyHour(9)
	{}
	void Draw(UGC& ugc);
	bool OnLButtonClick(int x, int y);
	inline const COleDateTime& getCurrentDate() { currentDay = currentDate.GetDay(); return currentDate; }
};



class CDutyCalendar : public CDialogEx
{
	DECLARE_DYNAMIC(CDutyCalendar)

public:
	CDutyCalendar(const CRect& rect, CWnd* pParent = nullptr);   // стандартный конструктор
	virtual ~CDutyCalendar();

	friend VCalendar;
	
// Данные диалогового окна
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_DUTY };
#endif

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


	static int GetDaysInMonth(COleDateTime dt)
	{
		dt.SetDate(dt.GetYear(), dt.GetMonth(), 1);
		COleDateTime dtNextMonth(dt);
		GetNextMonth(dtNextMonth);
		COleDateTimeSpan dtspan = dtNextMonth - dt;
		return dtspan.GetTotalDays();
	}

	static void GetNextMonth(COleDateTime& dt)
	{
		if (dt.GetMonth() == 12)
			dt.SetDate(dt.GetYear() + 1, 1, 1);
		else
			dt.SetDate(dt.GetYear(), dt.GetMonth() + 1, 1);
	}

	static void GetPrevMonth(COleDateTime& dt)
	{
		if (dt.GetMonth() == 1)
			dt.SetDate(dt.GetYear() - 1, 12, 1);
		else
			dt.SetDate(dt.GetYear(), dt.GetMonth() - 1, 1);
	}

};
