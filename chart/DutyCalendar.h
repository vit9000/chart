#pragma once
#include "ugc.h"
#include <string>
#include "VCalendar.h"


using namespace std;




class CDutyCalendar : public CDialogEx, public ICloser
{
	DECLARE_DYNAMIC(CDutyCalendar)

public:
	CDutyCalendar(const CRect& rect, IPickerUpdater * PickerUpdater, CWnd* pParent = nullptr);   // стандартный конструктор
	virtual ~CDutyCalendar();

	void Close() override { CDialogEx::OnOK(); }
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
	afx_msg void OnPaint();
	virtual void DoDataExchange(CDataExchange* pDX);    // поддержка DDX/DDV
	
	DECLARE_MESSAGE_MAP()


	

};
