#include "stdafx.h"
#include <cstdio>
#include <iostream>
#include <math.h>
#include "Picoscope.h"
#include "NumberCruncher.h"
#include "cal_experiment.h"

#define AMPLITUDE_SWEEP
#define FREQUENCY_SWEEP
using namespace std;

int main(int argc, char *argv[])
{
	if (argc < 3)
		return 0;

	vector<ComplexNum_polar> data;
	cal_experiment exp;
	exp.readExperimentParamsFile(argv[1]);
	exp.runExperiment();
	data.insert(data.end(), exp.rawData.begin(), exp.rawData.end());

	//NumberCruncher::NormalizeMag(data);

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
		system("pause");
		return 0;
	}
}