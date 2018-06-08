#include "stdafx.h"
#include "ugc.h"

Color convertColor(DWORD _color)
{
	return Color(GetRValue(_color), GetGValue(_color), GetBValue(_color));
};



UGC::UGC(Bitmap* bitmap)//, int width, int height) 
{

	doublebuffering = false;
	Init(bitmap->GetWidth(), bitmap->GetHeight());
	g = Gdiplus::Graphics::FromImage(bitmap);
	StartDraw();
};
//---------------------------------------- 
UGC::UGC(CDC* pDC, int width, int height)
{

	doublebuffering = true;
	Init(width, height);

	hdc = pDC->m_hDC;

	Memhdc = CreateCompatibleDC(hdc);
	Membitmap = CreateCompatibleBitmap(hdc, width, height);
	SelectObject(Memhdc, Membitmap);


	g = new Gdiplus::Graphics(Memhdc);
	StartDraw();
}
//--------------------------------------------
UGC::UGC(HDC& hdc, int width, int height)
{

	doublebuffering = false;
	Init(width, height);
	//hdc = _hdc; 

	g = new Gdiplus::Graphics(hdc);
	StartDraw();
};
//---------------------------------------- 
void UGC::Init(int width, int height)
{
	dpix = DPIX();
	Bold = false;
	WIDTH = width,
		HEIGHT = height;
	TextSize = 12;
	RealTextSize = 12;
	//CENTER = 0;; 
	//LEFT = 1;//Qt::AlignLeft; 
	//RIGHT = 2;//Qt::AlignRight; 
	align = LEFT;//LEFT; 
				 //FontName = L"Times New Roman"; 
	FontName = L"Calibri";
}
//---------------------------------------- 
UGC::~UGC()
{
	EndDraw();

}
//---------------------------------------- 
void UGC::StartDraw()
{
	g->SetPageUnit(Gdiplus::UnitPixel);
	//g->SetSmoothingMode(Gdiplus::SmoothingMode::SmoothingModeAntiAlias); 

	SetDrawColor(255, 255, 255);
	FillRectangle(0, 0, WIDTH, HEIGHT);
}
//---------------------------------------- 
void UGC::EndDraw()
{
	delete g;
	g = NULL;
	if (doublebuffering)
	{
		BitBlt(hdc, 0, 0, WIDTH, HEIGHT, Memhdc, 0, 0, SRCCOPY);
		DeleteObject(Membitmap);
		DeleteDC(Memhdc);
		DeleteDC(hdc);
	}
}
//---------------------------------------- 

void UGC::Clear()
{
	g->Clear(color);
}
//--------------------------------------------------------------------- 

void UGC::DrawBitmap(Gdiplus::Bitmap* bitmap, int x, int y, int width, int height)
{
	g->DrawImage(bitmap, x, y, width, height);
}
//--------------------------------------------------------------------- 

void UGC::SetSpecColor(int color)
{
	switch (color)
	{
	case 0:
		//SetDrawColor(254, 234, 6); 
		SetDrawColor(217, 197, 25);
		break;
	case 1:
		SetDrawColor(235, 144, 55);
		break;
	case 2:
		SetDrawColor(116, 199, 229);
		break;
	case 3:
		SetDrawColor(238, 165, 157);
		break;

	case 4:
		SetDrawColor(120, 120, 120);
		break;

	case 5:
		SetDrawColor(239, 103, 9);
		break;

	case 6:
		SetDrawColor(216, 13, 6);
		break;

	case 7:
		SetDrawColor(94, 94, 94);
		break;

	case 8:
		SetDrawColor(62, 61, 61);
		break;

	}
}

void UGC::SetDrawColor(int A, int R, int G, int B)
{
	color = Color(A, R, G, B);
};
//--------------------------------------- 
void UGC::SetDrawColor(int R, int G, int B)
{
	color = Color(R, G, B);
};
//--------------------------------------- 
void UGC::SetDrawColor(Color _color)
{
	color = _color;
};
//--------------------------------------- 
void UGC::SetDrawColor(DWORD _color)
{
	color = convertColor(_color);
}
//--------------------------------------- 
void UGC::DrawPoint(int x, int y, int size)
{
	size = static_cast<int>(getDPIX()*size);
	FillRectangle(x, y, size, size);
}

