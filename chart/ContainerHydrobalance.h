#pragma once
#include "ModelContainers.h"

class ContainerHydrobalance : public ContainerParameter
{
public:
	ContainerHydrobalance (const ID& _id, const wstring& Name, const wstring& measure_unit, const COLORREF& Color, int LegendMark)
		: ContainerParameter(_id, Name, measure_unit, Color, LegendMark, FIELD_TYPE::HYDROBALANCE, 0)
		
	{
	}

	bool AllowedSave() const override { return true; }
	wstring getSumm() const override { return L""; }
	void calculateSumm() override {};

};