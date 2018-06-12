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
void CNurseController::showSmartMenu(int x, int y, const ID&id, int unit_number, MENU& menu)
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




	menu.push_back(
		make_pair(wstring(L"Удалить назначение"),
			[this, id, unit_number]()
	{
		if (MessageBox(0, L"Вы уверены, что хотите удалить назначение?", L"Подтверждение", MB_YESNO) == IDYES)
			model->deleteUnit(id, unit_number);
	})
	);

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
void CNurseController::updateUnitValue(const ID& id, int unit_number, const Rect& rect)
{
	function<void(const std::wstring&)> callBack = [this, id, unit_number](const std::wstring& val)
	{
		model->updateUnitValue(id, unit_number, Value(val));
	};
	if (cursorHandler)
	{
		std::wstringstream ss;
		ss << model->getCurrentPatient()->getContainerUnit(id)->getUnit(unit_number).getValue().getValue();

		cursorHandler->setEditBox(rect, callBack, ss.str());
	}
}
//-----------------------------------------------------------------------------------------------
void CNurseController::updateUnitValue(const ID& id, int unit_number)
{
	ValueInputDlg dlg;
	int dialog_type = ValueInputDlg::STANDART;
	std::wstringstream ss;
	ss << model->getCurrentPatient()->getContainerUnit(id)->getUnit(unit_number).getValue().getValue();
	dlg.Init(id.getBlockName(), { model->getContainerName(id) }, { ss.str() });

	if (dlg.DoModal() == IDOK)
	{
		const auto& value = dlg.getValue();
		model->updateUnitValue(id, unit_number, value[0]);
	}
}
//-----------------------------------------------------------------------------------------------
void CNurseController::updateUnitValues(const vector<ID>& ids, int unit_number)
{
	ValueInputDlg dlg;
	vector<wstring> paramNames;
	vector<wstring> content;
	for (const ID& id : ids)
	{
		paramNames.push_back(model->getContainerName(id));

		std::wstringstream ss;
		ss << model->getCurrentPatient()->getContainerUnit(id)->getUnit(unit_number).getValue().getValue();
		content.push_back(ss.str());
	}
	dlg.Init(ids[0].getBlockName(), paramNames, content);

	if (dlg.DoModal() == IDOK)
	{
		const auto& value = dlg.getValue();
		model->updateUnitValues(ids, unit_number, value);
	}
}
//-----------------------------------------------------------------------------------------------

void CNurseController::repaint()
{
	model->NotifyEmpty();
}
//-----------------------------------------------------------------------------------------------