void UGC::FillTriangle(int x1, int y1, int x2, int y2, int x3, int y3)
{
	Gdiplus::SolidBrush blackBrush(color);
	Gdiplus::PointF point1((Gdiplus::REAL)x1, (Gdiplus::REAL)y1);
	Gdiplus::PointF point2((Gdiplus::REAL)x2, (Gdiplus::REAL)y2);
	Gdiplus::PointF point3((Gdiplus::REAL)x3, (Gdiplus::REAL)y3);
	Gdiplus::PointF points[3] = { point1, point2, point3 };
	g->SetSmoothingMode(Gdiplus::SmoothingMode::SmoothingModeAntiAlias);
	g->FillPolygon(&blackBrush, points, 3);
	g->SetSmoothingMode(Gdiplus::SmoothingMode::SmoothingModeDefault);

};

void UGC::DrawTriangle(int x1, int y1, int x2, int y2, int x3, int y3, int line_width)
{
	Gdiplus::Pen penCurrent(color);
	penCurrent.SetWidth((Gdiplus::REAL)(line_width*getDPIX()));
	Gdiplus::PointF point1((Gdiplus::REAL)x1, (Gdiplus::REAL)y1);
	Gdiplus::PointF point2((Gdiplus::REAL)x2, (Gdiplus::REAL)y2);
	Gdiplus::PointF point3((Gdiplus::REAL)x3, (Gdiplus::REAL)y3);
	Gdiplus::PointF points[3] = { point1, point2, point3 };
	g->SetSmoothingMode(Gdiplus::SmoothingMode::SmoothingModeAntiAlias);
	g->DrawPolygon(&penCurrent, points, 3);
	g->SetSmoothingMode(Gdiplus::SmoothingMode::SmoothingModeDefault);

};

//------------------------------------------------------- 
void UGC::DrawLine(int x1, int y1, int x2, int y2, int line_width)
{
	Gdiplus::Pen penCurrent(color);
	penCurrent.SetWidth((Gdiplus::REAL)(line_width*getDPIX()));
	g->DrawLine(&penCurrent, x1, y1, x2, y2);

};
void UGC::DrawLineAntialiased(int x1, int y1, int x2, int y2, int line_width)
{

	g->SetSmoothingMode(Gdiplus::SmoothingMode::SmoothingModeAntiAlias);
	DrawLine(x1, y1, x2, y2, line_width);
	g->SetSmoothingMode(Gdiplus::SmoothingMode::SmoothingModeDefault);

};
//------------------------------------------------------- 
void UGC::DrawDashLine(int x1, int y1, int x2, int y2, int line_width)
{
	Gdiplus::Pen penCurrent(color);
	penCurrent.SetWidth((Gdiplus::REAL)(line_width*getDPIX()));
	penCurrent.SetDashStyle(Gdiplus::DashStyleDash);
	g->DrawLine(&penCurrent, x1, y1, x2, y2);

};
//------------------------------------------------------- 
void UGC::DrawDotLine(int x1, int y1, int x2, int y2, int line_width)
{
	Gdiplus::Pen penCurrent(color);
	penCurrent.SetWidth((Gdiplus::REAL)(line_width*getDPIX()));
	penCurrent.SetDashStyle(Gdiplus::DashStyleDot);
	g->DrawLine(&penCurrent, x1, y1, x2, y2);

};

void UGC::DrawPunktRectangle(int x, int y, int width, int height, int line_width)
{


};

