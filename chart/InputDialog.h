#pragma once
#include <exception>
#include "NewLineDialog.h"
#include "ValueInputDlg.h"

class InputDialog
{
private:
	
	CDialog * dlg;
public:
	enum {NEW_LINE_DIALOG, VALUE_INPUT_DIALOG};

	InputDialog(int TYPE=NEW_LINE_DIALOG)
	{
		switch(TYPE)
		{
			case NEW_LINE_DIALOG:
				dlg = new NewLineDialog();
				break;
			case VALUE_INPUT_DIALOG:
				dlg = new ValueInputDlg();
				break;
		}
	}
	virtual ~InputDialog()
	{
		delete dlg;
	}

	int Show()
	{
		return static_cast<int>(dlg->DoModal());
	}

	wstring getString()
	{
		NewLineDialog *temp = dynamic_cast<NewLineDialog*>(dlg);
		if(!temp)
			throw(std::bad_cast());
		return temp->getString();
	}

	int getType()
	{
		NewLineDialog *temp = dynamic_cast<NewLineDialog*>(dlg);
		if (!temp)
			throw(std::bad_cast());
		return temp->getType();
	}

	const pair<double, int>& getValues()
	{
		ValueInputDlg *temp = dynamic_cast<ValueInputDlg*>(dlg);
		if (!temp)
			throw(std::bad_cast());
		return temp->getValues();
	}
};