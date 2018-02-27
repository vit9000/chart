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
		NewLineDialog dlg;
		if(dlg.DoModal()==IDOK)
		{
			model->addDrug(dlg.getType(), dlg.getString());
		}

	};

	void addDrugUnit(int ID, int start) override
	{
		ValueInputDlg dlg;
		dlg.Init(model->getContainerName(ID), L"");
		if (dlg.DoModal() == IDOK)
		{
			const auto& value = dlg.getValue();
			model->addDrugUnit(ID, value, start, 60);
		}
	}

	void addParameterUnit(int ID, int start) override
	{
		ValueInputDlg dlg;
		int dialog_type = ValueInputDlg::STANDART;
		if (dynamic_cast<const ContainerHemodynamic*>(model->getCurrentPatient()->getContainerUnit(ID).get()))
			dialog_type = ValueInputDlg::HEMODYNAMIC;

		dlg.Init(model->getContainerName(ID), L"", dialog_type);
		if (dlg.DoModal() == IDOK)
		{
			const Value& value = dlg.getValue();
			model->addParameterUnit(ID, value, start);
		}
	}

	void updateUnitValue(int ID, int unit_number) override
	{
		ValueInputDlg dlg;
		int dialog_type = ValueInputDlg::STANDART;
		if (dynamic_cast<const ContainerHemodynamic*>(model->getCurrentPatient()->getContainerUnit(ID).get()))
			dialog_type = ValueInputDlg::HEMODYNAMIC;
		std::wstringstream ss;
		ss << model->getCurrentPatient()->getContainerUnit(ID)->getUnit(unit_number).getValue();
		dlg.Init(model->getContainerName(ID), ss.str(), dialog_type);
		
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