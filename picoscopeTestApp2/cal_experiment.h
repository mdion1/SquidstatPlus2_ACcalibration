#pragma once

#include <cstdint>
#include <iostream>
#include <fstream> 
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
	PS5000A_RANGE range_chA;
	PS5000A_RANGE range_chB;
	scopeTimebase_t timebase;
	uint32_t numPoints;
}experimentParams_t;

class cal_experiment
{
public:
	cal_experiment();
	~cal_experiment();
	void setDefaultParameters(experimentParams_t params);
	void appendParameters(experimentParams_t params);
	void appendParameters(vector<double> freqList);
	void runExperiment(string filename, Picoscope * pscope);

private:
	experimentParams_t _defaultParams;
	vector<experimentParams_t> parameterList;
};
