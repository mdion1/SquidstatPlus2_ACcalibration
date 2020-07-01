#include "stdafx.h"
#include <cstdio>
#include <iostream>
#include <math.h>
#include "Picoscope.h"
#include "NumberCruncher.h"
//#include "FrequencySweepExperiment.h"
#include "AmplitudeSweepExperiment.h"

#define AMPLITUDE_SWEEP
using namespace std;

int main(int argc, char* argv[])
{
	if (argc != 3)
		return 0;

#ifdef FREQUENCY_SWEEP
	FrequencySweepExperiment exp;
#elif defined AMPLITUDE_SWEEP
	AmplitudeSweepExperiment exp;
#endif

	exp.readExperimentParamsFile(argv[1]);
	exp.runExperiment();
	auto sweepRawData = exp.getRawData();
	if (sweepRawData->size() == 0)
	{
		system("pause");
		return 0;
	}

	/* write data */
	std::ofstream fout;
	fout.open(argv[2], std::ofstream::out);
	fout.precision(8);
	cout.precision(8);

#ifdef FREQUENCY_SWEEP
	NumberCruncher::NormalizeMag(data);		//todo: this won't work with const ptr
	fout << "Frequency" << ',' << "Relative Mag" << ',' << "Relative Phase" << '\n';
	for (int i = 0; i < data.size(); i++)
	{
		fout << data[i]. << ',' << data[i].mag << ',' << data[i].phase << '\n';
	}
#elif defined AMPLITUDE_SWEEP
	auto rawDataArray = sweepRawData->data();
	fout << "Excitation frequency: " << rawDataArray[0].frequency;
	fout << "Input Amplitude" << ',' << "Relative Mag" << ',' << "Relative Phase" << '\n';
	for (int i = 0; i < sweepRawData->size(); i++)
	{
		fout << rawDataArray[i].inputAmplitude << ',' << rawDataArray[i].mag << ',' << rawDataArray[i].phase << '\n';
	}
#endif

	system("pause");
	return 0;
}