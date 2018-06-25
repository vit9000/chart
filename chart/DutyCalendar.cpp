// DutyCalendar.cpp: файл реализации
//

#include "stdafx.h"
#include "chart.h"
#include "DutyCalendar.h"
#include "afxdialogex.h"
#include "ugc.h"


// Диалоговое окно CDutyCalendar

IMPLEMENT_DYNAMIC(CDutyCalendar, CDialogEx)

CDutyCalendar::CDutyCalendar(const CRect& rect, IDutyPickerUpdater * PickerUpdater, const COleDateTime& SelectedDate, const VStartDutyTime& StartDutyTime, CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_DUTY, pParent), 
	//calendar(PickerUpdater, SelectedDate, StartDutyTime),
	X(rect.left), 
	Y(rect.top), 
	Width(rect.Width()), 
	Height(rect.Height()),
	pickerUpdater(PickerUpdater),
	selectedDate(SelectedDate),
	week_days({ L"Пн", L"Вт", L"Ср", L"Чт", L"Пт", L"Сб", L"Вс" }),
	monthes({ L"ЯНВАРЬ", L"ФЕВРАЛЬ", L"МАРТ", L"АПРЕЛЬ", L"МАЙ", L"ИЮНЬ", L"ИЮЛЬ", L"АВГУСТ", L"СЕНТЯБРЬ", L"ОКТЯБРЬ", L"НОЯБРЬ", L"ДЕКАБРЬ" }),
	startDutyTime(StartDutyTime)
{
	currentDate = COleDateTime::GetCurrentTime();
	// если не достигнуто время дежурства, значит дежурство предыдущее
	if (currentDate.GetHour() < startDutyTime.hour ||
		(currentDate.GetHour() == startDutyTime.hour && currentDate.GetMinute() < startDutyTime.minute))
		currentDate -= COleDateTimeSpan(1, 0, 0, 0);
	BuildValiables();
}

CDutyCalendar::~CDutyCalendar()
{
}

void CDutyCalendar::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDutyCalendar, CDialogEx)
	ON_WM_PAINT()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_DESTROY()
	ON_WM_CREATE()
	ON_WM_KILLFOCUS()
END_MESSAGE_MAP()
void CDutyCalendar::OnKillFocus(CWnd* pNewWnd)
{
	CWnd::OnKillFocus(pNewWnd);
	pickerUpdater->setOpened(false);
	CDialogEx::OnOK();
}
//-------------------------------------------------------------------------


BOOL CDutyCalendar::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	DPIX dpix;
	
	
	SetWindowPos(NULL, -1, -1, Width, Height, SWP_NOMOVE | SWP_NOACTIVATE | SWP_NOZORDER);
	SetWindowPos(NULL, X, Y, 50, 50, SWP_NOSIZE | SWP_NOZORDER);

	ModifyStyleEx(WS_EX_APPWINDOW, 0);
	ModifyStyleEx(0, WS_EX_TOOLWINDOW);


	CRect rect;
	GetClientRect(&rect);
	X = dpix(25);
	Y = dpix(25);
	Width = rect.Width() - X * 2;
	Height = rect.Height() - Y * 2;
	bitW = (Width / COLS);
	bitH = (Height / (1 + ROWS));
	Width = bitW * COLS;
	Height = bitH * (1 + ROWS);
	X = (rect.Width() - Width) / 2;

	pickerUpdater->setOpened(true);
	return TRUE;
}

