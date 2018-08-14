#include "stdafx.h"
#include "CHeader.h"
#include "ChartConfig.h"
#include "IDBConnector.h"

#include <ctime>
extern CChartConfig* config;

BEGIN_MESSAGE_MAP(CHeader, CWnd)
	ON_WM_SIZE()
	ON_WM_PAINT()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()



CHeader::~CHeader()
{
}


void CHeader::OnPaint()
{
	CWnd::OnPaint();
	UGC ugc(GetDC(), Width, Height);
	if (config->getChartType() == TIME_TYPE::ANESTH_CHART)
		ugc.SetDrawColor(100, 100, 255);
	else
		ugc.SetDrawColor(50, 200, 50);
	ugc.Clear();

	ugc.SetDrawColor(255, 255, 255);
	Draw(ugc);
}

void CHeader::Print(UGC& ugc, const CPrintDocument& pDoc)
{
	X = pDoc.getPxX();
	Y = pDoc.getPxPageY();
	Width = pDoc.getPxWidth();
	Height = pDoc.getPxColontitleHeight();

	ugc.SetDrawColor(0, 0, 0);
	//ugc.DrawRectangle(X, Y, Width, Height-DPIX()(2));
	
	int tempHeight = Height;
	Draw(ugc, false);
	Height = tempHeight;
}

void CHeader::Draw(UGC& ugc, bool DrawMenuButton)
{
	DPIX dpix;
	
	int border = dpix(15);

	int pos = border;
	int button_width = border * 2;

	int bH = Height / 12;
	if (DrawMenuButton)
	{
		for (int i = 0; i < 3; ++i)
		{
			ugc.FillRectangle(X + pos, Y + bH*i * 2 + Height / 4 + bH / 2, button_width, bH);
		}
		pos += button_width + border;
	}

	if (dbpatient.is_empty()) return;

	
	
	wstringstream ss(dbpatient[PatientInfo::DUTY]);
	for (int i = 0; i < 2; i++)
	{
		wstring date;
		wstring time;
		ss >> date >> time;
		pos += DrawSector2Lines(ugc, X + pos, Y, Height, date, time) + border;
		if (i == 0) ugc.DrawLine(X + pos - border, Y+Height / 2, X+pos, Y+Height / 2);
	}
	pos += border * 2;


	int y = Y;
	int height = int(Height*0.3);
	ugc.SetTextSize(14);
	ugc.SetBold(true);
	ugc.DrawString(dbpatient[PatientInfo::FIO], X + pos, y + height / 2 - ugc.GetTextHeight() / 2 + DPIX()(3));
	ugc.SetBold(false);

	y += height;
	height = Height - height;
	
	
	pos += DrawSector(ugc, X + pos, y, height, L"Возраст", dbpatient[PatientInfo::AGE]) + border;
	pos += DrawSector(ugc, X + pos, y, height, L"Вес", dbpatient[PatientInfo::WEIGHT]) + border;
	pos += DrawSector(ugc, X + pos, y, height, L"Рост", dbpatient[PatientInfo::HEIGHT]) + border;
	pos += DrawSector(ugc, X + pos, y, height, L"N и.б.", dbpatient[PatientInfo::NUM]) + border;
	pos += DrawSector(ugc, X + pos, y, height, L"NN", dbpatient[PatientInfo::ST_NUM]) + border;
	pos += DrawSector(ugc, X + pos, y, height, L"шифр", dbpatient[PatientInfo::CODE]) + border;
}
//------------------------------------------------------------
int CHeader::DrawSector(UGC& ugc, int x, int y, int height, const wstring& header, int content)
{
	wstringstream ss;
	ss << content;
	return DrawSector(ugc, x, y, height, header, ss.str());
}
//------------------------------------------------------------
int CHeader::DrawSector(UGC& ugc, int x, int y, int height, const wstring& header, const wstring& content)
{

	int width = ugc.GetTextWidth(header, DPIX()(10));
	int temp = ugc.GetTextWidth(content, DPIX()(14));
	if (temp > width) width = temp;
	ugc.SetAlign(UGC::CENTER);
	ugc.SetTextSize(10);
	ugc.DrawString(header, x+width/2, y+height / 3 - ugc.GetTextHeight() / 2);
	ugc.SetTextSize(14);
	ugc.DrawString(content, x + width / 2, y+height * 2 / 3 - ugc.GetTextHeight() / 2);
	ugc.SetAlign(UGC::LEFT);
	return width;
}

int CHeader::DrawSector2Lines(UGC& ugc, int x, int y, int height, const wstring& line1, const wstring& line2)
{

	int width = ugc.GetTextWidth(line1, DPIX()(12));
	int temp = ugc.GetTextWidth(line2, DPIX()(12));
	if (temp > width) width = temp;
	ugc.SetAlign(UGC::CENTER);
	ugc.SetTextSize(12);
	ugc.DrawString(line1, x + width / 2, y+height / 3 - ugc.GetTextHeight() / 2);
	ugc.SetTextSize(12);
	ugc.DrawString(line2, x + width / 2, y+height * 2 / 3 - ugc.GetTextHeight() / 2);
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
void CHeader::LoadPatient()
{
	dbpatient =	model->getPatient();
	
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

	X = 0;
	Y = 0;
	Width = rect.Width();
	Height = rect.Height();

}
//------------------------------------------------------------
void CHeader::OnLButtonUp(UINT flags, CPoint point)
{
	if(point.x<= DPIX()(15) * 4)
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