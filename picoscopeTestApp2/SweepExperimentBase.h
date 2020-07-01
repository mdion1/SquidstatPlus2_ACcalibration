#pragma once

#include <cstdint>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "NumberCruncher.h"
#include "Picoscope.h"

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
}experimentParams_t;

class SweepExperimentBase
{
public:
	const vector<ComplexNum_polar>* getRawData() { return &rawData; };
protected:
	static PS5000A_RANGE getRangeFromText(string str);
	void appendParameters(experimentParams_t params);
	void calcSamplingParams();
	vector<ComplexNum_polar> rawData;
	Picoscope pscope;
	string _outputFilename;
	experimentParams_t _defaultParams;
	vector<experimentParams_t> parameterList;
};

