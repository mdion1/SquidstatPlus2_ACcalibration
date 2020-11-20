#include "stdafx.h"
#include "BiasSweepExperiment.h"

void BiasSweepExperiment::runExperiment()
{
	// set up channels
	pscope.configureChannel(1, parameterList[0].range_chA);
	pscope.configureChannel(2, parameterList[0].range_chB);

	for (PicoscopeSamplingParams_t params : parameterList)
	{
		// turn on signal generator
		pscope.turnOnSignalGen(params.frequency, params.amplitude, params.DCbias);

		// get data, crunch the numbers
		vector<int16_t> A, B;
		vector<ComplexNum_polar> averageVector;
		for (int i = 0; i < params.num_repeats; i++)
		{
			pscope.getData_2ch(params.timebase, &params.numPoints, A, B);
			averageVector.push_back(NumberCruncher::CompareSignals(A, B, &pscope, params));
		}
		rawData.push_back(NumberCruncher::getAvg(averageVector));
		cout << rawData.back().biasIn << '\t' << rawData.back().biasOut << '\t' << rawData.back().mag << '\t' << rawData.back().phase << '\n';
	}
	pscope.close();
}

void BiasSweepExperiment::readExperimentParamsFile(string filename)
{
	/* Template for amplitude sweep :
		-Number of repeats per point
		-DC_Bias_in_V
		-frequency_in_Hz
		-VoltageRange1 (10mV, 20mV, 50mV, 100mV, 200mV, 500mV, 1V, 2V, 5V, 10V, 20V, 50V)
		-VoltageRange2 (10mV, 20mV, 50mV, 100mV, 200mV, 500mV, 1V, 2V, 5V, 10V, 20V, 50V)
		-NumberOfChannels (2 ==> Reports B/A; 3 ==> reports C / (A-B);)
		-List of amplitudes (V) (one per line) */

	ifstream file;
	file.open(filename);

	string str;
	getline(file, str);			//get number of repeated points
	_defaultParams.num_repeats = atof(str.c_str());
	getline(file, str);			//get DC bias
	_defaultParams.amplitude = atof(str.c_str());
	getline(file, str);			//get excitation signal frequency
	_defaultParams.frequency = atof(str.c_str());
	calcSamplingParams();
	getline(file, str);			//get signal 1 range setting
	_defaultParams.range_chA = Picoscope::getRangeFromText(str);
	getline(file, str);			//get signal 2 range setting
	_defaultParams.range_chB = Picoscope::getRangeFromText(str);
	getline(file, str);			//get number of signals/channels
	int numChannels = atoi(str.c_str());
	pscope.open(numChannels);

	str.clear();
	do
	{
		getline(file, str);
		_defaultParams.DCbias = atof(str.c_str());
		appendParameters(_defaultParams);
	} while (!file.eof());
}