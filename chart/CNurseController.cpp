#include "stdafx.h"
#include "CNurseController.h"



void CNurseController::objectMouseUp(const ID& id)
{

	
};
//-----------------------------------------------------------------------------------------------
void CNurseController::setPatient(size_t index)
{
//	model->setPatient(index);
}
//-----------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------
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
	/*for (const ID& id : ids)
	{
		tempUpdateDrugUnitValue(id, unit_number);
	}*/
	if (ids.size() == 0) return;
	tempUpdateDrugUnitValue(ids[0], unit_number);
	repaint();
}
//-----------------------------------------------------------------------------------------------