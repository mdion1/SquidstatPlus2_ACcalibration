#include "stdafx.h"
#include "SweepExperimentBase.h"

void SweepExperimentBase::appendParameters(PicoscopeSamplingParams_t params)
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