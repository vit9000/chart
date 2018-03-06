#pragma once

#include "TableLines.h"
#include "TableParameter.h"
#include "TableHemodynamic.h"

typedef shared_ptr<TableObject> CTableObject_Ptr;



class CTableBlock
{
private:
	
	wstring header;
	vector<CTableObject_Ptr> objects;
	Rect rect;
	int headerHeight;
public:
	CTableBlock()
		: header(L""),
		rect(Rect(0,0,1,1,1)),
		headerHeight (TableObject::LINE_HEIGHT)
	{
		rect.height = headerHeight;
	}
	//---------------------------------------------------------------------------
	CTableBlock(const wstring& BlockName, const Rect& rectangle)
		: header(BlockName),
		rect(rectangle),
		headerHeight(TableObject::LINE_HEIGHT)
	{
		rect.height = headerHeight;
	}
	//---------------------------------------------------------------------------
	size_t size() const
	{
		return objects.size();
	}
	//---------------------------------------------------------------------------
	const CTableObject_Ptr& operator[](size_t index) const
	{
		return objects.at(index);
	}
	//---------------------------------------------------------------------------
	void push_back(CTableObject_Ptr& TableObject)
	{
		objects.push_back(TableObject);
		rect.height += TableObject->getRect().height;
	}
	//---------------------------------------------------------------------------
	void resize(const Rect& rectangle)
	{
		rect.x = rectangle.x;
		rect.y = rectangle.y;
		rect.width = rectangle.width;
		rect.reserved = rectangle.reserved;
		//все кроме высоты
		Rect r(rect);
		r.y += headerHeight;
		for (size_t i = 0; i < objects.size(); ++i)
		{
			if (i > 0)
			{
				const Rect temp_rect = objects[i-1]->getRect();
				r.y = temp_rect.y + temp_rect.height;
			}
			objects[i]->Resize(r);
		}
	}
	//---------------------------------------------------------------------------
	const Rect& getRect() const
	{
		return rect;
	}
	//---------------------------------------------------------------------------
	void OnPaint(UGC& ugc) const
	{
		ugc.SetDrawColor(190, 190, 190);
		ugc.FillRectangle(rect.x, rect.y, rect.width, headerHeight);
		ugc.SetAlign(UGC::CENTER);
		ugc.SetDrawColor(0, 0, 0);
		ugc.SetTextSize(12);
		ugc.DrawString(header, rect.x + rect.width / 2, rect.y + headerHeight / 2 - ugc.GetTextHeight() / 2);
		
		ugc.SetAlign(UGC::LEFT);
		for (const auto& obj : objects)
			obj->OnPaint(ugc);
		ugc.SetDrawColor(0, 0, 0);
		ugc.DrawLine(rect.x, rect.y, rect.x + rect.width, rect.y, 2);
	}
	//---------------------------------------------------------------------------
};
