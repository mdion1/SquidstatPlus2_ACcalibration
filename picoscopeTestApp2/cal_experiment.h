#pragma once

#include <cstdint>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "NumberCruncher.h"
#include "Picoscope.h"
#include "argParse.h"

#define MIN(x, y) (x < y ? x : y)
#define MAX(x, y) (x > y ? x : y)

using namespace std;

typedef struct
{
	double frequency;
	scopeTimebase_t timebase;
	uint32_t numPoints;
}samplingParams_t;

class cal_experiment
{
public:
	void runExperiment(const inputParams_t& inputParams);
	void getFrequencies(double freq);
    const vector<vector<ComplexNum_polar>>& getRawData() { return rawData; };

private:
    double getScale(PS5000A_RANGE range);
    probeParams_t validateProbe(const probeParams_t& probe);
    vector<vector<ComplexNum_polar>> rawData;
	Picoscope pscope;
	string _outputFilename;
	vector<samplingParams_t> parameterList;
    vector<probeParams_t> scaledProbes;
};
