
// MFCApplicationDlg.h: файл заголовка
//

#pragma once

#include <string>
#include <sstream>
#include <fstream>



#include "ChartDLLFunction.h"
// Диалоговое окно CMFCApplicationDlg
class CMFCApplicationDlg : public CDialog
{
// Создание
public:
	CMFCApplicationDlg(CWnd* pParent = NULL);	// стандартный конструктор

// Данные диалогового окна
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MFCAPPLICATION_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// поддержка DDX/DDV


// Реализация
protected:
	HICON m_hIcon;
	HBITMAP hbitmap;

	// Созданные функции схемы сообщений
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

	
	std::wstring LoadFile();
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedPict();
};
