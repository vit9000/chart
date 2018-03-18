#pragma once

#include <sstream>
#include "IChartController.h"
#include "CMainModel.h"
#include "MessageDialog.h"

#include "NewLineDialog.h"
#include "ValueInputDlg.h"
#include "CursorHandler.h"

class CMainController : public IChartController
{
private:
	CMainModel* model;
	CursorHandler * cursorHandler;
public:
	CMainController(CMainModel* Model, CursorHandler * cursor_handler)
		: IChartController(), model(Model), cursorHandler(cursor_handler)
	{}

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

	void addDrugToDrug(const ID& host_id) override
	{
		NewLineDialog dlg;
		if (dlg.DoModal() == IDOK)
		{
			model->addDrugToDrug(host_id, dlg.getType(), dlg.getString());
		}
	}


	void addDrugUnit(const ID& id, int start) override
	{
		ValueInputDlg dlg;
		dlg.Init(id.getBlockName(), { model->getContainerName(id) }, { L"" });
		if (dlg.DoModal() == IDOK)
		{
			const auto& value = dlg.getValue();
			model->addDrugUnit(id, value[0], start, 60);
		}
	}

	void addDrugUnits(const vector<ID>& ids, int start) override
	{
		ValueInputDlg dlg;
		vector<wstring> drugNames;
		vector<wstring> content;
		for (const ID& id : ids)
		{
			drugNames.push_back(model->getContainerName(id));
			content.push_back(L"");
		}

		dlg.Init(ids[0].getBlockName(), drugNames, content);
		if (dlg.DoModal() == IDOK)
		{
			const vector<Value>& values = dlg.getValue();
			model->addDrugUnits(ids, values, start, 60);
		}
	}

	void addParameterUnit(const ID& id, int start, const Rect& rect) override
	{
		function<void(const std::wstring&)> callBack = [this, id, start](const std::wstring& val) 
		{
			model->addParameterUnit(id, Value(val), start);
		};
		if (cursorHandler)
			cursorHandler->setEditBox(rect, callBack, L"");

		
	}

	void addParameterUnits(const vector<ID>& ids, int start) override
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

	void updateUnitValue(const ID& id, int unit_number, const Rect& rect) override
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

	void updateUnitValue(const ID& id, int unit_number) override
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


	void updateUnitValues(const vector<ID>& ids, int unit_number) override
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

	void updateUnitPosition(const ID& id, int unit_number, int start, int duration) override
	{
		model->updateUnitPosition(id, unit_number, start, duration);
	}

	void updateUnitPositions(const vector<ID>& ids, int unit_number, int start, int duration) override
	{
		model->updateUnitPositions(ids, unit_number, start, duration);
	}


	void repaint() override
	{
		model->NotifyEmpty();
	}

	void SetMouseCursor(size_t index) override
	{
		if(cursorHandler)
			cursorHandler->SetMouseCursor(index);
	}


	/*void setEditBox(const Rect& rect) override
	{
		if (cursorHandler)
			cursorHandler->setEditBox(rect);

	}*/
};

