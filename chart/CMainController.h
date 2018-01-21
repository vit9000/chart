#pragma once

#include <sstream>
#include "IChartController.h"
#include "MessageDlg.h"

class CMainController : public IChartController
{
public:
	CMainController(IModel* Model) : IChartController(Model){}

	virtual ~CMainController() 
	{

	}
	
	virtual void objectMouseUp(int id)
	{
		std::wstringstream ss;
		ss << L"ID = " << id;
		MessageDlg(L"Field Click", ss.str()).Show();
	};

	virtual void setDatabase(size_t index){ model->setDatabase(index);}
};