#pragma once
#include "ugc.h"
#include <string>
#include <vector>
#include "PickerUpdater.h"

using namespace std;



struct VStartDutyTime
{
	int hour, minute;
};
class CDutyCalendar : public CDialogEx
{
	DECLARE_DYNAMIC(CDutyCalendar)
	enum { ROWS = 6, COLS = 7 };
public:
	CDutyCalendar(const CRect& rect, IDutyPickerUpdater * PickerUpdater, const COleDateTime& SelectedDate, const VStartDutyTime& StartDutyTime, CWnd* pParent = nullptr);   // стандартный конструктор
	virtual ~CDutyCalendar();

// Данные диалогового окна
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_DUTY };
#endif
	inline const COleDateTime& getSelectedDate() { return selectedDate; }
	inline void SetDate(const COleDateTime& newdate) { selectedDate = newdate; BuildValiables(); }
	void BuildValiables();

	static int GetDaysInMonth(COleDateTime dt)
	{
		dt.SetDate(dt.GetYear(), dt.GetMonth(), 1);
		COleDateTime dtNextMonth(dt);
		GetNextMonth(dtNextMonth);
		COleDateTimeSpan dtspan = dtNextMonth - dt;
		return static_cast<int>(dtspan.GetTotalDays());
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
	int X, Y;
	int Width, Height;
	int bitW, bitH;
	COleDateTime currentDate;
	COleDateTime selectedDate;
	IDutyPickerUpdater * pickerUpdater;
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
	
	afx_msg BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);    // поддержка DDX/DDV
	
	afx_msg void OnPaint();
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	DECLARE_MESSAGE_MAP()

};
