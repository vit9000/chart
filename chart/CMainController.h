#pragma once

#include <sstream>
#include "IChartController.h"
#include "MessageDlg.h"

class CMainController : public IChartController
{
public:
	virtual ~CMainController() 
	{

	}
	
	virtual void ObjectMouseUp(int id)
	{
		std::wstringstream ss;
		ss << L"ID = " << id;
		MessageDlg(L"Field Click", ss.str()).Show();
	};
};