#include "stdafx.h"
#include <cstdio>
#include <iostream>
#include <math.h>
#include "Picoscope.h"
#include "NumberCruncher.h"
#include "cal_experiment.h"

//#define CONTROL_EXPERIMENT_200mV  //hook up all probes to the signal generator.
//#define CONTROL_EXPERIMENT_50mV  //hook up all probes to the signal generator.
//#define STAGE1_SETTING1_EXPERIMENT	//hook up probe 1 and signal generator to WE input, in series with 100Ohm resistor. Hook up probe 2 to TIA stage 1 output.
//#define STAGE1_SETTING2_EXPERIMENT  //hook up probe 1 and signal generator to WE input, in series with 10kOhm resistor. Hook up probe 2 to TIA stage 1 output.
//#define STAGE1_SETTING3_EXPERIMENT  //hook up probe 1 and signal generator to WE input, in series with 100kOhm resistor. Hook up probe 2 to TIA stage 1 output.
#define STAGE2_SETTING1_EXPERIMENT  //hook up signal generator to WE input, in series with 100Ohm resistor. Hook up probe 1 to TIA stage 1 output, and hook up probe 2 to TIA stage 2 output.

using namespace std;

int main(int argc, char *argv[])
{
	cal_experiment exp;
	exp.readExperimentParamsFile(argv[1]);
	experimentParams_t defaultParams = exp.getDefaultParameters();
	vector<double> freqList;
	for (int i = 20; i > 0; i--)		//frequencies 1M through 100kHz
	{
		freqList.push_back(i * 100e3);
	}
	exp.appendParameters(freqList);

	freqList.clear();
	defaultParams.timebase = TIMEBASE_160NS;
	exp.setDefaultParameters(defaultParams);
	for (int i = 9; i > 0; i--)		//frequencies 90k through 10kHz
	{
		freqList.push_back(i * 10e3);
	}
	exp.appendParameters(freqList);

	freqList.clear();
	defaultParams.timebase = TIMEBASE_1000NS;
	defaultParams.numPoints = 100000;
	exp.setDefaultParameters(defaultParams);
	for (int i = 9; i > 0; i--)		//frequencies 9k through 1kHz
	{
		freqList.push_back(i * 1e3);
	}
	exp.appendParameters(freqList);

	freqList.clear();
	defaultParams.timebase = TIMEBASE_10US;
	exp.setDefaultParameters(defaultParams);
	for (int i = 9; i > 0; i--)		//frequencies 900 through 100Hz
	{
		freqList.push_back(i * 100);
	}
	exp.appendParameters(freqList);

	exp.runExperiment();
	
exit:
	{
		system("pause");
		return 0;
	}
}