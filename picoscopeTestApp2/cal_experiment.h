#pragma once

#include <cstdint>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "NumberCruncher.h"
#include "Picoscope.h"

#define MIN(x, y) (x < y ? x : y)
#define MAX(x, y) (x > y ? x : y)

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

typedef struct
{
	double frequency;
	double amplitude;
	double DCbias;
	PS5000A_RANGE range_chA;
	PS5000A_RANGE range_chB;
	scopeTimebase_t timebase;
	uint32_t numPoints;
	int num_repeats;
}experimentParams_amplitude_sweep_t;

class cal_experiment
{
public:
	cal_experiment();
	~cal_experiment();
	void setDefaultParameters(experimentParams_frequency_sweep_t params);
	void setDefaultParameters(experimentParams_amplitude_sweep_t params);
	experimentParams_frequency_sweep_t getDefaultParameters_freqSweep() { return _defaultParams_freqSweep; };
	experimentParams_amplitude_sweep_t getDefaultParameters_ampSweep() { return _defaultParams_ampSweep; };
	void appendParameters(experimentParams_frequency_sweep_t params);
	void appendParameters(experimentParams_amplitude_sweep_t params);
	void appendParameters(vector<double> freqList);
	void runExperiment();
	void readExperimentParamsFile(string filename);
	void getFrequencies(double freq);
	vector<ComplexNum_polar> rawData;

private:
	Picoscope pscope;
	string _outputFilename;
	experimentParams_amplitude_sweep_t _defaultParams_ampSweep;
	experimentParams_frequency_sweep_t _defaultParams_freqSweep;
	vector<experimentParams_amplitude_sweep_t> parameterList_ampSweep;
	vector<experimentParams_frequency_sweep_t> parameterList_freqSweep;
};