//-------------------------------------------------------------------------
void CDutyCalendar::BuildValiables()
{
	COleDateTime currentDate = getSelectedDate();
	COleDateTime prevMonth(currentDate);
	GetPrevMonth(prevMonth);
	COleDateTime nextMonth(currentDate);
	GetNextMonth(nextMonth);

	currentDate.SetDate(currentDate.GetYear(), currentDate.GetMonth(), 1);
	int startDayOfWeek = currentDate.GetDayOfWeek() - 1;
	if (startDayOfWeek <= 0) startDayOfWeek += 7;
	int monthDaysPrev = COleDateTimeSpan(currentDate - prevMonth).GetTotalDays();;
	int monthDaysCurr = COleDateTimeSpan(nextMonth - currentDate).GetTotalDays();

	int day = monthDaysPrev - (startDayOfWeek - 2);
	int month = -1;
	for (int R = 0; R<ROWS; R++)
		for (int C = 0; C < COLS; C++)
		{
			if ((month == -1 && day>monthDaysPrev) || (month == 0 && day > monthDaysCurr))
			{
				day = 1;
				month++;
			}
			int pos = COLS * R + C;
			date[pos] = { day, currentDate.GetMonth() + month };
			day++;
		}


}
//-------------------------------------------------------------------------
void CDutyCalendar::OnPaint()
{
	CWnd::OnPaint();

	CRect rect;
	GetClientRect(rect);
	UGC ugc(GetDC(), rect.Width(), rect.Height());



	ugc.SetDrawColor(0, 126, 186);
	ugc.FillRectangle(0, 0, rect.Width(), bitH);
	ugc.SetDrawColor(255, 255, 255);

	ugc.SetTextSize(12);
	int TextHeight12 = ugc.GetTextHeight(12);
	int TextHeight10 = ugc.GetTextHeight(11);

	{
		int h = TextHeight12;
		int top = (bitH - h) / 2;
		int Xpos = bitW / 2;
		ugc.FillTriangle(
			Xpos, top,
			Xpos, bitH - top,
			Xpos - h / 2, bitH / 2);

		Xpos = rect.Width() - Xpos;
		ugc.FillTriangle(
			Xpos, top,
			Xpos, bitH - top,
			Xpos + h / 2, bitH / 2);
	}



	ugc.SetAlign(UGC::CENTER);
	ugc.DrawString(monthes[selectedDate.GetMonth() - 1] + L" " + selectedDate.Format(L"%Y").GetBuffer(), rect.Width() / 2, bitH / 2 - TextHeight12 / 2);
	ugc.SetAlign(UGC::LEFT);

	int Xi = X;
	int Yi = Y + bitH;
	// выходные дни
	ugc.SetDrawColor(255, 255, 144);
	ugc.FillRectangle(Xi + bitW * 5, Yi, bitW * 2, bitH*ROWS);

	int dpi_one = dpix(1);
	int dpi_h = dpix(6);

	for (int R = 0; R<ROWS; R++)
		for (int C = 0; C < COLS; C++)
		{
			int pos = COLS * R + C;
			VDate& d = date[pos];
			int next_day = (pos + 1 >= COLS * ROWS) ? d.day + 1 : date[pos + 1].day; // следующий день для прорисовки

			int _x = Xi + C * bitW;
			int _y = Yi + R * bitH;
			// рамка ячейки

			ugc.SetDrawColor(190, 190, 190);
			ugc.DrawRectangle(_x, _y, bitW, bitH);

			// дни недели
			if (R == 0)
			{
				ugc.SetTextSize(12);
				ugc.SetAlign(UGC::CENTER);
				ugc.SetDrawColor(0, 126, 186);
				ugc.DrawString(week_days.at(C), _x + bitW / 2, Yi - TextHeight12);
				ugc.SetAlign(UGC::LEFT);
			}

			// рисуем выбранной даты рамочку
			if (selectedDate.GetMonth() == d.month)
			{
				if (d.day == selectedDate.GetDay())
				{
					ugc.SetDrawColor(255, 190, 0);
					ugc.FillRectangle(_x + dpi_one, _y + dpi_one, bitW - dpi_one, bitH - dpi_one);
				}
				else if (d.month == currentDate.GetMonth() && d.day == currentDate.GetDay())
				{
					ugc.SetDrawColor(0, 255, 0);
					ugc.FillRectangle(_x + dpi_one, _y + dpi_one, bitW - dpi_one, bitH - dpi_one);
				}


				ugc.SetDrawColor(0, 126, 186);
			}
			else// если не текущий месяц рисуем сереньким
				ugc.SetDrawColor(190, 190, 190);


			// рисуем день
			ugc.SetTextSize(12);
			ugc.SetBold(true);
			ugc.DrawNumber(d.day, _x, _y);
			ugc.SetBold(false);
			ugc.SetAlign(UGC::RIGHT);
			ugc.SetTextSize(10);
			ugc.DrawNumber(next_day, _x + bitW, _y + bitW - TextHeight10);
			ugc.SetAlign(UGC::LEFT);
		}

}
//-------------------------------------------------------------------------
void CDutyCalendar::OnLButtonDown(UINT nFlags, CPoint point)
{
	//CWnd::OnLButtonDown(nFlags, point);
}
//-------------------------------------------------------------------------
void CDutyCalendar::OnMouseMove(UINT nFlags, CPoint point)
{
	//CDialogEx::OnMouseMove(nFlags, point);
}
//-------------------------------------------------------------------------
void CDutyCalendar::OnLButtonUp(UINT nFlags, CPoint point)
{
	CRect rect;
	GetClientRect(rect);

	int mX = point.x;
	int mY = point.y;
	if (mY <= bitH)
	{
		int type = -1;
		if (mX <= bitW) type = 1;
		else if (mX >= rect.Width() - bitW) type = 2;
		if (type > 0)
		{
			(type == 1) ? GetPrevMonth(selectedDate) : GetNextMonth(selectedDate);
			BuildValiables();
			RedrawWindow();
		}

		return;
	}

	mX = point.x - X;
	mY = point.y - Y - bitH;

	if (mX >= 0 && mX <= Width && Y >= 0 && Y <= Height)
	{
		int row = mY / bitH; // получаем строку
		int col = mX / bitW; // получаем ячейку
		if (row < 0 || row >= ROWS) return;
		if (col < 0 || col >= COLS) return;
		int pos = COLS * row + col;
		VDate& d = date[pos];

		COleDateTime dt = COleDateTime::GetCurrentTime();
		dt.SetDateTime(dt.GetYear(), d.month, d.day, startDutyTime.hour, startDutyTime.minute, 0);
		SetDate(dt);
		RedrawWindow();
		if (pickerUpdater)
			pickerUpdater->Update(selectedDate);
	}

}
//------