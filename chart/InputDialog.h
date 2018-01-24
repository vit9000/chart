#pragma once
#include <exception>
#include "NewLineDialog.h"

class InputDialog
{
private:
	
	CDialog * dlg;
public:
	enum {NEW_LINE_DIALOG};

	InputDialog(int TYPE=NEW_LINE_DIALOG)
	{
		switch(TYPE)
		{
			case NEW_LINE_DIALOG:
				dlg = new NewLineDialog();
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
			throw(std::bad_cast("Bad cast from CDialog* to NewLineDialog*"));
		return temp->getString();
	}
};