#include "stdafx.h"
#include <cstdio>
#include <iostream>
#include <math.h>
#include "Picoscope.h"
#include "NumberCruncher.h"
#include "cal_experiment.h"

//#define CONTROL_EXPERIMENT_200mV  //hook up all probes to the signal generator.
#define CONTROL_EXPERIMENT_50mV  //hook up all probes to the signal generator.
//#define STAGE1_SETTING1_EXPERIMENT	//hook up probe 1 and signal generator to WE input, in series with 100Ohm resistor. Hook up probe 2 to TIA stage 1 output.
//#define STAGE1_SETTING2_EXPERIMENT  //hook up probe 1 and signal generator to WE input, in series with 10kOhm resistor. Hook up probe 2 to TIA stage 1 output.
//#define STAGE2_SETTING1_EXPERIMENT  //hook up signal generator to WE input, in series with 100Ohm resistor. Hook up probe 1 to TIA stage 1 output, and hook up probe 2 to TIA stage 2 output.

using namespace std;

int main(int)
{
	// 1) open unit
	Picoscope pscope;
	if (!pscope.open())
	{
		goto exit;
	}

	{
		cal_experiment exp;
#ifdef CONTROL_EXPERIMENT_200mV
		experimentParams_t defaultParams = experimentParams_t{ 0, 1,PS5000A_200MV, PS5000A_200MV, TIMEBASE_16NS, 62500 };
		string filepath = "c:/picoscope_experiments/control_200mV.csv";
#endif
#ifdef CONTROL_EXPERIMENT_50mV
		experimentParams_t defaultParams = experimentParams_t{ 0, 0.25,PS5000A_50MV, PS5000A_50MV, TIMEBASE_16NS, 62500 };
		string filepath = "c:/picoscope_experiments/control_50mV.csv";
#endif
#ifdef STAGE1_SETTING1_EXPERIMENT
		experimentParams_t defaultParams = experimentParams_t{ 0, 1,PS5000A_50MV, PS5000A_50MV, TIMEBASE_16NS, 62500 };
		string filepath = "c:/picoscope_experiments/stage1setting1.csv";
#endif
#ifdef STAGE1_SETTING2_EXPERIMENT
		experimentParams_t defaultParams = experimentParams_t{ 0, 1,PS5000A_200MV, PS5000A_200MV, TIMEBASE_16NS, 62500 };
		string filepath = "c:/picoscope_experiments/stage1setting2.csv";
#endif
		exp.setDefaultParameters(defaultParams);
		vector<double> freqList;
		for (int i = 10; i > 0; i--)		//frequencies 1M through 100kHz
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

		exp.runExperiment(filepath, &pscope);
	}

	// 5) close scope
	pscope.close();

exit:
	{
		cout << "Press <char> + <Enter> to quit.";
		char x;
		cin >> x;
		return 0;
	}
}