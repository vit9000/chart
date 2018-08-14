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
	ugc.FillRectangle(X, Y+Height-DPIX()(3), Width, DPIX()(3));
	
	int tempHeight = Height;
	smallTextSize -= 1;
	bigTextSize -= 2;

	Draw(ugc, false);

	Height = tempHeight;
	smallTextSize += 1;
	bigTextSize += 2;
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

	ugc.SetTextSize(smallTextSize+2);
	ugc.DrawString(dbpatient[PatientInfo::DUTY], X + pos, Y + Height / 3 - ugc.GetTextHeight() / 2);

	
	ugc.SetTextSize(bigTextSize);
	ugc.SetBold(true);
	ugc.DrawString(dbpatient[PatientInfo::FIO], X + pos, Y + Height * 2/ 3 - ugc.GetTextHeight() / 2);
	ugc.SetBold(false);
	pos = Width;//ugc.GetTextWidth(dbpatient[PatientInfo::FIO])+border;

	pos -= DrawSector(ugc, X + pos, Y, Height, L"шифр", dbpatient[PatientInfo::CODE]) + border;
	pos -= DrawSector(ugc, X + pos, Y, Height, L"N и.б.", dbpatient[PatientInfo::NUM]) + border;
	pos -= DrawSector(ugc, X + pos, Y, Height, L"NN", dbpatient[PatientInfo::ST_NUM]) + border;
	

	if (DrawMenuButton)
	{
		DrawPatientParameters(ugc, X+pos, Y, Height);
	}
	
}
//------------------------------------------------------------
void CHeader::DrawPatientParameters(UGC& ugc, int x, int y, int height)
{
	int border = DPIX()(8);
	int pos = x;
	pos -= DrawSector(ugc, pos, y, height, L"Вес", dbpatient[PatientInfo::WEIGHT]) + border;
	pos -= DrawSector(ugc, pos, y, height, L"Рост", dbpatient[PatientInfo::HEIGHT]) + border;
	pos -= DrawSector(ugc, pos, y, height, L"Возраст", dbpatient[PatientInfo::AGE]) + border;
}



int CHeader::DrawSector(UGC& ugc, int x, int y, int height, const wstring& header, int content)
{
	wstringstream ss;
	ss << content;
	return DrawSector(ugc, x, y, height, header, ss.str());
}
//------------------------------------------------------------
int CHeader::DrawSector(UGC& ugc, int x, int y, int height, const wstring& header, const wstring& content)
{

	int width = ugc.GetTextWidth(header, DPIX()(smallTextSize));
	int temp = ugc.GetTextWidth(content, DPIX()(bigTextSize));
	if (temp > width) width = temp;
	ugc.SetAlign(UGC::CENTER);
	ugc.SetTextSize(smallTextSize);
	ugc.DrawString(header, x-width/2, y+height / 3 - ugc.GetTextHeight() / 2);
	ugc.SetTextSize(bigTextSize);
	ugc.DrawString(content, x - width / 2, y+height * 2 / 3 - ugc.GetTextHeight() / 2);
	ugc.SetAlign(UGC::LEFT);
	return width;
}
//------------------------------------------------------------
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