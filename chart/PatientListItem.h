#pragma once
#include "CustomListViewItem.h"
#include "PatientInfo.h"

class CPatientListItem : public CCustomListViewItem
{
private:
	const PatientInfo* patient;
public:
	CPatientListItem(const PatientInfo* patientInfo, int Height, const function<void()>& CallBack) 
		: CCustomListViewItem(Height, CallBack), patient(patientInfo) {}

	void OnPaint(UGC& ugc, int y, int width) override
	{
		int border = static_cast<int>(DPIX() * 10);
		
		ugc.SetTextSize(12);
		ugc.DrawString((*patient)[PatientInfo::FIO], border, y);
		
		y += border * 2;
		ugc.SetTextSize(10);
		ugc.DrawString(L"Возраст: " + (*patient)[PatientInfo::AGE], border, y);
	}

};