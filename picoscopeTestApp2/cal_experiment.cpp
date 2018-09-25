#include "stdafx.h"
#include "cal_experiment.h"


cal_experiment::cal_experiment()
{
}

cal_experiment::~cal_experiment()
{
}

void cal_experiment::runExperiment(string filename, Picoscope * pscope)
{
	std::ofstream fout;
	fout.open(filename, std::ofstream::out);
	fout << "Frequency" << ',' << "Mag" << ',' << "Phase" << '\n';

	for (experimentParams_t params : parameterList)
	{
		// set up channels
		pscope->configureChannel(1, params.range_chA);
		if (pscope->getNumChannels() == 2)
		{
			pscope->configureChannel(2, params.range_chB);
		}
		else
		{
			pscope->configureChannel(2, params.range_chA);
			pscope->configureChannel(3, params.range_chB);
		}

		// turn on signal generator
		pscope->turnOnSignalGen(params.frequency, params.amplitude);

		// get data, crunch the numbers
		vector<int16_t> A, B, C;
		ComplexNum_polar dataResult;
		if (pscope->getNumChannels() == 2)
		{
			pscope->getData_2ch(params.timebase, &params.numPoints, A, B);
			dataResult = NumberCruncher::CompareSignals(A, B, params.frequency, Picoscope::getTimebase(params.timebase));
		}
		else
		{
			pscope->getData_3ch(params.timebase, &params.numPoints, A, B, C);
			dataResult = NumberCruncher::CompareSignalsDiff(A, B, C, params.frequency, Picoscope::getTimebase(params.timebase));
		}
		cout << params.frequency << '\t' << dataResult.mag << '\t' << dataResult.phase << '\n';
		fout << params.frequency << ',' << dataResult.mag << ',' << dataResult.phase << '\n';
	}
}

void cal_experiment::setDefaultParameters(experimentParams_t params)
{
	_defaultParams = params;
}

void cal_experiment::appendParameters(experimentParams_t params)
{
	parameterList.push_back(params);
}

void cal_experiment::appendParameters(vector<double> freqList)
{
	for (double freq : freqList)
	{
		_defaultParams.frequency = freq;
		parameterList.push_back(_defaultParams);
	}
}