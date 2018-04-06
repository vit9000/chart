#pragma once
#include "TableObjectMovable.h" // включает TableObject, TableObjectResizable
#include "Button.h"
#include <memory>


class TableObject_IVdrops : public TableObjectResizable
{

protected:
	
public:
	TableObject_IVdrops(IChartController* Controller, const ContainerUnit* containerUnit)
		: TableObjectResizable(Controller, containerUnit)
	{
		
		
	}
	//-------------------------------------------------------------
	void OnPaint(UGC& ugc) override
	{
		int tempHeight = rect.height;
		

		for (const auto& obj : child_objects)
		{

			const Rect& r = obj->getRect();
			ugc.SetDrawColor(155, 155, 155);
			ugc.DrawDashLine(r.x, r.y, r.x + r.width, r.y);

			TableObject_IVdrops* IVobj = static_cast<TableObject_IVdrops*>(obj.get());
			IVobj->mouseShift = mouseShift;
			IVobj->unitN = unitN;
			IVobj->OnPaint(ugc);

			

		}
		/*if (child_objects.size() > 0)
		{
			ugc.SetDrawColor(color);
			int bitW = static_cast<int>(2 * ugc.getDPIX());
			ugc.DrawUnitedForm(rect.x + bitW, rect.y + bitW, bitW * 3, rect.height - bitW * 2, 2);
		}*/

		rect.height = getDefaultHeight();
		TableObjectResizable::OnPaint(ugc);
		rect.height = tempHeight;
		
		
	}
	//------------------------------------------------------------
	virtual void addAddButton()
	{
		button = new Button(L"+");
		button->func = [this]() {
			controller->addDrugToDrug(getID());
		};
		button->repaint = [this]()
		{
			controller->repaint();
		};
	}
	//-------------------------------------------------------------
	bool OnLButtonUp(int x, int y) override
	{
		if (IsThisObject(x, y))
		{
			if (button && button->OnLButtonUp(x, y))
				return true;

			if (controller)
			{
				if (x > rect.x + rect.reserved)
				{
					if (mouseShift.is_action() && mouseShift.getShift() != 0)
					{
						// отправить запрос на обновление Юнита
						int start(0), duration(0);
						getPosition(start, duration);
						controller->updateUnitPositions(getAllIDs(), unitN, start, duration);
					}
					else
					{
						mouseShift.reset();
						if (unitN >= 0)
							controller->updateUnitValues(getAllIDs(), unitN);
						else
							controller->addDrugUnits(getAllIDs(), static_cast<int>(getMinuteByX(x)));
					}
					unitN = -1;
				}
				return true;
			}
		}
		return false;
	}
	//-------------------------------------------------------------
	void addChild(const ContainerUnit* new_container) override
	{
		if (new_container)
		{
			child_objects.push_back(Obj_Ptr(new TableObject_IVdrops(controller, new_container)));
			controller->repaint();
		}
	}
	//-------------------------------------------------------------
	virtual void Resize(const Rect& rectangle)
	{
		rect.x = rectangle.x;
		rect.y = rectangle.y;
		rect.width = rectangle.width;
		rect.reserved = rectangle.reserved;

		Rect r(rect);
		rect.height = getDefaultHeight();
		r.y += rect.height;
		for (size_t i = 0; i < child_objects.size(); ++i)
		{
			if (i > 0)
			{
				const Rect temp_rect = child_objects[i - 1]->getRect();
				r.y = temp_rect.y + temp_rect.height;
				//r.y = rect.y+rect.height;
			}
			rect.height += child_objects[i]->getRect().height;
			child_objects[i]->Resize(r);
		}
		ResizeButton();
	}

	
};