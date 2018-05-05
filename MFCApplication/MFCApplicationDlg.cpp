
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
	CDC* pDC = GetDC();
	CBitmap *pBitmap = CBitmap::FromHandle(hbitmap);
	BITMAP bm;
	pBitmap->GetBitmap(&bm);
	CDC MemDC;
	MemDC.CreateCompatibleDC(pDC);
	MemDC.SelectObject(pBitmap);
	pDC->BitBlt(0, 0, bm.bmWidth, bm.bmHeight, &MemDC, 0, 0, SRCCOPY);


	//cdc->BitBlt()

	if (IsIconic())
	{
		CPaintDC dc(this); // контекст устройства для рисования

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Выравнивание значка по центру клиентского прямоугольника
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Нарисуйте значок
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// Система вызывает эту функцию для получения отображения курсора при перемещении
//  свернутого окна.
HCURSOR CMFCApplicationDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

typedef void(*ShowDialog)();


void CMFCApplicationDlg::OnBnClickedOk()
{
	// TODO: добавьте свой код обработчика уведомлений
	BOOL freeResult, runTimeLinkSuccess = FALSE;
	HINSTANCE dllHandle = NULL;
	ShowDialog ShowDialogPtr = NULL;

	//Load the dll and keep the handle to it
	dllHandle = LoadLibrary(L"chart.dll");

	// If the handle is valid, try to get the function address. 
	if (NULL != dllHandle)
	{
		//Get pointer to our function using GetProcAddress:
		ShowDialogPtr = (ShowDialog)GetProcAddress(dllHandle, "ShowDialog");

		// If the function address is valid, call the function. 
		//HINSTANCE hPrevInstance = AfxGetResourceHandle();
		//AfxSetResourceHandle(dllHandle);


		if (runTimeLinkSuccess = (NULL != ShowDialogPtr))
		{
			ShowDialogPtr();
		}

		//AfxSetResourceHandle(hPrevInstance);

		//Free the library:
		freeResult = FreeLibrary(dllHandle);
	}



	// TODO: добавьте свой код обработчика уведомлений
	//CDialog::OnOK();
}


void CMFCApplicationDlg::OnBnClickedCancel()
{
	
	CDialog::OnCancel();
}

typedef void(*GetHBITMAP)(HBITMAP *hbitmap);
void CMFCApplicationDlg::OnBnClickedPict()
{
	BOOL freeResult, runTimeLinkSuccess = FALSE;
	HINSTANCE dllHandle = NULL;
	GetHBITMAP GetHBITMAPPtr = NULL;

	//Load the dll and keep the handle to it
	dllHandle = LoadLibrary(L"chart.dll");

	// If the handle is valid, try to get the function address. 
	if (NULL != dllHandle)
	{
		//Get pointer to our function using GetProcAddress:
		GetHBITMAPPtr = (GetHBITMAP)GetProcAddress(dllHandle, "GetHBITMAP");

		// If the function address is valid, call the function. 
		//HINSTANCE hPrevInstance = AfxGetResourceHandle();
		//AfxSetResourceHandle(dllHandle);


		if (runTimeLinkSuccess = (NULL != GetHBITMAPPtr))
		{
			GetHBITMAPPtr(&hbitmap);
		}

		//AfxSetResourceHandle(hPrevInstance);

		//Free the library:
		freeResult = FreeLibrary(dllHandle);
	}

}
