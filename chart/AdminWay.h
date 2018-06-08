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
	uint64_t value;
public:
	ADMINWAY()
		: value(0)
	{
	}

	enum
	{
		INTRAVENOUS = 1,
		INTRAMUSCULAR,
		SUBCUTANEOUS,
		ENTERAL,
		SUBLINGUAL,
		SUBBUCAL,
		RECTAL,
		SPINAL,
		EPIDURAL,
		EXTERNAL,
		INHALATION,
		NASAL,
		EYE_DROPS,
		EAR_DROPS
	};

	
	enum class _INTRAVENOUS
	{
		DROPS = 32,
		BOLUS = 33,
		INFUSION = 34
	};
	enum class _ENTERAL
	{
		PER_OS = 35,
		CATHETER = 36
	};
	enum class _EPIDURAL
	{
		BOLUS = 37,
		INFUSION = 38
	};

	void setAllOn()
	{
		value = UINT32_MAX;
	}
	void setAllOff()
	{
		value = 0;
	}

	void setOn(int TYPE)
	{
		value |= (1 << TYPE);
	}

	void setOff(int TYPE)
	{
		value &= !(1 << TYPE);
	}

	bool getStatus(int TYPE)
	{
		return ((value & (1 << TYPE)) == 1);
	}

	static int getRootMaxSize()
	{
		return sizeof(uint64_t) * 4;
	}

};