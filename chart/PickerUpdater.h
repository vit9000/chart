#pragma once
#include "stdafx.h"

class IPickerUpdater
{
public: 
	virtual void Update(const COleDateTime&) = 0;
};

class ICloser
{
public:
	virtual void Close() = 0;
};
