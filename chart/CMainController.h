#pragma once

#include <sstream>
#include "IChartController.h"
#include "MessageDlg.h"



class CMainController : public IChartController
{
public:
	CMainController(CMainModel* Model) : IChartController(Model){}

	virtual ~CMainController() 
	{

	}
	
	virtual void objectMouseUp(int id)
	{
		std::wstringstream ss;
		ss << L"ID = " << id;
		MessageDlg(L"Field Click", ss.str()).Show();
	};

	virtual void setPatient(size_t index){ model->setPatient(index);}

	virtual void addDrug(const wstring& DrugName) { model->addDrug(DrugName); };
};