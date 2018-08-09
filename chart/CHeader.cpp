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
	const DPIX& dpix = DPIX();



	ugc.SetDrawColor(255, 255, 255);
	int border = dpix(15);

	int pos = border;
	int button_width = border * 2;

	int bH = Height / 12;
	for (int i = 0; i < 3; ++i)
	{
		ugc.FillRectangle(pos, bH*i * 2 + Height / 4 + bH / 2, button_width, bH);
	}
	pos += button_width + border;

	if (dbpatient.is_empty()) return;

	/*
	int d = Height * 2 / 3;
	ugc.FillEllipse(pos, Height / 2 - d/2, d);
	ugc.SetDrawColor(Gdiplus::Color::ForestGreen);
	ugc.SetAlign(UGC::CENTER);
	ugc.SetTextSize(20);
	ugc.DrawNumber(patient_number, pos + d / 2, Height / 2 - ugc.GetTextHeight() / 2);
	ugc.SetAlign(UGC::LEFT);

	pos += d + border;
	*/
	ugc.SetDrawColor(255, 255, 255);
	
	wstringstream ss(dbpatient[PatientInfo::DUTY]);
	for (int i = 0; i < 2; i++)
	{
		wstring date;
		wstring time;
		ss >> date >> time;
		pos += DrawSector2Lines(ugc, pos, date, time) + border;
		if (i == 0) ugc.DrawLine(pos - border, Height / 2, pos, Height / 2);
	}
	pos += border * 2;

	ugc.SetTextSize(15);
	ugc.SetBold(true);
	ugc.DrawString(dbpatient[PatientInfo::FIO], pos, Height / 2 - ugc.GetTextHeight() / 2);
	ugc.SetBold(false);
	

	pos += ugc.GetTextWidth(dbpatient[PatientInfo::FIO]) + border * 2;

	pos += DrawSector(ugc, pos, L"Возраст", dbpatient[PatientInfo::AGE]) + border;
	pos += DrawSector(ugc, pos, L"Вес", dbpatient[PatientInfo::WEIGHT]) + border;
	pos += DrawSector(ugc, pos, L"Рост", dbpatient[PatientInfo::HEIGHT]) + border;
	pos += DrawSector(ugc, pos, L"N и.б.", dbpatient[PatientInfo::NUM]) + border;
	pos += DrawSector(ugc, pos, L"NN", dbpatient[PatientInfo::ST_NUM]) + border;
	pos += DrawSector(ugc, pos, L"шифр", dbpatient[PatientInfo::CODE]) + border;

	
	
}
//------------------------------------------------------------
int CHeader::DrawSector(UGC& ugc, int x, const wstring& header, int content)
{
	wstringstream ss;
	ss << content;
	return DrawSector(ugc, x, header, ss.str());
}
//------------------------------------------------------------
int CHeader::DrawSector(UGC& ugc, int x, const wstring& header, const wstring& content)
{

	int width = ugc.GetTextWidth(header, DPIX()(10));
	int temp = ugc.GetTextWidth(content, DPIX()(14));
	if (temp > width) width = temp;
	ugc.SetAlign(UGC::CENTER);
	ugc.SetTextSize(10);
	ugc.DrawString(header, x+width/2, Height / 3 - ugc.GetTextHeight() / 2);
	ugc.SetTextSize(14);
	ugc.DrawString(content, x + width / 2, Height * 2 / 3 - ugc.GetTextHeight() / 2);
	ugc.SetAlign(UGC::LEFT);
	return width;
}

int CHeader::DrawSector2Lines(UGC& ugc, int x, const wstring& line1, const wstring& line2)
{

	int width = ugc.GetTextWidth(line1, DPIX()(12));
	int temp = ugc.GetTextWidth(line2, DPIX()(12));
	if (temp > width) width = temp;
	ugc.SetAlign(UGC::CENTER);
	ugc.SetTextSize(12);
	ugc.DrawString(line1, x + width / 2, Height / 3 - ugc.GetTextHeight() / 2);
	ugc.SetTextSize(12);
	ugc.DrawString(line2, x + width / 2, Height * 2 / 3 - ugc.GetTextHeight() / 2);
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