//------------------------------------------------------- 
void UGC::DrawRectangle(int x, int y, int width, int height, int line_width)
{

	Gdiplus::Pen penCurrent(color);
	penCurrent.SetWidth((Gdiplus::REAL)line_width);
	g->DrawRectangle(&penCurrent, x, y, width, height);

};
//------------------------------------------------------- 
void UGC::FillRectangle(int x, int y, int width, int height, bool finishdraw)
{
	Gdiplus::SolidBrush mySolidBrush(color);

	g->FillRectangle(&mySolidBrush, x, y, width, height);

};
//------------------------------------------------------- 
void UGC::FillObj(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4)
{

};
//------------------------------------------------------- 
void UGC::DrawEllipse(int x, int y, int D, int line_width)
{
	Gdiplus::Pen penCurrent(color);
	penCurrent.SetWidth((Gdiplus::REAL)line_width);
	g->SetSmoothingMode(Gdiplus::SmoothingMode::SmoothingModeAntiAlias);
	g->DrawEllipse(&penCurrent, x, y, D, D);
	g->SetSmoothingMode(Gdiplus::SmoothingMode::SmoothingModeDefault);
}
//------------------------------------------------------- 
void UGC::DrawArc(int x, int y, int D, int start, int end, int line_width)
{
	Gdiplus::Pen penCurrent(color);
	penCurrent.SetWidth((Gdiplus::REAL)line_width);
	g->SetSmoothingMode(Gdiplus::SmoothingMode::SmoothingModeAntiAlias);
	g->DrawArc(&penCurrent, Gdiplus::Rect((int)x, (int)y, (int)D, (int)D), (Gdiplus::REAL)start, (Gdiplus::REAL)end);
	g->SetSmoothingMode(Gdiplus::SmoothingMode::SmoothingModeDefault);
}
//------------------------------------------------------- 

