#include "stdafx.h"
#include "CPrintController.h"



void CPrintController::objectMouseUp(const ID& id)
{

	/*
	std::wstringstream ss;
	ss << L"ID = " << id.getIndex();
	MessageDialog(L"Field Click", ss.str()).Show();*/
};
//-----------------------------------------------------------------------------------------------
void CPrintController::setPatient(size_t index)
{
	model->setPatient(index);
}
//-----------------------------------------------------------------------------------------------
void CPrintController::repaint()
{
	model->NotifyEmpty();
}
//-----------------------------------------------------------------------------------------------
