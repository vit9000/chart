#pragma once
#include <vector>
#include <memory>
#include "IModel.h"
#include "ILogCommand.h"
#include <set>



using namespace std;


class LogCommandAdministrator
{
private:
	int cursor;
	vector<LogCommandPtr> commands;
	CMenu * editMenu;
public:
	LogCommandAdministrator() : cursor(-1), editMenu(nullptr) {}
	void setEditMenu(CMenu * menu) { editMenu = menu; setEnabled(); }
	void push_back(const LogCommandPtr& command);
	bool undo(IModel& model);
	bool redo(IModel& model);
	void reset();
	void getUpdatedUnitsIDs(set<wstring>& containers_ids, set<wstring>& unit_ids);
private:
	void buildUpdatedUnits(set<wstring>& ids, LogCommandPtr& command);
	void trim();
	void setEnabled();
	bool isUndoAvailable();
	bool isRedoAvailable();
};
