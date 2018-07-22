#include "stdafx.h"
#include "CMainController.h"

bool CMainController::isAvailableUpdateUnit(const Unit& unit)
{
	if (unit.isCompleted())
	{
		MessageBox(parentDlg->m_hWnd, L"Выполненное назначение не может быть изменено", L"Внимание", MB_OK | MB_ICONINFORMATION);
		repaint();
		return false;
	}
	else return true;
}

void CMainController::objectMouseUp(const ID& id)
{

	/*
	std::wstringstream ss;
	ss << L"ID = " << id.getIndex();
	MessageDialog(L"Field Click", ss.str()).Show();*/
};
//-----------------------------------------------------------------------------------------------
void CMainController::setPatient(size_t index)
{
	//model->setPatient(index);
}
//-----------------------------------------------------------------------------------------------
void CMainController::addDrug()
{
	
	NewLineDialog dlg(true);
	if (dlg.DoModal() == IDOK)
	{
		model->addDrug(ID(), dlg.getCode(), dlg.getDrugInfo());
	}

};

void CMainController::lineSmartMenu(int x, int y, const ID&id)
{
	const wstring& db_id = id.getIndex();
	if (!db_id.empty() && db_id.at(0) != L'N') return;

	MENU menu;
	CSmartMenu *sm = new CSmartMenu;
	int xi(0), yi(0);
	if (cursorHandler)
		cursorHandler->getWindowPos(xi, yi);
	
	menu.push_back(
		make_pair(L"Удалить строку",
			[this, id]()
		{
			if (MessageBox(0, L"Вы уверены, что хотите удалить строку?", L"Подтверждение", MB_YESNO) == IDYES)
			{
				model->deleteDrug(id);
				repaint();
			}
		})
	);
	sm->Init(x + xi, y + yi, menu);
	sm->Create(IDD_SMART_MENU);
	sm->ShowWindow(SW_SHOW);
}

