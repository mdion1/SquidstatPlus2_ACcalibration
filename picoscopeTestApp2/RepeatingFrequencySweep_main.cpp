//#ifdef SINGLE_SWEEP_EXPERIMENT_MODE

#include "stdafx.h"
#include <cstdio>
#include <iostream>
#include <math.h>
#include "Picoscope.h"
#include "NumberCruncher.h"
#include "cal_experiment.h"
#include <filesystem>

using namespace std;

int main(int argc, char *argv[])
{
	/* Argument list:
		Amplitude, in V (double)
		Bias, in V (double)
		Starting frequency, in Hz (double)
		Ending frequency, in Hz (double)
		Total points (int)
	*/
	//if (argc < 3)
	//	return 0;

	cal_experiment exp;
	exp.buildSimpleSweep(0.1, 0, 1e6, 1e5, 20);

	while (true)
	{
		exp.rawData.clear();
		exp.runExperiment();
		
		/* write data */
		std::ofstream fout;
		string filename = std::filesystem::temp_directory_path().string() + "tempScopeSweepData.txt";
		fout.open(filename, std::ofstream::out);
		fout.precision(8);
		cout.precision(8);
		fout << "Frequency" << ',' << "Mag" << ',' << "Phase" << '\n';
		for (int i = 0; i < exp.rawData.size(); i++)
		{
			fout << exp.rawData[i].frequency << ',' << exp.rawData[i].mag << ',' << exp.rawData[i].phase << '\n';
		}
		fout.close();

		string keyInput;
		cin >> keyInput;
		if (keyInput[0] == 'y' || keyInput[0] == 'Y')
			continue;
		else
			break;
	}
	exp.closePicoscope();
	return 0;
}
//#endif