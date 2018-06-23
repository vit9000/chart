#pragma once
#include <string>
#include <vector>
#include "ugc.h"
#include "PickerUpdater.h"
using namespace std;
struct VStartDutyTime
{
	int hour, minute;
};
// ƒиалоговое окно CDutyCalendar
class VCalendar : public CWnd
{
	enum { ROWS = 6, COLS = 7 };
	int X, Y, Width, Height;
	int bitW, bitH;
	COleDateTime currentDate;
	COleDateTime selectedDate;
	IPickerUpdater * pickerUpdater;
	DPIX dpix;
	//int selectedDay;
	vector<wstring> week_days;
	vector<wstring> monthes;
	
	VStartDutyTime startDutyTime;
	struct VDate
	{
		int day;
		int month;
	} date[ROWS*COLS];
public:
	VCalendar(IPickerUpdater * PickerUpdater, const COleDateTime& SelectedDate, const VStartDutyTime& StartDutyTime)
		: 
		pickerUpdater(PickerUpdater),
		selectedDate(SelectedDate),
		week_days({ L"ѕн", L"¬т", L"—р", L"„т", L"ѕт", L"—б", L"¬с" }),
		monthes({L"яЌ¬ј–№", L"‘≈¬–јЋ№", L"ћј–“", L"јѕ–≈Ћ№", L"ћј…", L"»ёЌ№", L"»ёЋ№", L"ј¬√”—“", L"—≈Ќ“яЅ–№", L"ќ “яЅ–№", L"ЌќяЅ–№", L"ƒ≈ јЅ–№"}),
		startDutyTime(StartDutyTime)
	{
		currentDate = COleDateTime::GetCurrentTime();
		// если не достигнуто врем€ дежурства, значит дежурство предыдущее
		if (currentDate.GetHour() < startDutyTime.hour ||
			(currentDate.GetHour() == startDutyTime.hour && currentDate.GetMinute() < startDutyTime.minute))
			currentDate -= COleDateTimeSpan(1,0,0,0);
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
	afx_msg void OnKillFocus(CWnd* pNewWnd);

	DECLARE_MESSAGE_MAP();
};

