
// MFCApplicationDlg.cpp: файл реализации
//

#include "stdafx.h"
#include "MFCApplication.h"
#include "MFCApplicationDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// Диалоговое окно CMFCApplicationDlg



CMFCApplicationDlg::CMFCApplicationDlg(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_MFCAPPLICATION_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMFCApplicationDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CMFCApplicationDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &CMFCApplicationDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CMFCApplicationDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDPICT, &CMFCApplicationDlg::OnBnClickedPict)
END_MESSAGE_MAP()


// Обработчики сообщений CMFCApplicationDlg

BOOL CMFCApplicationDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Задает значок для этого диалогового окна.  Среда делает это автоматически,
	//  если главное окно приложения не является диалоговым
	SetIcon(m_hIcon, TRUE);			// Крупный значок
	SetIcon(m_hIcon, FALSE);		// Мелкий значок

	// TODO: добавьте дополнительную инициализацию

	return TRUE;  // возврат значения TRUE, если фокус не передан элементу управления
}

// При добавлении кнопки свертывания в диалоговое окно нужно воспользоваться приведенным ниже кодом,
//  чтобы нарисовать значок.  Для приложений MFC, использующих модель документов или представлений,
//  это автоматически выполняется рабочей областью.

void CMFCApplicationDlg::OnPaint()
{
	CDialog::OnPaint();

	CDC* pDC = GetDC();
	CBitmap *pBitmap = CBitmap::FromHandle(hbitmap);
	BITMAP bm;
	pBitmap->GetBitmap(&bm);
	CDC MemDC;
	MemDC.CreateCompatibleDC(pDC);
	MemDC.SelectObject(pBitmap);
	pDC->BitBlt(0, 0, bm.bmWidth, bm.bmHeight, &MemDC, 0, 0, SRCCOPY);
	
	

}

// Система вызывает эту функцию для получения отображения курсора при перемещении
//  свернутого окна.
HCURSOR CMFCApplicationDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}




void CMFCApplicationDlg::OnBnClickedCancel()
{
	
	CDialog::OnCancel();
}


void CMFCApplicationDlg::OnBnClickedOk()
{
	ChartDLLFunction<void()> ShowDialog("ShowDialog");
	if(ShowDialog())
		ShowDialog()();
}
void CMFCApplicationDlg::OnBnClickedPict()
{
	ChartDLLFunction<void(HBITMAP *hbitmap)> GetHBITMAP("GetHBITMAP");
	if(GetHBITMAP())
		GetHBITMAP()(&hbitmap);
	RedrawWindow();
}