void UGC::FillEllipse(int x, int y, int D)
{
	Gdiplus::SolidBrush mySolidBrush(color);
	g->SetSmoothingMode(Gdiplus::SmoothingMode::SmoothingModeAntiAlias);
	g->FillEllipse(&mySolidBrush, x, y, D, D);
	g->SetSmoothingMode(Gdiplus::SmoothingMode::SmoothingModeDefault);
}
//------------------------------------------------------- 
void UGC::SetTextSize(int size)
{
	TextSize = static_cast<int>(size*getDPIX());
	RealTextSize = static_cast<int>(TextSize*1.2);

}
//------------------------------------------------------- 
int UGC::GetTextHeight(int size)
{
	Gdiplus::Font font(FontName.c_str(), (Gdiplus::REAL)(size / (double)DPIX()), Gdiplus::FontStyleRegular);
	//Font font(L"Helvetica", TextSize, FontStyleRegular); 

	int length = 1;

	PointF origin(0.0f, 0.0f);
	Gdiplus::StringFormat format;
	format.SetAlignment(Gdiplus::StringAlignmentNear);
	RectF boundRect;
	// Measure the string. 
	g->MeasureString(L"A", length, &font, origin, &format, &boundRect);
	return (int)boundRect.Height;
	//return RealTextSize; 
}
//------------------------------------------------------- 
int UGC::GetTextHeight()
{
	return GetTextHeight(TextSize);
	//return RealTextSize; 
}
//------------------------------------------------------- 
int UGC::GetTextWidth(string str)
{
	return GetTextWidth(str, TextSize);
}
//-------------------------------------------------------
int UGC::GetTextWidth(string str, int size)
{
	//size = static_cast<int>(size*getDPIX());

	Gdiplus::Font font(FontName.c_str(), (Gdiplus::REAL)(size / getDPIX()), Bold ? Gdiplus::FontStyleBold : Gdiplus::FontStyleRegular);
	//Font font(L"Helvetica", TextSize, FontStyleRegular); 

	int length = (int)str.length();

	PointF origin(0.0f, 0.0f);
	Gdiplus::StringFormat format;
	format.SetAlignment(Gdiplus::StringAlignmentNear);
	RectF boundRect;
	// Measure the string. 
	g->MeasureString(StringToWString(str).c_str(), length, &font, origin, &format, &boundRect);
	return (int)boundRect.Width;
}
//-----------------------------------------------------------
int UGC::GetTextWidth(wstring str)
{
	return GetTextWidth(str, TextSize);
}
//-------------------------------------------------------------
int UGC::GetTextWidth(wstring str, int size)
{

	Gdiplus::Font font(FontName.c_str(), (Gdiplus::REAL)(size / getDPIX()), Bold ? Gdiplus::FontStyleBold : Gdiplus::FontStyleRegular);
	//Font font(L"Helvetica", TextSize, FontStyleRegular); 

	int length = (int)str.length();

	PointF origin(0.0f, 0.0f);
	Gdiplus::StringFormat format;
	format.SetAlignment(Gdiplus::StringAlignmentNear);
	RectF boundRect;
	// Measure the string. 
	g->MeasureString(str.c_str(), length, &font, origin, &format, &boundRect);
	return (int)boundRect.Width;
}
//------------------------------------------------------- 
void UGC::DrawString(char* str, int x, int y)
{
	string mstring = str;
	DrawString(mstring, x, y);

}
//------------------------------------------------------- 
void UGC::DrawVerticalString(wstring mstring, int x, int y)
{
	g->TranslateTransform((Gdiplus::REAL)x, (Gdiplus::REAL)y); // Set rotation point 
	g->RotateTransform(-90); // Rotate text 
	g->TranslateTransform((Gdiplus::REAL) - x, (Gdiplus::REAL) - y); // Reset translate transform 
	DrawString(mstring, x, y);
	g->ResetTransform(); // Only needed if you reuse the Graphics object for multiple calls to DrawString 

}
//------------------------------------------------------- 
void UGC::DrawString(string mstring, int x, int y, bool antialiasing)
{

	Gdiplus::Font font(FontName.c_str(), static_cast<Gdiplus::REAL>(TextSize / getDPIX()), Bold ? Gdiplus::FontStyleBold : Gdiplus::FontStyleRegular);
	if (antialiasing)
		g->SetTextRenderingHint(Gdiplus::TextRenderingHintAntiAlias);//SetTextRenderingHint(TextRenderingHintAntiAlias); 
	Gdiplus::SolidBrush brush(color);
	if (align == RIGHT)
		x -= GetTextWidth(mstring);
	else if (align == CENTER)
		x -= GetTextWidth(mstring) / 2;
	int length = static_cast<int>(mstring.length());
	g->DrawString(StringToWString(mstring).c_str(), length, &font, Gdiplus::PointF((Gdiplus::REAL)x, (Gdiplus::REAL)y), &brush);
};
//------------------------------------------------------- 
void UGC::DrawString(wstring mstring, int x, int y, bool antialiasing)
{

	Gdiplus::Font font(FontName.c_str(), (Gdiplus::REAL)(TextSize / getDPIX()), Bold ? Gdiplus::FontStyleBold : Gdiplus::FontStyleRegular);
	if (antialiasing)
		g->SetTextRenderingHint(Gdiplus::TextRenderingHintAntiAlias);//SetTextRenderingHint(TextRenderingHintAntiAlias); 
	else
		g->SetTextRenderingHint(Gdiplus::TextRenderingHintSystemDefault);
	Gdiplus::SolidBrush brush(color);
	if (align == RIGHT)
		x -= GetTextWidth(mstring);
	else if (align == CENTER)
		x -= GetTextWidth(mstring) / 2;
	int length = static_cast<int>(mstring.length());
	g->DrawString(mstring.c_str(), length, &font, Gdiplus::PointF((Gdiplus::REAL)x, (Gdiplus::REAL)y), &brush);
};
//------------------------------------------------------- 
void UGC::DrawNumber(int number, int x, int y, bool antialiasing)
{
	char buf[50];
	sprintf_s(buf, "%d", number);
	string str = buf;
	//int len = str.length(); 
	DrawString(str, x, y, antialiasing);
}
//------------------------------------------------------- 
void UGC::DrawNumber(double number, int x, int y, bool antialiasing)
{
	DrawString(ToString(number), x, y, antialiasing);
}
//------------------------------------------------------- 
void UGC::DrawNumber(double number, int x, int y, int count, bool antialiasing)
{
	DrawString(ToString(number, count), x, y, antialiasing);
}
//------------------------------------------------------- 

