#pragma once

#include <vector>
using std::vector;

#include <memory>
using std::shared_ptr;
//#include <boost/shared_ptr.hpp>
//using boost::shared_ptr;

#include "ugc.h"
#include "dpix.h"

#include "ModelContainers.h"
#include "ID.h"
#include "CTableBlock.h"
#include "CTableBlockHemodynamic.h"
#include "Constants.h"

typedef shared_ptr<CTableBlock> CTableBlock_Ptr;

class CTableContainer
{
private:
	map<wstring, CTableBlock_Ptr> table_lines;
	vector<wstring> blocks;
	IChartController** controller;
	Rect rect;
	const int MIN_HEADER_WIDTH;
	bool move_aborted;
	int SCROLL;
public: 

	int STEP_COUNT;
	//int HOUR_START;
	COleDateTime startTime;
	COleDateTime endTime;

	CTableContainer(IChartController** Controller, const Rect& rectangle)
		: controller(Controller),
		rect(rectangle),
		MIN_HEADER_WIDTH(static_cast<int>(160*DPIX())),
		move_aborted(false),
		SCROLL(0)
		
	{
		wstring temp;
		Default();
	}

	void setTimes(const COleDateTime& StartTime, const COleDateTime& EndTime)
	{
		wstring s = StartTime.Format(L"%Y-%m-%d %H:%M:%S");
		wstring e = EndTime.Format(L"%Y-%m-%d %H:%M:%S");

		startTime = StartTime;
		endTime = EndTime;
		COleDateTimeSpan d = EndTime - StartTime;
		int m = d.GetDays()*1440 + d.GetHours()*60 + d.GetMinutes();
		config->setMaxMinute((int)m);
		STEP_COUNT = config->getMaxMinute() / config->getStep();
	}

	void setScroll(int new_value, bool resize=true)
	{
		
		rect.y -= new_value-SCROLL;
		SCROLL = new_value;
		Resize(rect);
	}
	
	virtual ~CTableContainer()
	{
		Default();
	}

	void SetButtonsVisible()
	{
		for (auto& block : table_lines)
			block.second->SetButtonsVisible();
	}

