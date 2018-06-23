#pragma once

#include <windows.h>
#include <vector>
#include <string>
#include <utility>
#include <functional>
#include <map>
#include <thread>
#include <mutex>
#include "ugc.h"
#include "DutyCalendar.h"
#include "PickerUpdater.h"

struct DutyDateTime
{
	COleDateTime startDutyDatetime;//время начала дежурства
	COleDateTime endDutyDatetime;//время окончания дежурства

};


//-----------
class CDutyDatePicker : public CWnd, public IPickerUpdater
{
public:
	CDutyDatePicker();

	void ParseDateTime(const wstring& StartDutyTime);
	~CDutyDatePicker();
	
	void Update(const COleDateTime& newdate) override
	{
		currentDuty.startDutyDatetime = newdate;
		currentDuty.endDutyDatetime = currentDuty.startDutyDatetime + COleDateTimeSpan(1, 0, 0, 0);
		RedrawWindow();
	}
	
protected:
	int Width;
	int Height;
	bool isOpen;
	DutyDateTime currentDuty;//текущее дежурство
	
	inline void DatetimeToString(const COleDateTime& t, wstring& str) const { str = t.Format(_T("%d.%m.%Y %H:%M")).GetBuffer(); }
	inline void DateToString(const COleDateTime& t, wstring& str) const { str = t.Format(_T("%d.%m.%Y")).GetBuffer(); }
	inline void TimeToString(const COleDateTime& t, wstring& str) const { str = t.Format(_T("%H:%M")).GetBuffer(); }
	void DrawSector(UGC& ugc, const wstring& date, const wstring& time, int x, int width);
	int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnDestroy();

	
	
	DECLARE_MESSAGE_MAP();
private:
	
};
