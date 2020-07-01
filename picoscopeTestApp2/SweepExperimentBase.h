#pragma once

#include <cstdint>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "NumberCruncher.h"
#include "Picoscope.h"

class SweepExperimentBase
{
public:
	const vector<ComplexNum_polar>* getRawData() { return &rawData; };
protected:
	void appendParameters(PicoscopeSamplingParams_t params);
	void calcSamplingParams();
	vector<ComplexNum_polar> rawData;
	Picoscope pscope;
	string _outputFilename;
	PicoscopeSamplingParams_t _defaultParams;
	vector<PicoscopeSamplingParams_t> parameterList;
};