	void Default()
	{
		blocks.clear();
		table_lines.clear();
		
		//blocks = ChartStructure::getInstance()->getBlocks();
		
	}
	//--------------------------------------------------
	size_t getBlocksCount()
	{
		return table_lines.size();
	}
	//--------------------------------------------------
	int getColumnWidth() const
	{
		return (rect.width - MIN_HEADER_WIDTH) / (STEP_COUNT +1);
	}
	//--------------------------------------------------
	int getHeaderWidth() const
	{
		return rect.width - getColumnWidth()*(STEP_COUNT +1);
	}
	//--------------------------------------------------
	int getContentHeight() const
	{
		int h = static_cast<int>(TableObject::LINE_HEIGHT*DPIX());

		for (const auto& blockname : blocks)
		{
			if (table_lines.count(blockname) == 0)
				continue;
			const CTableBlock& block = (*table_lines.at(blockname));
			h += block.getRect().height;
		}
		return h;
	}
	//--------------------------------------------------
	void DeleteContainerUnit(const ID& id)
	{
		table_lines[id.getBlockName()]->deleteTableObject(id);
	}
	//--------------------------------------------------
	void DeleteChildContainerUnit(const ID& parent_id, const ID& child_id)
	{
		CTableObject_Ptr obj = table_lines[child_id.getBlockName()]->getTableObject(parent_id);
		obj->deleteChild(child_id);
	}
	//--------------------------------------------------
	void moveDrug(const ID& id, int new_pos)
	{
		if (table_lines[id.getBlockName()]->move(id, new_pos))
			(*controller)->updateDrugPos(id, new_pos);
	}
	//--------------------------------------------------
	void AddBlock(const wstring& BlockName, int type)
	{
		if (table_lines.count(BlockName) > 0) return;
		if (type == static_cast<int>(BLOCK_TYPE::PLOT) || type== static_cast<int>(BLOCK_TYPE::PLOT_PA))
			table_lines[BlockName] = CTableBlock_Ptr(new CTableBlockHemodynamic(BlockName, rect, controller, type));
		else
			table_lines[BlockName] = CTableBlock_Ptr(new CTableBlock(BlockName, rect, controller));

		table_lines[BlockName]->AddButton(CTableBlock::BUTTON_TYPE::RESIZE);
		
		if (type == static_cast<int>(BLOCK_TYPE::ADMINISTRATIONS))
			table_lines[BlockName]->AddButton(CTableBlock::BUTTON_TYPE::ADMINISTRATIONS);

		blocks.push_back(BlockName);
	}
	//--------------------------------------------------
	void AddToBlock(const wstring& BlockName, const ContainerUnit* containerUnit, size_t pos)
	{
		CTableObject_Ptr object;

		if (const ContainerParameter * temp = dynamic_cast<const ContainerParameter*>(containerUnit))
			object = CTableObject_Ptr(new TableParameter(controller, temp));
		
		else if (const ContainerIVdrops * temp = dynamic_cast<const ContainerIVdrops*>(containerUnit))
			object = CTableObject_Ptr(new TableObject_IVdrops(controller, temp));

		else if (const ContainerInfusion * temp = dynamic_cast<const ContainerInfusion*>(containerUnit))
			object = CTableObject_Ptr(new TableObject_Pump(controller, temp));
		
		/*else if (const ContainerIVbolus * temp = dynamic_cast<const ContainerIVbolus*>(containerUnit))
			object = CTableObject_Ptr(new TableObject_IVbolus(controller, temp));*/
		
		else if (const ContainerUnitMovable * temp = dynamic_cast<const ContainerUnitMovable*>(containerUnit))
			object = CTableObject_Ptr(new TableObjectMovable(controller, temp));

		if (containerUnit->isChild())
		{
			CTableObject_Ptr parent_obj = table_lines[BlockName]->getTableObject(containerUnit->getParentID());
			if (parent_obj)
				parent_obj->addChild(containerUnit);
			
		}
		else
		{
			table_lines[BlockName]->insert(object, pos);
			object->addAddButton();
		}


		Resize(rect);
	}
	//--------------------------------------------------
	void OnPaint(UGC& ugc)
	{

		int tableHeight = static_cast<int>(TableObject::LINE_HEIGHT * ugc.getDPIX());
		int headerWidth = getHeaderWidth();
		int columnWidth = getColumnWidth();
		ugc.SetDrawColor(Gdiplus::Color::Gray);
		for (int i = 0; i <= STEP_COUNT; ++i)
		{
			int x = rect.x + headerWidth + i*columnWidth;
			ugc.DrawLine(x, 0, x, tableHeight + rect.height);
		}

		for (const wstring& block : blocks)
		{
			if (table_lines.count(block) == 0) continue;
			table_lines.at(block)->OnPaint(ugc);
		}

		ugc.SetDrawColor(255, 255, 255);
		ugc.FillRectangle(rect.x, 0, rect.x + rect.width, tableHeight);
		ugc.SetDrawColor(Gdiplus::Color::Gray);
		ugc.DrawLine(rect.x, tableHeight, rect.x + rect.width, tableHeight);
		
		ugc.SetAlign(UGC::CENTER);
		ugc.SetTextSize(12);
		

		COleDateTime dt = startTime;
		COleDateTimeSpan sp(0, 0, config->getStep(), 0);
		for (int i = 0; i <= STEP_COUNT; ++i)
		{
			int x = rect.x + headerWidth + i * columnWidth;
			ugc.DrawLine(x, 0, x, tableHeight);
			if (i == STEP_COUNT)
			{
				ugc.DrawString(L"Ñ", x + columnWidth / 2, tableHeight / 2 - ugc.GetTextHeight() / 2);
				break;
			}
			wstring time;
			if (config->getStep() == 60)
				time = dt.Format(L"%H").GetBuffer();
			else
				time = dt.Format(L"%H:%M").GetBuffer();
			ugc.DrawString(time, x + columnWidth / 2, tableHeight / 2 - ugc.GetTextHeight() / 2);

			dt += sp;
		}
		
		ugc.SetAlign(UGC::LEFT);
		
	}
	//--------------------------------------------------
	void Resize()
	{
		Resize(rect);
	}
	void Resize(const Rect& rectangle)
	{
		rect = Rect(rectangle);
		rect.height = 0;
		rect.reserved = getHeaderWidth();
		Rect r(rect);

		
		// ÐÅÑÀÉÇ ÏÎ ÂÑÅÌ ÏÎÊÀÇÀÒÅËßÌ
		for (const wstring& block : blocks)
		{
			if (table_lines.count(block) == 0) continue;
			CTableBlock& tableBlock = *(table_lines.at(block));
			tableBlock.resize(r);
			r.y += tableBlock.getRect().height;
			rect.height += tableBlock.getRect().height;
		}
	}
	//--------------------------------------------------
	bool OnRButtonUp(int x, int y)
	{
		for (const wstring& block : blocks)
			if (table_lines.count(block) > 0)
			{
				if (table_lines.at(block)->OnRButtonUp(x, y))
					return true;
			}
		return false;
	}
	//--------------------------------------------------
	bool OnLButtonUp(int x, int y)
	{
		if(move_aborted)
		{
			move_aborted = false;
			return false;
		}
		for (const wstring& block : blocks)
			if (table_lines.count(block) > 0)
			{
				if (table_lines.at(block)->OnLButtonUp(x, y))
					return true;
			}
		return false;
	}
	//--------------------------------------------------
	bool OnLButtonDown(int x, int y)
	{
		move_aborted = false;
		for (const wstring& block : blocks)
			if (table_lines.count(block) > 0)
			{
				if (table_lines.at(block)->OnLButtonDown(x, y))
					return true;
			}
		return false;
	}
	//--------------------------------------------------
	bool OnMouseMove(int x, int y)
	{
		bool status = false;
		for (const wstring& block : blocks)
			if (table_lines.count(block) > 0)
			{
				for (size_t i = 0; i < table_lines[block]->size(); ++i)
				{
					if (table_lines.at(block)->OnMouseMove(x, y, move_aborted))
						status = true;
				}
			}
		return status;
	}
	//--------------------------------------------------
	bool IsMoveAborted()
	{
		return move_aborted;
	}
};
