#pragma once
#include <windows.h>
#include "ugc.h"
#include "MainBridge.h"
#include "IShowHide.h"
#include "PatientInfo.h"
#include "CMainModel.h"
class CHeader : public CWnd
{
public:
	CHeader() {}
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
	CMainModel* model;
	int Width;
	int Height;
	PatientInfo dbpatient;
	IShowHide * ShowHider;

	int DrawSector(UGC& ugc, int x, const wstring& header, const wstring& content);
	int DrawSector(UGC& ugc, int x, const wstring& header, int content);
	int DrawSector2Lines(UGC& ugc, int x, const wstring& line1, const wstring& line2);
};

