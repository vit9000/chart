#pragma once

#include <sstream>
#include "IChartController.h"
#include "CMainModel.h"
#include "MessageDlg.h"



class CMainController : public IChartController
{
private:
	CMainModel* model;
public:
	CMainController(CMainModel* Model) 
		: IChartController(), model(Model){}

	virtual ~CMainController() 
	{

	}
	
	virtual void objectMouseUp(int id)
	{
		std::wstringstream ss;
		ss << L"ID = " << id;
		MessageDlg(L"Field Click", ss.str()).Show();
	};

	virtual void setPatient(size_t index)
	{ 
		model->setPatient(index);
	}

	virtual void addDrug() 
	{ 
		model->addDrug(L"NaCl 0.9%"); 
	};
};