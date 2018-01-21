#pragma once

#include "IModel.h"

class CMainModel : public IModel
{
public:
	CMainModel() : IModel()
	{ loadDatabase();}
	
	
protected:
	virtual void loadDatabase()
	{
		database.push_back(Patient(L"Иванов Иван Иванович"));
		database.push_back(Patient(L"Петров Петр Петрович"));
		if(current < 0) current=0;
	}
};