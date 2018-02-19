#pragma once

#include <sstream>
#include "IChartController.h"
#include "CMainModel.h"
#include "MessageDialog.h"

#include "NewLineDialog.h"
#include "ValueInputDlg.h"


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
		//InputDialog dlg(InputDialog::NEW_LINE_DIALOG);
		NewLineDialog dlg;
		if(dlg.DoModal()==IDOK)
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
		//InputDialog dlg(InputDialog::VALUE_INPUT_DIALOG);
		ValueInputDlg dlg;
		if (dlg.DoModal() == IDOK)
		{
			const auto& value = dlg.getValue();
			model->addDrugUnit(ID, value, start, 60);
		}
		//
	}

	void addParameterUnit(int ID, int start) override
	{
		//InputDialog dlg(InputDialog::VALUE_INPUT_DIALOG);
		ValueInputDlg dlg;
		if (dlg.DoModal() == IDOK)
		{
			const auto& value = dlg.getValue();
			model->addParameterUnit(ID, value, start);
		}
	}

	void updateUnitValue(int ID, int unit_number) override
	{
		ValueInputDlg dlg;
		if (dlg.DoModal() == IDOK)
		{
			const auto& value = dlg.getValue();
			model->updateUnitValue(ID, unit_number, value);
		}
	}

	virtual void updateUnitPosition(int ID, int unit_number, int start, int duration)
	{
		model->updateUnitPosition(ID, unit_number, start, duration);
	}
};