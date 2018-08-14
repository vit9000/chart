#pragma once
#include <windows.h>
#include "ugc.h"
#include "MainBridge.h"
#include "IShowHide.h"
#include "PatientInfo.h"
#include "CMainModel.h"
#include "PrintDocument.h"
class CHeader : public CWnd
{
public:
	CHeader() : bigTextSize(14), smallTextSize(10) {}
	~CHeader();
	void LoadPatient();
	void Clear() { dbpatient = PatientInfo(); RedrawWindow(); }
	void SetFeadback(IShowHide * ShowHider_)
	{
		ShowHider = ShowHider_;
	}
	void SetModel(CMainModel* _model) { model = _model; }
	PatientInfo& getPatient() { return dbpatient; }
	const PatientInfo& getPatient() const { return dbpatient; }
	void Print(UGC& ugc, const CPrintDocument& pDoc);
	void DrawPatientParameters(UGC& ugc, int x, int y, int height);
protected:

	//void ClearTableObjects();
	void SetBounds();
	afx_msg void OnPaint();
	void Draw(UGC& ugc, bool DrawMenuButton = true);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnLButtonUp(UINT flags, CPoint point);
	afx_msg void OnLButtonDown(UINT flags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);

	
	DECLARE_MESSAGE_MAP();
private:
	CMainModel* model;
	int X;
	int Y;
	int Width;
	int Height;
	PatientInfo dbpatient;
	IShowHide * ShowHider;
	int smallTextSize;
	int bigTextSize;

	int DrawSector(UGC& ugc, int x, int y, int height, const wstring& header, const wstring& content);
	int DrawSector(UGC& ugc, int x, int y, int height, const wstring& header, int content);
};

