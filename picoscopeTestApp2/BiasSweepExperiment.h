#pragma once

#include "SweepExperimentBase.h"

using namespace std;

class BiasSweepExperiment : public SweepExperimentBase
{
public:
	void runExperiment();
	void readExperimentParamsFile(string filename);
};
