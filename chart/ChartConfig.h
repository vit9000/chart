#pragma once

class CChartConfig
{
private:
	int time_max; // ������������ ����� � �������. ��� �������������� ����� 1440 ��� - 24 ����, ��������� ����� ���������� ������������
	int time_step; // ��� ������ � �����
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