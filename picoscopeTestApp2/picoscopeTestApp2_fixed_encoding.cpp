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
	/*cal_experiment exp;
	exp.readExperimentParamsFile(argv[1], argv[2]);
	exp.runExperiment();*/

	if (argc < 3)
		return 0;

	vector<ComplexNum_polar> data;
	string comport = argv[1];
	for (int i = 2; i < argc - 1; i++)
	{
		cal_experiment exp;
		exp.readExperimentParamsFile(comport, argv[i]);
		exp.runExperiment();
		data.insert(data.end(), exp.rawData.begin(), exp.rawData.end());
	}

	NumberCruncher::NormalizeMag(data);

	/* write data */
	std::ofstream fout;
	fout.open(argv[argc - 1], std::ofstream::out);
	fout.precision(8);
	cout.precision(8);
	fout << "Frequency" << ',' << "Mag" << ',' << "Phase" << '\n';
	for (int i = 0; i < data.size(); i++)
	{
		fout << data[i].frequency << ',' << data[i].mag << ',' << data[i].phase << '\n';
	}
	
exit:
	{
		//system("pause");
		return 0;
	}
}