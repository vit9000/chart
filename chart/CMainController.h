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
	
	void objectMouseUp(const ID& id) override
	{
		std::wstringstream ss;
		ss << L"ID = " << id.getIndex();
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

	void addDrugUnit(const ID& id, int start) override
	{
		ValueInputDlg dlg;
		dlg.Init(model->getContainerName(id), L"");
		if (dlg.DoModal() == IDOK)
		{
			const auto& value = dlg.getValue();
			model->addDrugUnit(id, value, start, 60);
		}
	}

	void addParameterUnit(const ID& id, int start) override
	{
		ValueInputDlg dlg;
		int dialog_type = ValueInputDlg::STANDART;
		if (dynamic_cast<const ContainerHemodynamic*>(model->getCurrentPatient()->getContainerUnit(id.getBlockName(), id.getIndex()).get()))
			dialog_type = ValueInputDlg::HEMODYNAMIC;

		dlg.Init(model->getContainerName(id), L"", dialog_type);
		if (dlg.DoModal() == IDOK)
		{
			const Value& value = dlg.getValue();
			model->addParameterUnit(id, value, start);
		}
	}

	void updateUnitValue(const ID& id, int unit_number) override
	{
		ValueInputDlg dlg;
		int dialog_type = ValueInputDlg::STANDART;
		if (dynamic_cast<const ContainerHemodynamic*>(model->getCurrentPatient()->getContainerUnit(id.getBlockName(), id.getIndex()).get()))
			dialog_type = ValueInputDlg::HEMODYNAMIC;
		std::wstringstream ss;
		ss << model->getCurrentPatient()->getContainerUnit(id.getBlockName(), id.getIndex())->getUnit(unit_number).getValue().getValue();
		dlg.Init(model->getContainerName(id), ss.str(), dialog_type);
		
		if (dlg.DoModal() == IDOK)
		{
			const auto& value = dlg.getValue();
			model->updateUnitValue(id, unit_number, value);
		}
	}

	virtual void updateUnitPosition(const ID& id, int unit_number, int start, int duration)
	{
		model->updateUnitPosition(id, unit_number, start, duration);
	}
};