wstring UGC::ToString(double number)
{
	int sign = 1;
	if (number < 0)
	{
		number = -number;
		sign = -1;
	}
	wstringstream ss;
	ss << number;
	return ss.str();

	/*if (temp ==0)
	return ToString(sign*number, 0);
	else if (temp<0.001)
	return ToString(0, 0);
	else if (temp<0.01)
	return ToString(sign*number, 3);
	else if (temp<0.1)
	return ToString(sign*number, 2);
	else if (temp<1.0)
	return ToString(sign*number, 1);*/

	/*if(number>9.999)
	return ToString(sign*number, 0);
	else if(number<0.001)
	return ToString(0, 0);
	else if (number<0.01)
	return ToString(sign*number, 3);
	else if(number<1.0)
	return ToString(sign*number, 2); */

	//return ToString(sign*number,0); 
}
//-------------------------------------------------------		 

wstring UGC::ToString(double number, int count)
{

	wstringstream ss;
	ss << fixed;
	if (count == 0)
		ss << static_cast<int>(number);
	else
	{
		ss.precision(count);
		ss << number;
	}

	return ss.str();
}
//------------------------------------------------------- 
void UGC::SetAlign(Align _align)
{
	align = _align;
}
void UGC::SetBold(bool status)
{
	Bold = status;
}
//------------------------------------------------------- 
int UGC::getX(int R, int grad)
{
	const double rad_conv = 0.0174;
	double result = R * std::cos(grad*rad_conv);
	return static_cast<int>(result);
}
int UGC::getY(int R, int grad)
{
	const double rad_conv = 0.0174;
	double result = R * std::sin(grad*rad_conv);
	return static_cast<int>(result);
}
void UGC::FillButtonForm(int x, int y, int width, int height)
{

	int R = height / 2;
	Gdiplus::GraphicsPath path;
	path.AddLine(x + R, y,
		x + width - R, y);

	path.AddArc(Gdiplus::Rect(x + width - height, y, height, height), -90, 180);

	path.AddLine(x + width - R, y + height,
		x + R, y + height);

	path.AddArc(Gdiplus::Rect(x, y, height, height), 90, 180);

	Gdiplus::SolidBrush brush(color);

	g->SetSmoothingMode(Gdiplus::SmoothingMode::SmoothingModeAntiAlias);
	g->FillPath(&brush, &path);
	g->SetSmoothingMode(Gdiplus::SmoothingMode::SmoothingModeDefault);



	//FillRectangle(x,y,width,height); 
}


void UGC::FillForm(int x, int y, int width, int height, int b)
{
	Gdiplus::GraphicsPath path;
	path.AddLine(x + b, y,
		x + width - b, y);
	path.AddBezier(x + width - b, y,
		x + width - b / 2, y + b / 4,
		x + width - b / 4, y + b / 2,
		x + width, y + b);

	path.AddLine(x + width, y + b,
		x + width, y + height - b);

	path.AddBezier(x + width, y + height - b,
		x + width - b / 4, y + height - b / 2,
		x + width - b / 2, y + height - b / 4,
		x + width - b, y + height);

	path.AddLine(x + width - b, y + height,
		x + b, y + height);


	path.AddBezier(x + b, y + height,
		x + b / 2, y + height - b / 4,
		x + b / 4, y + height - b / 2,
		x, y + height - b);

	path.AddLine(x, y + height - b,
		x, y + b);

	path.AddBezier(x, y + b,
		x + b / 4, y + b / 2,
		x + b / 2, y + b / 4,
		x + b, y);

	Gdiplus::SolidBrush brush(color);
	g->FillPath(&brush, &path);

	//FillRectangle(x,y,width,height); 
}
//------------------------------------------------------- 
void UGC::FillFormTop(int x, int y, int width, int height, int b)
{
	Gdiplus::GraphicsPath path;
	path.AddLine(x + b, y,
		x + width - b, y);
	path.AddBezier(x + width - b, y,
		x + width - b / 2, y + b / 8,
		x + width - b / 8, y + b / 2,
		x + width, y + b);

	path.AddLine(x + width, y + b,
		x + width, y + height);



	path.AddLine(x + width, y + height,
		x, y + height);


	path.AddLine(x, y + height,
		x, y + b);

	path.AddBezier(x, y + b,
		x + b / 8, y + b / 2,
		x + b / 2, y + b / 8,
		x + b, y);

	Gdiplus::SolidBrush brush(color);
	g->FillPath(&brush, &path);
}
//------------------------------------------------------- 
void UGC::FillFormButtomInvert(int x, int y, int width, int height, int b)
{

	Gdiplus::GraphicsPath path;

	path.AddLine(x, y, x + width, y);

	path.AddLine(x + width, y, x + width, y + height - b);

	path.AddBezier(x + width, y + height - b,
		x + width + b / 6, y + height - b / 3,
		x + width + b / 3, y + height - b / 6,
		x + width + b, y + height);

	path.AddLine(x + width + b, y + height, x - b, y + height);

	path.AddBezier(x - b, y + height,
		x - b / 3, y + height - b / 6,
		x - b / 6, y + height - b / 3,
		x, y + height - b);

	//path.AddLine(x,		y+height-b, x, y+height+b); 

	path.AddLine(x, y + height - b, x, y);

	Gdiplus::SolidBrush brush(color);
	g->FillPath(&brush, &path);

}
//------------------------------------------------------- 
wstring UGC::StringToWString(string str)
{
	//CP_ACP 
	DWORD dwNum = MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, NULL, 0);
	//âûäåëÿåì ïàìÿòü ïîä wide char ñòðîêó: 
	wchar_t *pwText;
	pwText = new wchar_t[dwNum];
	MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, pwText, dwNum);
	wstring w = pwText;
	delete[] pwText;
	return w;
}

