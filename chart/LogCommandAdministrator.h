#pragma once
#include <vector>
#include <memory>
#include "IModel.h"
#include "ILogCommand.h"



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
private:
	void setEnabled();
	bool isUndoAvailable();
	bool isRedoAvailable();
};
