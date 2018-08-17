#pragma once
enum class FIELD_TYPE{ TEXT = 0, NUMERIC_WITH_SUMM = 1, NUMERIC_WITHOUT_SUMM = 2, HYDROBALANCE = 3 };
enum class BLOCK_TYPE { STANDART=0, ADMINISTRATIONS=1, PLOT=2 };


struct BlockInfo
{
	int type;
	int scale_value_min;
	int scale_value_max;
};