double UGC::getDPIX()
{

	return dpix;
}

void UGC::FillDiamondShape(int x, int y, int width, int height)
{
	Gdiplus::GraphicsPath path;

	path.AddLine(x, y + height / 2, x + width / 2, y);
	path.AddLine(x + width / 2, y, x + width, y + height / 2);
	path.AddLine(x + width, y + height / 2, x + height / 2, y + height);

	Gdiplus::SolidBrush brush(color);


	g->SetSmoothingMode(Gdiplus::SmoothingMode::SmoothingModeAntiAlias);
	g->FillPath(&brush, &path);
	g->SetSmoothingMode(Gdiplus::SmoothingMode::SmoothingModeDefault);
}

void UGC::FillDropsShape(int x, int y, int width, int height)
{
	Gdiplus::GraphicsPath path;
	int b = static_cast<int>(getDPIX() * 7);

	path.AddLine(x, y, x + width - b, y);
	path.AddLine(x + width - b, y, x + width, y + b);
	path.AddLine(x + width, y + b, x + width, y + height);
	path.AddLine(x + width, y + height, x, y + height);

	Gdiplus::SolidBrush brush(color);


	//g->SetSmoothingMode(Gdiplus::SmoothingMode::SmoothingModeAntiAlias);
	g->FillPath(&brush, &path);
	//g->SetSmoothingMode(Gdiplus::SmoothingMode::SmoothingModeDefault);
}

void UGC::DrawUnitedForm(int x, int y, int width, int height, int line_width)
{
	Gdiplus::GraphicsPath path;
	int b = width / 2;
	line_width = static_cast<int>(line_width * getDPIX());

	path.AddLine(x + width, y, x + b, y + line_width);
	path.AddLine(x + b, y + line_width, x + b, y + height / 2 - b);
	path.AddLine(x + b, y + height / 2 - b, x, y + height / 2);
	path.AddLine(x, y + height / 2, x + b, y + height / 2 + b);
	path.AddLine(x + b, y + height / 2 + b, x + b, y + height - line_width);
	path.AddLine(x + b, y + height - line_width, x + width, y + height);


	Gdiplus::Pen penCurrent(color);
	penCurrent.SetWidth((Gdiplus::REAL)line_width);
	g->SetSmoothingMode(Gdiplus::SmoothingMode::SmoothingModeAntiAlias);
	g->DrawPath(&penCurrent, &path);
	g->SetSmoothingMode(Gdiplus::SmoothingMode::SmoothingModeDefault);
}

	 