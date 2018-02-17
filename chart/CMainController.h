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
	
	void objectMouseUp(int id) override
	{
		std::wstringstream ss;
		ss << L"ID = " << id;
		MessageDialog(L"Field Click", ss.str()).Show();
	};

	void setPatient(size_t index) override
	{ 
		model->setPatient(index);
	}

	void addDrug() override
	{ 
		InputDialog dlg(InputDialog::NEW_LINE_DIALOG);
		if(dlg.Show()==IDOK)
		{
			switch (dlg.getType())
			{
			case 0:
				model->addDrug(dlg.getString());
				break;
			case 1:
				model->addParameter(dlg.getString());
				break;
			}
		}

	};

	void addDrugUnit(int ID, int start) override
	{
		InputDialog dlg(InputDialog::VALUE_INPUT_DIALOG);
		if (dlg.Show() == IDOK)
		{
			const auto& res = dlg.getValues();
			model->addDrugUnit(ID, res.first, start, res.second);
		}
		//
	}

	void addParameterUnit(int ID, int start) override
	{
		InputDialog dlg(InputDialog::VALUE_INPUT_DIALOG);
		if (dlg.Show() == IDOK)
		{
			const auto& res = dlg.getValues();
			model->addParameterUnit(ID, res.first, start);
		}
	}
};