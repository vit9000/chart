#pragma once
#include <map>
#include <string> 



/*
# внутривенный
--- капельный
--- болюсный
--- микроструйный
# внутримышечный
# подкожный
# энтеральный
--- per os
--- в зонд
# сублингвальный
# суббукальный
# ректальный
# спинальный
# эпидуральный
--- болюсный
--- микроструйный
# наружный
# ингал€ционный
# назальный
# ушные капли
# глазные капли
*/
#include <cstdint>

using namespace std;
class ADMINWAY
{
	uint32_t value;
public:
	ADMINWAY()
		: value(0)
	{
	}

	/*enum WAY
	{
		INTRAVENOUS_DROPS,
		INTRAVENOUS_BOLUS,
		INTRAVENOUS_INFUSION,
		INTRAMUSCULAR,
		SUBCUTANEOUS,
		ENTERAL,
		SUBLINGUAL,
		SUBBUCAL,
		RECTAL,
		SPINAL,
		EPIDURAL_BOLUS,
		EPIDURAL_INFUSION,
		EXTERNAL,
		INHALATION,
		NASAL,
		EYE_DROPS,
		EAR_DROPS
	};*/

	enum ADMIN_TYPE
	{
		COMBINED_DROPS,
		DROPS,
		BOLUS,
		INFUSION,
		DEFAULT
	};

	/*static int getAdminTypeByWay(int WAY)
	{
		switch (WAY)
		{
		case INTRAVENOUS_DROPS: 
			return DROPS;

		case INTRAVENOUS_BOLUS: 
		case EPIDURAL_BOLUS:
			return BOLUS;

		case EPIDURAL_INFUSION:
		case INTRAVENOUS_INFUSION: 
			return INFUSION;

		default: return DEFAULT;
		}
	}*/
	
	bool IsNull() const
	{
		return (value == 0);
	}

	void setAllOn()
	{
		value = UINT32_MAX;
	}
	
	void setAllOff()
	{
		value = 0;
	}

	void setOn(int WAY)
	{
		value |= (1 << WAY);
	}

	void setOff(int WAY)
	{
		value &= !(1 << WAY);
	}

	bool getStatus(int WAY) const
	{
		bool temp = ((value & (1 << WAY)) ? true : false);
		return temp;
	}

	void add(const ADMINWAY& new_adminway)
	{
		value |= new_adminway.value;
	}

};