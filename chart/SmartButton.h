#pragma once
#include <functional>
#include <string> 
#include <memory>
#include "ugc.h"

using namespace std;

class CToolBarBase : public CWnd
{
public:
	virtual void setPressed(int button_index) = 0;
}; 


class CSmartButton : public CWnd
{
	enum STATUS { NORMAL=0, HOVER=1, DOWN=2, DISABLED=3 };
	wstring text;
	int width, height;
	function<void()> func;
	function<void(int)> set_pressed_callback;
	int status;
	bool checked;
	int group;
	int id;
	CToolBarBase *parent;
public:
	bool m_bMouseTracking;
	CSmartButton(CToolBarBase* Parent, int ID, const wstring& Text, CRect& rect, const function<void()>& Func, int Group);
	void setPressed(bool s) 
	{ 
		if (s)
		{
			status = DOWN;
			checked = true;
		}
		else
		{
			status = NORMAL;
			checked = false;
		} 
		RedrawWindow();
	}
	inline int getGroup() const { return group; }
	afx_msg void OnPaint();
	void OnPaint(UGC& ugc);
	afx_msg void OnLButtonUp(UINT flags, CPoint point);
	afx_msg void OnLButtonDown(UINT flags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnMouseLeave();
	afx_msg void OnSize(UINT nType, int cx, int cy);

	inline int getWidth() const { return width; }
	inline int getHeight() const { return height; }
	inline void setEnabled(bool enabled) { status = (enabled) ? NORMAL : DISABLED; }
	DECLARE_MESSAGE_MAP();
public:


protected:
	void DrawNormal(UGC& ugc);
	void DrawHover(UGC& ugc);
	void DrawDown(UGC& ugc);
	void DrawDownRect(UGC& ugc);
	void DrawHoverRect(UGC& ugc);
};


typedef unique_ptr<CSmartButton> CSmartButton_Ptr;