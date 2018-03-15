#pragma once
#include <functional>
#include <string>

class CInPlaceEditbox : public CEdit
{
public:
	CInPlaceEditbox(std::function<void(const std::wstring&)> CallBackFunction, const std::wstring& defaultValue, std::function<void()> Next=nullptr);
	virtual ~CInPlaceEditbox();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
protected:
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnNcDestroy();
	//afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);

	DECLARE_MESSAGE_MAP()

private:
	std::function<void(const std::wstring&)> callBack;
	std::function<void()> next;
	CString m_sInitText;
	BOOL    m_bESC;	 	// To indicate whether ESC key was pressed
	bool is_cancel;
	bool allow_next;
	CFont* font;
};

