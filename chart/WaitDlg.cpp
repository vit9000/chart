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
	: CDialogEx(IDD_WAIT_DIALOG, pParent), msg(message), func(std::move(Func))
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
	CRect rect;
	GetClientRect(&rect);
	
	animator.Create(NULL, NULL, WS_VISIBLE, rect, this, LOADING_ANIMATOR);
	animator.setMessage(msg);
	thread t([this]() 
	{
		animator.setLoading(true);
		func();
		animator.setLoading(false);
	});
	t.detach();

	SetTimer(2, 30, NULL);
	return TRUE;  // return TRUE  unless you set the focus to a control
}

BEGIN_MESSAGE_MAP(CWaitDlg, CDialogEx)
	ON_WM_TIMER()
END_MESSAGE_MAP()


afx_msg void CWaitDlg::OnTimer(UINT timer)
{
	if (!animator.isLoading())
	{
		KillTimer(2);
		OnOK();
	}
}

