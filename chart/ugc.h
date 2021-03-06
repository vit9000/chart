#ifndef UGC_H 
 #define UGC_H 
 #define null NULL 
  
  
 #include <windows.h> 
 #include <GdiPlus.h> 
 #include <string> 
 #include <sstream>
 #include <cstdio> 
#include <iomanip>
 #include "dpix.h" 
  
 using namespace std; 
 
  
 using Gdiplus::Bitmap; 
 using Gdiplus::Color; 
 using Gdiplus::RectF; 
 using Gdiplus::PointF; 
 
 Color convertColor(DWORD _color);
  
  
 class UGC 
 { 
 public:
 	enum Align { CENTER, LEFT, RIGHT }; 
 private: 
 	int WIDTH, HEIGHT; 
 	int TextSize; 
 	int RealTextSize; 
 	Align align; 
  
 	bool Bold; 
	double dpix;
 	Gdiplus::Graphics *g; 
 	Gdiplus::Color color; 
 	wstring FontName; 
  
 	bool doublebuffering; 
 	 
 	HDC hdc; 
 	HDC Memhdc; 
 	HBITMAP Membitmap; 
 public: 
	 
 	inline Gdiplus::Graphics *getGraphics() { return g; }; 

  
 	UGC(Bitmap* bitmap); 
 	UGC(CDC* pDC, int width, int height); 
	UGC(HDC& hdc, int width, int height);
	void Init(int width, int height);
	~UGC();
 	inline int getWidth() const { return WIDTH; } 
 	inline int getHeight() const { return HEIGHT; } 
 private: 
	 void StartDraw();
 	 void EndDraw();
 	
 public: 
	void Clear();
	void DrawBitmap(Gdiplus::Bitmap* bitmap, int x, int y, int width, int height);
	void SetSpecColor(int color);
 	void SetDrawColor(int A, int R, int G, int B) ; 
 	void SetDrawColor(int R, int G, int B); 
 	void SetDrawColor(Color _color); 
	void SetDrawColor(DWORD _color);
	void DrawPoint(int x, int y, int size = 1);
	void FillTriangle(int x1, int y1, int x2, int y2, int x3, int y3); 
	void DrawTriangle(int x1, int y1, int x2, int y2, int x3, int y3, int line_width = 1);
	void DrawLine(int x1, int y1, int x2, int y2, int line_width = 1);
	void DrawLineAntialiased(int x1, int y1, int x2, int y2, int line_width = 1);
	void DrawDashLine(int x1, int y1, int x2, int y2, int line_width = 1);
	void DrawDotLine(int x1, int y1, int x2, int y2, int line_width = 1);
	void DrawPunktRectangle(int x, int y, int width, int height, int line_width = 1);
	void DrawRectangle(int x, int y, int width, int height, int line_width = 1);
	void FillRectangle(int x, int y, int width, int height, bool finishdraw = true);
	void FillObj(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4);
	void DrawEllipse(int x, int y, int D, int line_width);
	void DrawArc(int x, int y, int D, int start, int end, int line_width);
	void FillEllipse(int x, int y, int D);
	void SetTextSize(int size);
	int GetTextHeight(int size);
	int GetTextHeight();
	int GetTextWidth(string str);
	int GetTextWidth(string str, int size);
	int GetTextWidth(wstring str);
	int GetTextWidth(wstring str, int size);
	void DrawString(char* str, int x, int y);
	void DrawVerticalString(wstring mstring, int x, int y);
	void DrawString(string mstring, int x, int y, bool antialiasing = true);
	void DrawString(wstring mstring, int x, int y, bool antialiasing = true);
	void DrawNumber(int number, int x, int y, bool antialiasing = true);
	void DrawNumber(double number, int x, int y, bool antialiasing = true);
	void DrawNumber(double number, int x, int y, int count, bool antialiasing = true);
	wstring ToString(double number);
	wstring ToString(double number, int count);
	void SetAlign(Align _align);
	void SetBold(bool status);
	int getX(int R, int grad);
	int getY(int R, int grad);
	void FillButtonForm(int x, int y, int width, int height);
	void FillForm(int x, int y, int width, int height, int b = 10);
	void FillFormTop(int x, int y, int width, int height, int b = 10);
	void FillFormButtomInvert(int x, int y, int width, int height, int b = 10);
	wstring StringToWString(string str);
	double getDPIX();
	void FillDiamondShape(int x, int y, int width, int height);
	void FillDropsShape(int x, int y, int width, int height);
	void DrawUnitedForm(int x, int y, int width, int height, int line_width = 1);
 }; 
  
 #endif // UGC_H 
