#pragma once

#include <vector>

struct DeptInfo
{
	CString status;
	CString name;
	CString code;
	CString keyID;
};

typedef std::vector<DeptInfo>* DeptInfoArrayPtr;

// Диалоговое окно DepDlg
class DepDlg : public CDialog
{
	DECLARE_DYNAMIC(DepDlg)

public:
	DepDlg(CWnd* pParent = NULL);   // стандартный конструктор
	virtual ~DepDlg();
	void Init(const DeptInfoArrayPtr deptInfo);
	inline int getSelected() { return selected_index; }

// Данные диалогового окна
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DEP_DIALOG };
#endif

private:
	DeptInfoArrayPtr dept_info;
	CListBox m_DepList;
	int selected_index;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // поддержка DDX/DDV
	virtual BOOL OnInitDialog();
	

	DECLARE_MESSAGE_MAP()
	
	afx_msg void OnBnClickedOk();
	afx_msg void OnLbnSelchangeDepList();
};
