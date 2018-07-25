#pragma once
#include <vector>
#include <functional>
#include "SmartButton.h"

using namespace std;

class CSmartToolBar : public CToolBarBase
{
	int Width, Height;
	vector<CSmartButton_Ptr> buttons;
	int nextButtonID;
public:

	CSmartToolBar(): Width(0), Height(0), nextButtonID(2000){};
	void addButton(CWnd* parent, const wstring& text, const function<void()>& Func, int group = -1);

	afx_msg void OnPaint(); 
	afx_msg void OnSize(UINT nType, int cx, int cy);
	void setPressed(int button_index) override;
	
	DECLARE_MESSAGE_MAP();
};
