#pragma once

class CChartConfig
{
private:
	int chart_type;
	int time_max; // длительность карты в минутах. Для реанимационной карты 1440 мин - 24 часа, наркозная карта переменной длительности
	int time_step; // шаг клеток в карте
	int min_duration;
	int count_steps;
public:
	CChartConfig() : time_max(1440), time_step(60), min_duration(30), count_steps(24) {}

	void setTimes(int CHART_TYPE, int MAX, int STEP, int MIN_DURATION)
	{
		chart_type = CHART_TYPE;
		time_max = MAX;
		time_step = STEP;
		min_duration = MIN_DURATION;
		count_steps = time_max / time_step;
	}

	void setMaxMinute(int MAX)
	{
		time_max = MAX;
		count_steps = time_max / time_step;
	}

	int getMaxMinute()
	{
		return time_max;
	}

	void setStep(int STEP)
	{
		time_step = STEP;
		count_steps = time_max / time_step;
	} 

	int getCountSteps()
	{
		return count_steps;
	}
	int getStep()
	{
		return time_step;
	}

	void setMinDuration(int MIN_DURATION)
	{
		min_duration = MIN_DURATION;
	}

	int getMinDuration()
	{
		return min_duration;
	}

	int getChartType() 
	{
		return chart_type;
	}
};