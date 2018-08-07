#pragma once
#include <vector>
#include <functional>
#include "SmartButton.h"

#define ST_FILL_ALL_WIDTH 0x10000000L
#define BUTTON_MIN_WIDTH 140

using namespace std;

class CSmartToolBar : public CToolBarBase
{
	int Width, Height;
	vector<CSmartButton_Ptr> buttons;
	int nextButtonID;
	DWORD params;
public:

	CSmartToolBar(): Width(0), Height(0), nextButtonID(2000), params(0){};
	void SetParam(DWORD Params) { params = Params; }
	void addButton(CWnd* parent, const wstring& text,  const function<void()>& Func, int group = -1);

	afx_msg void OnPaint(); 
	afx_msg void OnSize(UINT nType, int cx, int cy);
	void setPressed(int button_index) override;
	void setEnabled(bool status);
	
	DECLARE_MESSAGE_MAP();
};

