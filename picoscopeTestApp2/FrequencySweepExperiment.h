#pragma once

#include "SweepExperimentBase.h"

using namespace std;

typedef struct
{
	double frequency;
	double amplitude;
	double DCbias;
	PS5000A_RANGE range_chA;
	PS5000A_RANGE range_chB;
	scopeTimebase_t timebase;
	uint32_t numPoints;
}experimentParams_frequency_sweep_t;

class FrequencySweepExperiment : SweepExperimentBase
{
public:
	experimentParams_frequency_sweep_t getDefaultParameters_freqSweep() { return _defaultParams; };
	void appendParameters(experimentParams_frequency_sweep_t params);
	void appendParameters(vector<double> freqList);
	void runExperiment();
	void readExperimentParamsFile(string filename);
	void getFrequencies(double freq);
	vector<ComplexNum_polar> rawData;

private:
	experimentParams_frequency_sweep_t _defaultParams;
	vector<experimentParams_frequency_sweep_t> parameterList;
};
