#pragma once
#include <windows.h>
#include "ugc.h"
#include "MainBridge.h"
#include "IShowHide.h"
class CHeader : public CWnd
{
public:
	CHeader();
	~CHeader();
	void LoadPatient(int index);
	void SetFeadback(IShowHide * ShowHider_)
	{
		ShowHider = ShowHider_;
	}
protected:

	//void ClearTableObjects();
	void SetBounds();
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnLButtonUp(UINT flags, CPoint point);
	afx_msg void OnLButtonDown(UINT flags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);


	DECLARE_MESSAGE_MAP();
private:
	int Width;
	int Height;
	DBPatient dbpatient;
	int patient_number;
	IShowHide * ShowHider;

	int DrawSector(UGC& ugc, int x, const wstring& header, const wstring& content);
	int DrawSector(UGC& ugc, int x, const wstring& header, int content);
};

