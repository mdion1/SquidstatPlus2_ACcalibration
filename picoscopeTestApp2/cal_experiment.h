#pragma once

#include <cstdint>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "NumberCruncher.h"
#include "Picoscope.h"
#include "siglent_scope.h"

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
	experimentParams_t getDefaultParameters() { return _defaultParams; };
	void appendParameters(experimentParams_t params);
	void appendParameters(vector<double> freqList);
	void runExperiment();
	void readExperimentParamsFile(string comPortAddr, string filename);
	void getFrequencies(double freq);
	vector<ComplexNum_polar> rawData;

private:
	Picoscope pscope;
	siglent_scope sig_scope;
	string _outputFilename;
	experimentParams_t _defaultParams;
	vector<experimentParams_t> parameterList;
};
