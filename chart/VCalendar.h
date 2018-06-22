#pragma once
#include <string>
#include <vector>
#include "ugc.h"
using namespace std;

// Диалоговое окно CDutyCalendar
class VCalendar : public CWnd
{
	enum { ROWS = 6, COLS = 7 };
	int Width, Height;
	int bitW, bitH;
	COleDateTime currentDate;
	COleDateTime selectedDate;
	
	DPIX dpix;
	//int selectedDay;
	vector<wstring> week_days;
	int startDutyHour;
	struct VDate
	{
		int day;
		int month;
	} date[ROWS*COLS];
public:
	VCalendar()
		: 
		currentDate(COleDateTime::GetCurrentTime()),
		selectedDate(currentDate),
		week_days({ L"Пн", L"Вт", L"Ср", L"Чт", L"Пт", L"Сб", L"Вс" }),
		startDutyHour(9)
	{
		BuildValiables();
	}


	inline const COleDateTime& getSelectedDate() { return selectedDate; }
	inline void SetDate(const COleDateTime& newdate) { selectedDate = newdate; BuildValiables(); }
	void BuildValiables();

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

protected:
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);


	DECLARE_MESSAGE_MAP();
};

