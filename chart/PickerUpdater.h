#pragma once
#include "stdafx.h"

class IDutyPickerUpdater
{
public: 
	virtual void Update(const COleDateTime&) = 0;
	virtual void setOpened(bool status) = 0;
};
