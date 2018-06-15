// ValueInputDlg.cpp : implementation file
//
#include "resource.h"
#include "stdafx.h"
#include "chart.h"
#include "ValueInputDlg.h"
#include "afxdialogex.h"
#include "dpix.h"
#include "utils.h"

// ValueInputDlg dialog

IMPLEMENT_DYNAMIC(ValueInputDlg, CDialogEx)

ValueInputDlg::ValueInputDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_INSERT_UNIT_DIALOG, pParent)
{

}

ValueInputDlg::~ValueInputDlg()
{
}

void ValueInputDlg::Init(const wstring& block_name, const vector<wstring>& parameters, const vector<wstring>& editbox_content)
{
	param = vector<wstring>(parameters);
	content = vector<wstring>(editbox_content);

	header = CString(block_name.c_str());
	
	count = parameters.size();
}

void ValueInputDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//DDX_Control(pDX, IDC_PARAMETER_LIST, main_list);
}

BOOL ValueInputDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	this->SetWindowTextW(header);
	
	CRect rect;
	GetClientRect(&rect);
	double dpix = DPIX();
	rect.left += static_cast<int>(10. * dpix);
	rect.right -= static_cast<int>(10. * dpix);
	rect.top += static_cast<int>(10. * dpix);
	rect.bottom -= static_cast<int>(40. * dpix);

	main_list.Create(NULL, NULL, WS_VISIBLE | WS_CHILD, rect, this, IDC_PARAMETER_LIST);
	main_list.ModifyStyle(LVS_LIST, LVS_REPORT, 0); //- ставим режим таблицы
	main_list.SetCloseDlgFunction([this]() {OnBnClickedOk(); });
	/*main_list.SetExtendedStyle(LVS_EDITLABELS | LVS_EX_GRIDLINES);
	main_list.InsertColumn(0, L"Показатель", LVCFMT_CENTER, 144 * DPIX(), 0);//добавляем колонки
	main_list.InsertColumn(1, L"Значение", LVCFMT_CENTER, 100 * DPIX(), 0);//добавляем колонки
	*/
	for (size_t i=0; i<param.size(), i<content.size(); i++)
	{
		/*int line = main_list.InsertItem(LVIF_TEXT, i, param[i].c_str(), 0, 0, 0, NULL);
		main_list.SetItemText(line, 1, content[i].c_str());*/
		main_list.InsertItem(param[i], content[i]);
	}
	
	
	/*HWND hwndControl;
	GetDlgItem(IDC_EDIT_VALUE, &hwndControl);
	PostMessage(WM_NEXTDLGCTL, (WPARAM)hwndControl, TRUE);*/

	return TRUE;  // return TRUE  unless you set the focus to a control
}

BEGIN_MESSAGE_MAP(ValueInputDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &ValueInputDlg::OnBnClickedOk)
	ON_WM_SHOWWINDOW()
END_MESSAGE_MAP()


// ValueInputDlg message handlers

#include <sstream>
#include <string>
void ValueInputDlg::OnBnClickedOk()
{
	CString temp;
	
	wstring err_fields;
	result.clear();
	for (size_t i = 0; i < count; i++)
	{
		/*CString res = main_list.GetItemText(i, 1);
		result.push_back(Value(res.GetBuffer()));*/

		Value val = Value(main_list.GetItemText(i));
		if (!isInputDataValid(val))
		{
			if (!err_fields.empty())
				err_fields += L", ";
			err_fields += L"\""+main_list.GetItemName(i)+L"\"";
		}
		result.push_back(val);
	}
	
	if (err_fields.size() > 0)
	{
		wstring msg = L"Допускается только числовые значения в полях: " + err_fields;
		MessageBox(msg.c_str(), L"Внимание", MB_OK | MB_ICONINFORMATION);
		
		return;
	}

	CDialogEx::OnOK();
}


void ValueInputDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	main_list.SetEditBox(0);
}