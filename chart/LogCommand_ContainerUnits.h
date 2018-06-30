#pragma once

#include <vector>

#include "ILogCommand.h"
#include "ContainerUnit.h"

using namespace std;




class LogCommand_AddDrug : public ILogCommand
{
	ContainerUnit container;

public:
	LogCommand_AddDrug(const ContainerUnit& new_container)
		: ILogCommand(ID()),
		container(new_container)
	{}

	void undo(IModel& model, bool redraw = true) override
	{
		model.deleteDrug(container.getID());
	}

	void redo(IModel& model, bool redraw = true) override
	{
		const DrugInfo& di = container.getDrugInfo();
		model.addDrug(container.getID(), di.selected_admin_way, di, container.getMapUnits());
	}
};
//-----------------------------------------------------
class LogCommand_AddDrugToDrug : public ILogCommand
{
	ContainerUnit container;

public:
	LogCommand_AddDrugToDrug(const ID& parent_id, const ContainerUnit& new_container)
		: ILogCommand(parent_id),
		container(new_container)
	{}

	void undo(IModel& model, bool redraw = true) override
	{
		model.deleteChildDrug(container.getID());
	}

	void redo(IModel& model, bool redraw = true) override
	{
		const DrugInfo& di = container.getDrugInfo();
		model.addChildDrug(container.getID(), id, di, container.getMapUnits());
	}
};
//-----------------------------------------------------
class LogCommand_DeleteDrug : public ILogCommand
{
	ContainerUnit container; 
public:
	LogCommand_DeleteDrug(const ContainerUnit& _container)
		: ILogCommand(ID()),
		container(_container)
	{}

	void undo(IModel& model, bool redraw = true) override
	{
		const DrugInfo& di = container.getDrugInfo();
		model.addDrug(container.getID(), di.selected_admin_way, di, container.getMapUnits());
	}

	void redo(IModel& model, bool redraw = true) override
	{
		model.deleteDrug(container.getID());
	}
};
//-----------------------------------------------------
class LogCommand_DeleteChildDrug : public ILogCommand
{
	ContainerUnit container;
public:
	LogCommand_DeleteChildDrug(const ContainerUnit& _container)
		: ILogCommand(_container.getParentID()),
		container(_container)
	{}

	void undo(IModel& model, bool redraw = true) override
	{
		const DrugInfo& di = container.getDrugInfo();
		model.addChildDrug(container.getID(), id, di, container.getMapUnits());
	}

	void redo(IModel& model, bool redraw = true) override
	{
		model.deleteDrug(container.getID()); // DeleteChildDrug !!!!
	}
};