//-----------------------------------------------------------------------------------------------
void CMainController::showSmartMenu(int x, int y, const ID&id, int unit_number, MENU& menu)
{
	CSmartMenu *sm = new CSmartMenu;
	int xi(0), yi(0);
	if (cursorHandler)
		cursorHandler->getWindowPos(xi, yi);

	const auto& cu = model->getCurrentPatient()->getContainerUnit(id);
	if (cu->isChangeStatusAvailable())
	{
		bool temp = cu->getUnit(unit_number).isCompleted();
		menu.push_back(
			make_pair(L"Пометить как " + wstring((temp) ? L"не" : L"") + L"выполненное",
				[this, id, unit_number, temp]()
		{
			if (MessageBox(0, L"Вы уверены, что хотите изменить статус назначения?", L"Подтверждение", MB_YESNO) == IDYES)
			{
				model->getCurrentPatient()->getContainerUnit(id)->setCompleted(unit_number, !temp);
				repaint();
			}
		})
		);
	}
	

	
	Unit unit = model->getUnit(id, unit_number);
	if (unit.getDB_ID().empty()) // если сохранено в БД, то удалить нельзя
	{
		menu.push_back(
			make_pair(wstring(L"Удалить назначение"),
				[this, id, unit_number]()
		{
			if (MessageBox(0, L"Вы уверены, что хотите удалить назначение?", L"Подтверждение", MB_YESNO) == IDYES)
				model->deleteUnit(id, unit_number);
		})
		);
	}

	menu.push_back(
		make_pair(wstring(L"Информация о препарате"),
			[this, id]()
	{
		AdditionalFeatures().RunDrugInfo(model->getDrugInfo(id).name);
	})
	);


	sm->Init(x + xi, y + yi, menu);
	sm->Create(IDD_SMART_MENU);
	sm->ShowWindow(SW_SHOW);
}
//-----------------------------------------------------------------------------------------------
void CMainController::addDrugToDrug(const ID& host_id)
{
	NewLineDialog dlg(false);
	if (dlg.DoModal() == IDOK)
	{
		model->addChildDrug(ID(), host_id, dlg.getDrugInfo());
	}
}
//-----------------------------------------------------------------------------------------------
void CMainController::addDrugUnit(const ID& id, int start)
{
	ValueInputDlg dlg;
	ContainerUnit_Ptr cu = model->getCurrentPatient()->getContainerUnit(id);
	const DrugInfo& di = cu->getDrugInfo();

	dlg.Init(id.getBlockName(), { cu->getName() }, { di.ED }, { ToString(model->getDrugInfo(id).dose) });
	if (dlg.DoModal() == IDOK)
	{
		const auto& value = dlg.getValue();
		model->addUnit(id, Unit(value[0], start, config->getStep()));
	}
}
//-----------------------------------------------------------------------------------------------
void CMainController::addDrugUnits(const vector<ID>& ids, int start)
{
	ValueInputDlg dlg;
	size_t size = ids.size();
	vector<wstring> drugNames; drugNames.reserve(size);
	vector<wstring> mes_units; mes_units.reserve(size);
	vector<wstring> content;   content.reserve(size);
	for (const ID& id : ids)
	{
		ContainerUnit_Ptr cu = model->getCurrentPatient()->getContainerUnit(id);
		const DrugInfo& di = cu->getDrugInfo();
		drugNames.push_back(cu->getName());
		mes_units.push_back(di.ED);
		content.push_back(ToString(di.dose));
	}

	dlg.Init(ids[0].getBlockName(), drugNames, mes_units, content);
	if (dlg.DoModal() == IDOK)
	{
		const vector<Value>& values = dlg.getValue();
		model->addUnits(ids, values, start, config->getStep());
	}
}
//-----------------------------------------------------------------------------------------------
void CMainController::addParameterUnit(const ID& id, int start, const Rect& rect)
{
	function<void(const std::wstring&)> callBack = [this, id, start](const std::wstring& val)
	{
		model->addUnit(id, Unit(Value(val), start, config->getStep()));
	};
	if (cursorHandler)
		cursorHandler->setEditBox(rect, callBack, L"", model->getCurrentPatient()->getContainerUnit(id)->isDigit());
}
//-----------------------------------------------------------------------------------------------
void CMainController::addParameterUnits(const vector<ID>& ids, int start)
{
	ValueInputDlg dlg;
	size_t size = ids.size();
	vector<wstring> paramNames; paramNames.reserve(size);
	vector<wstring> mes_units;  mes_units.reserve(size);
	vector<wstring> content; content.reserve(size);

	for (const ID& id : ids)
	{
		const ContainerUnit_Ptr& cu_ptr = model->getCurrentPatient()->getContainerUnit(id);
		paramNames.push_back(cu_ptr->getName());
		content.push_back(L"");
		mes_units.push_back(cu_ptr->getMeasureUnit());
	}

	dlg.Init(ids[0].getBlockName(), paramNames, mes_units, content);
	if (dlg.DoModal() == IDOK)
	{
		const vector<Value>& values = dlg.getValue();
		model->addUnits(ids, values, start, config->getStep());
	}
}
//-----------------------------------------------------------------------------------------------
void CMainController::updateUnitValue(const ID& id, int unit_number, const Rect& rect)
{
	function<void(const std::wstring&)> callBack = [this, id, unit_number](const std::wstring& val)
	{
		Unit unit = model->getUnit(id, unit_number);
		model->updateUnit(id, unit_number, Unit(Value(val), unit.getStart(), unit.getDuration()));
	};
	if (cursorHandler)
	{
		ContainerUnit_Ptr cu = model->getCurrentPatient()->getContainerUnit(id);
		cursorHandler->setEditBox(rect, callBack, cu->getUnit(unit_number).getValue(), cu->isDigit());
	}
}
//-----------------------------------------------------------------------------------------------
void CMainController::updateUnitValue(const ID& id, int unit_number)
{
	ValueInputDlg dlg;
	int dialog_type = ValueInputDlg::STANDART;

	ContainerUnit_Ptr cu = model->getCurrentPatient()->getContainerUnit(id);
	const Unit& oldUnit = cu->getUnit(unit_number);
	if (!isAvailableUpdateUnit(oldUnit))
		return;
	dlg.Init(id.getBlockName(), { cu->getName() }, { cu->getDrugInfo().ED }, { oldUnit.getValue() });

	if (dlg.DoModal() == IDOK)
	{
		const auto& value = dlg.getValue();
		Unit unit = model->getUnit(id, unit_number);
		model->updateUnit(id, unit_number, Unit(value[0], unit.getStart(), unit.getDuration()));
	}
}
//-----------------------------------------------------------------------------------------------
void CMainController::updateUnitValues(const vector<ID>& ids, int unit_number)
{
	ValueInputDlg dlg;
	size_t size = ids.size();
	vector<wstring> paramNames; paramNames.reserve(size);
	vector<wstring> mes_units; mes_units.reserve(size);
	vector<wstring> content;   content.reserve(size);
	for (const ID& id : ids)
	{
		const ContainerUnit_Ptr& cu_ptr = model->getCurrentPatient()->getContainerUnit(id);
		const Unit& oldUnit = cu_ptr->getUnit(unit_number);
		if (!isAvailableUpdateUnit(oldUnit))
			return;
		paramNames.push_back(cu_ptr->getName());
		mes_units.push_back(cu_ptr->getMeasureUnit());
		content.push_back(cu_ptr->getUnit(unit_number).getValue());
	}
	dlg.Init(ids[0].getBlockName(), paramNames, mes_units, content);

	if (dlg.DoModal() == IDOK)
	{
		const auto& value = dlg.getValue();
		model->updateUnitValues(ids, unit_number, value);
	}
}
//-----------------------------------------------------------------------------------------------
void CMainController::updateUnitPosition(const ID& id, int unit_number, int start, int duration)
{
	const Unit& oldUnit = model->getUnit(id, unit_number);
	if (!isAvailableUpdateUnit(oldUnit))
		return;
	Value val = oldUnit.getValue();
	model->updateUnit(id, unit_number, Unit(val, start, duration));
}
//-----------------------------------------------------------------------------------------------
void CMainController::updateUnitPositions(const vector<ID>& ids, int unit_number, int start, int duration)
{
	const Unit& oldUnit = model->getUnit(ids[0], unit_number);
	if (!isAvailableUpdateUnit(oldUnit))
		return;
	model->updateUnitPositions(ids, unit_number, start, duration);
}
//-----------------------------------------------------------------------------------------------
void CMainController::repaint()
{
	model->NotifyEmpty();
}
//-----------------------------------------------------------------------------------------------
void CMainController::SetMouseCursor(size_t index)
{
	if (cursorHandler)
		cursorHandler->SetMouseCursor(index);
}
//-----------------------------------------------------------------------------------------------
