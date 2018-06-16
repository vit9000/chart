#include "stdafx.h"
#include "CInPlaceEditbox.h"


CInPlaceEditbox::CInPlaceEditbox(std::function<void(const std::wstring&)> CallBackFunction, const std::wstring& defaultValue, bool OnlyDigit, std::function<void()> Next)
	: callBack(CallBackFunction), 
	next(Next),
	m_bESC(false), 
	m_sInitText(defaultValue.c_str()), 
	font(nullptr),
	is_cancel(false),
	allow_next(false),
	digit_only(OnlyDigit)
{
}


CInPlaceEditbox::~CInPlaceEditbox()
{
	if(font)
		delete font;
}


BOOL CInPlaceEditbox::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_RETURN)
		{
			
			allow_next = true;
			DestroyWindow();
			return TRUE;
		}
		else if (pMsg->wParam == VK_ESCAPE)
		{
			is_cancel = true;
			DestroyWindow();
			return TRUE;
		}
		else if (pMsg->wParam == VK_DELETE
			|| GetKeyState(VK_CONTROL)
			)
		{
			::TranslateMessage(pMsg);
			::DispatchMessage(pMsg);
			return TRUE;		    	// DO NOT process further
		}
	}

	return CEdit::PreTranslateMessage(pMsg);
}

BEGIN_MESSAGE_MAP(CInPlaceEditbox, CEdit)
	//{{AFX_MSG_MAP(CInPlaceEdit)
	ON_WM_KILLFOCUS()
	ON_WM_NCDESTROY()
	ON_WM_CHAR()
	ON_WM_CREATE()
	
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CInPlaceEditbox::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	if (digit_only)
	{
		CString str;
		GetWindowText(str);
		int point_count = 0;
		for(int i=0; i<str.GetLength(); i++)
			if (str[i] == L',' || str[i] == L'.')
			{
				point_count++;
				break;
			}

		
		if((!(nChar == L',' || nChar == L'.' || (nChar >= L'0' && nChar <= L'9') || nChar == VK_BACK || nChar == VK_DELETE)) ||
			((nChar == L',' || nChar == L'.') && point_count!=0))
		{
			MessageBeep(MB_OK);
			return;
		}
		
	}
	
	CEdit::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CInPlaceEditbox::OnKillFocus(CWnd* pNewWnd)
{
	CEdit::OnKillFocus(pNewWnd);

	if (!is_cancel)
	{
		CString str;
		GetWindowText(str);
		if (!str.IsEmpty() && digit_only)
			str.Replace(L',', L'.');

		if(callBack)
			callBack(str.GetBuffer());
		if (allow_next && next)
			next();
	}

	DestroyWindow();
}

void CInPlaceEditbox::OnNcDestroy()
{
	CEdit::OnNcDestroy();

	delete this;
}

int CInPlaceEditbox::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CEdit::OnCreate(lpCreateStruct) == -1)
		return -1;

	font = new CFont;
	RECT r;
	GetClientRect(&r);
	font->CreateFontW(r.bottom-r.top, 0, 0, 0, FW_SEMIBOLD, 0, 0, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS
		, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, _T("Calibri"));
	
	SetFont(font);

	SetWindowText(m_sInitText);
	SetFocus();
	SetSel(0, -1);
	return 0;
}