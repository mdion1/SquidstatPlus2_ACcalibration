#include "stdafx.h"
#include "SweepExperimentBase.h"

PS5000A_RANGE SweepExperimentBase::getRangeFromText(string str)
{
	if (!str.compare("10mV"))
		return PS5000A_10MV;
	else if (!str.compare("20mV"))
		return PS5000A_20MV;
	else if (!str.compare("50mV"))
		return PS5000A_50MV;
	else if (!str.compare("100mV"))
		return PS5000A_100MV;
	else if (!str.compare("200mV"))
		return PS5000A_200MV;
	else if (!str.compare("500mV"))
		return PS5000A_500MV;
	else if (!str.compare("1V"))
		return PS5000A_1V;
	else if (!str.compare("2V"))
		return PS5000A_2V;
	else if (!str.compare("5V"))
		return PS5000A_5V;
	else if (!str.compare("10V"))
		return PS5000A_10V;
	else if (!str.compare("20V"))
		return PS5000A_20V;
	else
		return PS5000A_50V;
}

void SweepExperimentBase::appendParameters(experimentParams_t params)
{
	parameterList.push_back(params);
}

void SweepExperimentBase::calcSamplingParams()
{
	_defaultParams.timebase = TIMEBASE_16NS;
	while (true)
	{
		int maxNumPoints = 1e5;
		double dt = Picoscope::getTimebase(_defaultParams.timebase);
		int numCycles = 2;
		double numPoints = (numCycles / _defaultParams.frequency) / dt;
		if (numPoints > maxNumPoints)
			_defaultParams.timebase = (scopeTimebase_t)((int)_defaultParams.timebase + 1);
		else
		{
			_defaultParams.numPoints = maxNumPoints;
			break;
		}
	}
}