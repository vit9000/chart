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
		database.push_back(Patient(L"������ ���� ��������"));
		database.push_back(Patient(L"������ ���� ��������"));
		if(current < 0) current=0;
	}
};