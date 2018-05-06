
// MFCApplicationDlg.cpp: файл реализации
//

#include "stdafx.h"
#include "MFCApplication.h"
#include "MFCApplicationDlg.h"
#include "afxdialogex.h"



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
//-----------------------------------------------------------------
void CMFCApplicationDlg::OnBnClickedCancel()
{
	
	CDialog::OnCancel();
}
//-----------------------------------------------------------------
void CMFCApplicationDlg::OnBnClickedOk()
{
	std::wstring fileJSON_UTF16 = LoadFile();
	ChartDLL::function<void(const wchar_t*)> ShowDialog("ShowDialog");
	if(ShowDialog)
		ShowDialog(fileJSON_UTF16.c_str());
}
//-----------------------------------------------------------------
void CMFCApplicationDlg::OnBnClickedPict()
{
	std::wstring fileJSON_UTF16 = LoadFile();
	ChartDLL::function<void(HBITMAP*, const wchar_t*)> GetHBITMAP("GetHBITMAP");
	if(GetHBITMAP)
		GetHBITMAP(&hbitmap, fileJSON_UTF16.c_str());
	RedrawWindow();
}
//-----------------------------------------------------------------
#include <locale>
#include <codecvt>
std::wstring CMFCApplicationDlg::LoadFile()
{
	std::wifstream wif(L"structure_json.txt");
	wif.imbue(std::locale(std::locale::empty(), new std::codecvt_utf8<wchar_t, 0x10ffff, std::consume_header>));

	std::wstringstream wss;
	wss << wif.rdbuf();
	
	return wss.str();

}
//-----------------------------------------------------------------

