#pragma once

#include "SweepExperimentBase.h"

using namespace std;

class AmplitudeSweepExperiment : public SweepExperimentBase
{
public:
	void runExperiment();
	void readExperimentParamsFile(string filename);
};
