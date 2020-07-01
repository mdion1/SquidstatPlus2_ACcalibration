#include "stdafx.h"
#include "FrequencySweepExperiment.h"

void FrequencySweepExperiment::runExperiment()
{
	for (experimentParams_frequency_sweep_t params : parameterList)
	{
		// set up channels
		pscope.configureChannel(1, params.range_chA);
		if (pscope.getNumChannels() == 2)
		{
			pscope.configureChannel(2, params.range_chB);
		}
		else if (pscope.getNumChannels() == 3)
		{
			pscope.configureChannel(2, params.range_chA);
			pscope.configureChannel(3, params.range_chB);
		}
		else if (pscope.getNumChannels() == 4)
		{
			pscope.configureChannel(2, params.range_chA);
			pscope.configureChannel(3, params.range_chB);
			pscope.configureChannel(4, params.range_chB);
		}

		// turn on signal generator
		pscope.turnOnSignalGen(params.frequency, params.amplitude, params.DCbias);
		
		// get data, crunch the numbers
		vector<int16_t> A, B, C, D;
		
		if (pscope.getNumChannels() == 2)
		{

			vector<ComplexNum_polar> averageVector;
			for (int i = 0; i < 5; i++)
			{
				pscope.getData_2ch(params.timebase, &params.numPoints, A, B);
				averageVector.push_back(NumberCruncher::CompareSignals(A, B, params.frequency, Picoscope::getTimebase(params.timebase)));
			}
			rawData.push_back(NumberCruncher::getAvg(averageVector));
		}
		else if (pscope.getNumChannels() == 3)
		{
			vector<ComplexNum_polar> averageVector;
			for (int i = 0; i < 5; i++)
			{
				pscope.getData_3ch(params.timebase, &params.numPoints, A, B, C);
				averageVector.push_back(NumberCruncher::CompareSignalsDiff(A, B, C, params.frequency, Picoscope::getTimebase(params.timebase)));
			}
			rawData.push_back(NumberCruncher::getAvg(averageVector));
		}
		/*else if (pscope.getNumChannels() == 4)
		{
			pscope.getData_4ch(params.timebase, &params.numPoints, A, B, C, D);
			dataResults.push_back(NumberCruncher::CompareSignalsDiff2(A, B, C, D, params.frequency, Picoscope::getTimebase(params.timebase)));
		}*/
		//cout << params.frequency << '\t' << dataResults.back().mag << '\t' << dataResults.back().phase << '\n';
		cout << rawData.back().frequency << '\t' << rawData.back().mag << '\t' << rawData.back().phase << '\n';
	}

	//NumberCruncher::NormalizeMag(dataResults);
	//for (int i = 0; i < dataResults.size(); i++)
	//{
	//	fout << parameterList_freqSweep[i].frequency << ',' << dataResults[i].mag << ',' << dataResults[i].phase << '\n';
	//}
	pscope.close();
}

void FrequencySweepExperiment::appendParameters(experimentParams_frequency_sweep_t params)
{
	parameterList.push_back(params);
}

void FrequencySweepExperiment::appendParameters(vector<double> freqList)
{
	for (double freq : freqList)
	{
		_defaultParams.frequency = freq;
		parameterList.push_back(_defaultParams);
	}
}

void FrequencySweepExperiment::readExperimentParamsFile(string filename)
{
	/* Template for frequency sweep :
		-DC_Bias_in_V
		-AC_Amplitude_in_V
		-VoltageRange1 (10mV, 20mV, 50mV, 100mV, 200mV, 500mV, 1V, 2V, 5V, 10V, 20V, 50V)
		-VoltageRange2 (10mV, 20mV, 50mV, 100mV, 200mV, 500mV, 1V, 2V, 5V, 10V, 20V, 50V)
		-NumberOfChannels (2 ==> Reports B/A; 3 ==> reports C / (A-B);)
		-List of frequencies (one per line) */

	ifstream file;
	file.open(filename);
	
	string str;
	getline(file, str);			//get DC bias
	_defaultParams.DCbias = atof(str.c_str());
	getline(file, str);			//get excitation signal amplitude
	_defaultParams.amplitude = atof(str.c_str());
	getline(file, str);			//get signal 1 range setting
	_defaultParams.range_chA = getRangeFromText(str);
	getline(file, str);			//get signal 2 range setting
	_defaultParams.range_chB = getRangeFromText(str);
	getline(file, str);			//get number of signals/channels
	int numChannels = atoi(str.c_str());
	pscope.open(numChannels);

	str.clear();

	do
	{
		getline(file, str);
		double frequency = atof(str.c_str());
		getFrequencies(frequency);
	} while (!file.eof());
}

void FrequencySweepExperiment::getFrequencies(double freq)
{
	experimentParams_frequency_sweep_t defaultParams = getDefaultParameters_freqSweep();
	defaultParams.frequency = freq;

	defaultParams.timebase = TIMEBASE_16NS;
	while (true)
	{
		int maxNumPoints = 1e5;
		double dt = Picoscope::getTimebase(defaultParams.timebase);
		int numCycles = 2;
		double numPoints = (numCycles / freq) / dt;
		if (numPoints > maxNumPoints)
			defaultParams.timebase = (scopeTimebase_t)((int)defaultParams.timebase + 1);
		else
		{
			defaultParams.numPoints = maxNumPoints;
			break;
		}
	}

	appendParameters(defaultParams);
}