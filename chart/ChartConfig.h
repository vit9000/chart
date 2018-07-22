#pragma once

class CChartConfig
{
private:
	int time_max; // длительность карты в минутах. Для реанимационной карты 1440 мин - 24 часа, наркозная карта переменной длительности
	int time_step; // шаг клеток в карте
public:
	CChartConfig() : time_max(1440), time_step(60) {}

	void setTimes(int MAX, int STEP)
	{
		time_max = MAX;
		time_step = STEP;
	}

	int getMaxMinute()
	{
		return time_max;
	}

	int getStep()
	{
		return time_step;
	}
};