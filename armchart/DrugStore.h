#pragma once
#include "Global.h"
#include <vector>
#include <string>
#include "type_defines.h"


class DrugStore
{
	DrugList drug_list;
	std::wstring deptID;
public:
	void setDeptID(const std::wstring id)
	{
		deptID = id;
	}
	const DrugList& GetDrugList(const std::wstring& drug)
	{
		drug_list.clear();
		/*std::wstring request = L"EXECUTE solution_apteka.pkg_select_list.select_prod_name_form_existing\n  ";
		request += L"'"+ deptID + L"',";//'65'\n, 
		request += L"'2018-05-21 00:00:00'\n, ''\n, '";
		request += drug;
		request += L"%'\n, NULL\n, ''\n, ''\n, 0";*/
		

		/*CMacroQuery Q;
		//Q.SQL = GetSql(_T("SELECT_PRODUCT_NAME_FORM_BY_MASK_EXISTING2"));
		//Q.SQL = GetSql(_T("SELECT_PRODUCT_NAME_FORM_BY_MASK_EXISTING3"));
		Q.SQL = GetSql(_T("SELECT_PRODUCT_NAME_FORM_BY_MASK_ALL2"));
		//Q.SQL = GetSql(_T("SELECT_PRODUCT_NAME_FORM_BY_MASK_UNIQ_NAME"));
		//Q.ParamByName(_T("dep_id")).AsString = g_CommonStorageProperties.General.DepID;
		//Q.ParamByName(_T("PAYTYPE")).AsString = (NOT_VALID(m_PayTypeID)
		Q.ParamByName(_T("DAT")).AsDate = (int)COleDateTime::GetCurrentTime().m_dt;
		Q.ParamByName(_T("mask")).AsString = L"АН";
		LoadDrugList(Q.SQL);
		*/
		
		std::wstring request = L"SELECT * FROM solution_apteka.product_name WHERE NAME LIKE '" + drug + L"%'";
		request+= L"OR NAME LIKE UPPER('" + drug + L"%')";
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

			std::vector<CString> names;
			std::vector<CString> values;
			while (!rs.Eof()) {

				int count = rs.GetColCount();
				names.clear();
				values.clear();
				for (int i = 0; i < count; i++)
				{
				names.push_back(rs.GetColName(i));
				values.push_back(rs.GetStrValue(i));
				}
				//CString temp = rs.GetStrValue(L"TEXT");
				CString temp = rs.GetStrValue(L"NAME");
				//drug_list.push_back(temp.GetBuffer());
				drug_list.push_back(temp.GetBuffer());
				
				rs.Next();
			}
			rs.Close();
		}
		catch (...) {
			AfxMessageDlg(_T("Ошибка формирования списка !"), MB_ICONSTOP);
		}

	}
};