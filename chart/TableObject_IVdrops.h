#pragma once
#include "TableObjectMovable.h" // включает TableObject, TableObjectResizable
#include <memory>


class TableObject_IVdrops : public TableObjectResizable
{
	typedef std::shared_ptr<TableObject_IVdrops> Obj_Ptr;
protected:
	vector<Obj_Ptr> objects;
public:
	TableObject_IVdrops(IChartController* Controller, const ContainerUnit* containerUnit)
		: TableObjectResizable(Controller, containerUnit)
	{
		sort_type = IVDROPS;
		color = Gdiplus::Color::CornflowerBlue;
		
	}
	//-------------------------------------------------------------
	void OnPaint(UGC& ugc) override
	{
		int tempHeight = rect.height;
		rect.height = getDefaultHeight();
		TableObjectResizable::OnPaint(ugc);
		rect.height = tempHeight;


		for (const auto& obj : objects)
		{
			const Rect& r = obj->getRect();
			ugc.SetDrawColor(155, 155, 155);
			ugc.DrawDashLine(r.x, r.y, r.x + r.width, r.y);
			obj->mouseShift = mouseShift;
			obj->unitN = unitN;
			obj->FillRectangle = true;
			obj->OnPaint(ugc);
		}
		
	}
	//-------------------------------------------------------------
	bool OnLButtonUp(int x, int y) override
	{
		if (IsThisObject(x, y))
		{
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
				else
				{//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! ВРЕМЕННО
					ContainerUnit_Ptr new_container = controller->addDrugToDrug(getID());
					if (new_container)
					{
						objects.push_back(Obj_Ptr(new TableObject_IVdrops(controller, new_container)));
						controller->repaint();
					}
					//
				}
				
				return true;
			}
		}
		return false;
	}
	//-------------------------------------------------------------
	vector<ID> getAllIDs()
	{
		vector<ID> ids{ getID() };
		for (const auto& obj : objects)
			ids.push_back(obj->getID());
		return ids;
	}

	virtual void Resize(const Rect& rectangle)
	{
		rect.x = rectangle.x;
		rect.y = rectangle.y;
		rect.width = rectangle.width;
		rect.reserved = rectangle.reserved;

		
		Rect r(rect);
		rect.height = getDefaultHeight();
		r.y += rect.height;
		for (size_t i = 0; i < objects.size(); ++i)
		{
			if (i > 0)
			{
				const Rect temp_rect = objects[i - 1]->getRect();
				r.y = temp_rect.y + temp_rect.height;
				//r.y = rect.y+rect.height;
			}
			rect.height += objects[i]->getRect().height;
			objects[i]->Resize(r);
		}
	}
};