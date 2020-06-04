//#ifdef SINGLE_SWEEP_EXPERIMENT_MODE

#include "stdafx.h"
#include <cstdio>
#include <iostream>
#include <math.h>
#include "Picoscope.h"
#include "NumberCruncher.h"
#include "cal_experiment.h"
#include <filesystem>
#include "InputCheckerAsync.h"

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
	/* Parse arguments */
	if (argc < 6)
		return 0;
	double ACamp = atof(argv[1]);
	double DCBias = atof(argv[2]);
	double f_start = atof(argv[3]);
	double f_end = atof(argv[4]);
	int numPoints = atof(argv[5]);

	/* setup asynchronous keyboard input monitor */
	InputCheckerAsync mInputCheckerAsync;

	/* setup experiment sweep */
	cal_experiment exp;
	exp.buildSimpleSweep(ACamp, DCBias, f_start, f_end, numPoints);

	/* write data header */
	std::ofstream fout;
	string filename = std::filesystem::temp_directory_path().string() + "tempScopeSweepData.txt";
	fout.open(filename, std::ofstream::out);
	fout.precision(8);
	cout.precision(8);
	fout << "Frequency" << ',' << "Mag" << ',' << "Phase" << '\n';
	fout.close();

	while (true)
	{
		exp.rawData.clear();
		exp.runExperiment();
		
		/* write data */
		fout.open(filename, std::ofstream::app);
		for (int i = 0; i < exp.rawData.size(); i++)
		{
			fout << exp.rawData[i].frequency << ',' << exp.rawData[i].mag << ',' << exp.rawData[i].phase << '\n';
		}
		fout.close();

		if (mInputCheckerAsync.getKeyPressed() == 0)
			continue;
		else
			break;
	}
	exp.closePicoscope();
	return 0;
}
//#endif