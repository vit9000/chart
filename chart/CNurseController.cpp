#include "stdafx.h"
#include "CNurseController.h"



void CNurseController::objectMouseUp(const ID& id)
{

	
};
//-----------------------------------------------------------------------------------------------
void CNurseController::setPatient(size_t index)
{
	model->setPatient(index);
}
//-----------------------------------------------------------------------------------------------
void CNurseController::addDrug()
{
	NewLineDialog dlg;
	if (dlg.DoModal() == IDOK)
	{
		model->addDrug(dlg.getType(), dlg.getDrugInfo());
	}

};
//-----------------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------------
void CNurseController::addParameterUnit(const ID& id, int start, const Rect& rect)
{
	function<void(const std::wstring&)> callBack = [this, id, start](const std::wstring& val)
	{
		model->addParameterUnit(id, Value(val), start);
	};
	if (cursorHandler)
		cursorHandler->setEditBox(rect, callBack, L"");


}
//-----------------------------------------------------------------------------------------------
void CNurseController::addParameterUnits(const vector<ID>& ids, int start)
{
	ValueInputDlg dlg;
	vector<wstring> paramNames;
	vector<wstring> content;
	for (const ID& id : ids)
	{
		paramNames.push_back(model->getContainerName(id));
		content.push_back(L"");
	}

	dlg.Init(ids[0].getBlockName(), paramNames, content);
	if (dlg.DoModal() == IDOK)
	{
		const vector<Value>& values = dlg.getValue();
		model->addParameterUnits(ids, values, start);
	}
}
//-----------------------------------------------------------------------------------------------
void CNurseController::tempUpdateDrugUnitValue(const ID& id, int unit_number)
{
	const auto& cu = model->getCurrentPatient()->getContainerUnit(id);
	if (cu->isChangeStatusAvailable())
	{
		bool temp = cu->getUnit(unit_number).isCompleted();
		model->getCurrentPatient()->getContainerUnit(id)->setCompleted(unit_number, !temp);
	}
}
//-----------------------------------------------------------------------------------------------
void CNurseController::updateDrugUnitValue(const ID& id, int unit_number)
{
	tempUpdateDrugUnitValue(id, unit_number);
	repaint();
}
//-----------------------------------------------------------------------------------------------
void CNurseController::updateDrugUnitValues(const vector<ID>& ids, int unit_number)
{
	for (const ID& id : ids)
	{
		tempUpdateDrugUnitValue(id, unit_number);
	}
	repaint();
}
//-----------------------------------------------------------------------------------------------

void CNurseController::repaint()
{
	model->NotifyEmpty();
}
//-----------------------------------------------------------------------------------------------
