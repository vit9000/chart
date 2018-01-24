#pragma once

#include <sstream>
#include "IChartController.h"
#include "CMainModel.h"
#include "MessageDialog.h"
#include "InputDialog.h"



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
		MessageDialog(L"Field Click", ss.str()).Show();
	};

	virtual void setPatient(size_t index)
	{ 
		model->setPatient(index);
	}

	virtual void addDrug() 
	{ 
		InputDialog dlg(InputDialog::NEW_LINE_DIALOG);
		if(dlg.Show()==IDOK)
		{
			model->addDrug(dlg.getString()); 
		}
/*
		NewLineDialog *dlg = new NewLineDialog();
		if(dlg->DoModal()==IDOK)
		{
			model->addDrug(dlg->getString()); 
		}
		delete dlg;*/
	};
};