#pragma once
#include "Global.h"
#include <vector>
#include <string>
#include "type_defines.h"


class DrugStore
{
	DrugList drug_list;
public:
	const DrugList& GetDrugList(const std::wstring& drug)
	{
		drug_list.clear();
		std::wstring request = L"EXECUTE solution_apteka.pkg_select_list.select_prod_name_form_existing\n  '65'\n, '2018-05-21 00:00:00'\n, ''\n, '";
		request += drug;
		request += L"%'\n, NULL\n, ''\n, ''\n, 0";
		LoadDrugList(request.c_str());
		return drug_list;
	}
	//-----------------------------------------------------------------
private:
	void LoadDrugList(const TCHAR * sql)
	{
		try {
			CADOResult rs = g_lpConn->Execute(sql);
			int row = 1;
			while (!rs.Eof()) {

				int count = rs.GetColCount();
				/*names.clear();
				values.clear();
				for (int i = 0; i < count; i++)
				{
				names.push_back(rs.GetColName(i));
				values.push_back(rs.GetStrValue(i));
				}*/
				CString temp = rs.GetStrValue(L"TEXT");
				drug_list.push_back(temp.GetBuffer());
				//if (FillRow(rs, row) == TRUE)
				//	row++;
				rs.Next();
			}
			rs.Close();
		}
		catch (...) {
			AfxMessageDlg(_T("Ошибка формирования списка !"), MB_ICONSTOP);
		}

	}
};