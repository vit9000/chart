#pragma once

#include <functional>
#include "LoadingAnimator.h"
// Диалоговое окно CWaitDlg
#define LOADING_ANIMATOR 1000

class CWaitDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CWaitDlg)
	std::function<void()> func;
	std::wstring msg;
public:
	CWaitDlg(CWnd* pParent, const std::wstring& message, const std::function<void()>& Func);   // стандартный конструктор
	virtual ~CWaitDlg();
	virtual BOOL OnInitDialog();
	CLoadingAnimator animator;
// Данные диалогового окна
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_WAIT_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // поддержка DDX/DDV
	afx_msg void OnTimer(UINT timer);
	DECLARE_MESSAGE_MAP()

};
