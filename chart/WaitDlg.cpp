// WaitDlg.cpp: файл реализации
//

#include "stdafx.h"
#include "chart.h"
#include "WaitDlg.h"
#include "afxdialogex.h"
#include <thread>
#include "ugc.h"

// Диалоговое окно CWaitDlg

IMPLEMENT_DYNAMIC(CWaitDlg, CDialogEx)

CWaitDlg::CWaitDlg(CWnd* pParent, const std::wstring& message, const std::function<void()>& Func)
	: CDialogEx(IDD_WAIT_DIALOG, pParent), msg(message), func(std::move(Func)), animator(this)
{

}

CWaitDlg::~CWaitDlg()
{
}

void CWaitDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BOOL CWaitDlg::OnInitDialog()
{
	CDialog::OnInitDialog();


	std::thread t([this]() 
	{
		animator.SetLoading(true); 
		func();
		animator.SetLoading(false);
		while (!animator.isReadyToExit()) {}
		RedrawWindow();
	});
	t.detach();


	return TRUE;  // return TRUE  unless you set the focus to a control
}

BEGIN_MESSAGE_MAP(CWaitDlg, CDialogEx)
	ON_WM_PAINT()
END_MESSAGE_MAP()


// Обработчики сообщений CWaitDlg
void CWaitDlg::OnPaint()
{

	if (animator.isReadyToExit()) 
		OnOK();

	CRect rect;
	GetClientRect(&rect);
	
	int Width = rect.Width();
	int Height = rect.Height();

	UGC ugc(GetDC(), Width, Height);
	ugc.SetDrawColor(255, 255, 255);
	ugc.Clear();
	ugc.SetDrawColor(100, 100, 100);
	ugc.SetTextSize(14);
	ugc.SetAlign(UGC::CENTER);
	ugc.DrawString(msg, Width / 2, ugc.getDPIX()(10));
	ugc.SetAlign(UGC::LEFT);

	animator.DrawLoadingAnimation(ugc, Width, Height);

	CWnd::OnPaint();
}