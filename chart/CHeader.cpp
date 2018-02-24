#include "stdafx.h"
#include "CHeader.h"


BEGIN_MESSAGE_MAP(CHeader, CWnd)
	ON_WM_SIZE()
	ON_WM_PAINT()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()

CHeader::CHeader()
{
}


CHeader::~CHeader()
{
}


void CHeader::OnPaint()
{
	CWnd::OnPaint();

	CRect rect;
	GetClientRect(&rect);
	UGC ugc(GetDC(), rect.Width(), rect.Height());
	ugc.SetDrawColor(37, 134, 15);
	ugc.Clear();

	ugc.SetDrawColor(255, 255, 255);
	int border = static_cast<int>(10*DPIX());
	
	int pos = border;
	int button_width = border * 2;

	int bH = Height / 12;
	for (int i = 0; i < 3; ++i)
	{
		ugc.FillRectangle(pos, bH*i*2+Height/4+bH/2, button_width, bH);
	}
	pos += button_width + border;
	int d = Height * 2 / 3;
	ugc.FillEllipse(pos, Height / 2 - d/2, d);
	ugc.SetDrawColor(37, 134, 15);
	ugc.SetAlign(UGC::CENTER);
	ugc.SetTextSize(20);
	ugc.DrawNumber(patient_number, pos + d / 2, Height / 2 - ugc.GetTextHeight() / 2);
	ugc.SetAlign(UGC::LEFT);

	
	pos += d + border;
	ugc.SetDrawColor(255, 255, 255);
	ugc.SetTextSize(14);
	ugc.DrawString(dbpatient.name, pos, Height / 2 - ugc.GetTextHeight() / 2);

	
	
	pos += Width/3 + border;
	pos += DrawSector(ugc, pos, L"�������", dbpatient.age) + border;
	pos += DrawSector(ugc, pos, L"���", dbpatient.weight) + border;
	pos += DrawSector(ugc, pos, L"������ �����", dbpatient.blood_type) + border;
	pos += DrawSector(ugc, pos, L"N �.�.", dbpatient.case_number) + border;
}
//------------------------------------------------------------
int CHeader::DrawSector(UGC& ugc, int x, const wstring& header, int content)
{
	wstringstream ss;
	ss << content;
	return DrawSector(ugc, x, header, ss.str());
}
int CHeader::DrawSector(UGC& ugc, int x, const wstring& header, const wstring& content)
{

	int width = ugc.GetTextWidth(header, 10);
	int temp = ugc.GetTextWidth(content, 14);
	if (temp > width) width = temp;
	ugc.SetAlign(UGC::CENTER);
	ugc.SetTextSize(10);
	ugc.DrawString(header, x+width/2, Height / 3 - ugc.GetTextHeight() / 2);
	ugc.SetTextSize(14);
	ugc.DrawString(content, x + width / 2, Height * 2 / 3 - ugc.GetTextHeight() / 2);
	ugc.SetAlign(UGC::LEFT);
	return width;
}


void CHeader::OnSize(UINT nType, int cx, int cy)
{
	SetBounds();

	
	RedrawWindow();
	CWnd::OnSize(nType, cx, cy);
}
//------------------------------------------------------------
void CHeader::LoadPatient(int index)
{
	DatabaseLoader db;
	dbpatient = db.getPatient(index);
	patient_number = index + 1;
	RedrawWindow();
}
//------------------------------------------------------------
void CHeader::SetBounds()
{

	CRect rect;
	GetWindowRect(&rect);
	ScreenToClient(&rect);
	rect.bottom -= rect.top;
	rect.right -= rect.left;

	Width = rect.Width();
	Height = rect.Height();


}
//------------------------------------------------------------
void CHeader::OnLButtonUp(UINT flags, CPoint point)
{
	int x = point.x;
	int y = point.y;
	if (ShowHider)
		ShowHider->setVisible(!ShowHider->getVisible());
}
//------------------------------------------------------------
void CHeader::OnLButtonDown(UINT flags, CPoint point)
{
	int x = point.x;
	int y = point.y;
	
}
//------------------------------------------------------------
void CHeader::OnMouseMove(UINT nFlags, CPoint point)
{

	int x = point.x;
	int y = point.y;
	
}
//------------------------------------------------------------