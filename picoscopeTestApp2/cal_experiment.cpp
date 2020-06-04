#include "stdafx.h"
#include "cal_experiment.h"


cal_experiment::cal_experiment()
{
}

cal_experiment::~cal_experiment()
{
}

void cal_experiment::runExperiment()
{
	for (experimentParams_t params : parameterList)
	{
		cout << params.frequency << "\n";
		// turn on signal generator
		pscope.turnOnSignalGen(params.frequency, params.amplitude, params.DCbias);

		// get data, crunch the numbers
		vector<int16_t> A, B;
		vector<ComplexNum_polar> averageVector;
		pscope.getData_2ch(params.timebase, &params.numPoints, A, B);
		//averageVector.push_back(NumberCruncher::CompareSignals(A, B, params.frequency, Picoscope::getTimebase(params.timebase)));
		//rawData.push_back(NumberCruncher::getAvg(averageVector));
		rawData.push_back(NumberCruncher::CompareSignals(A, B, params.frequency, Picoscope::getTimebase(params.timebase)));
	}
}

void cal_experiment::closePicoscope()
{
	pscope.close();
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

void cal_experiment::buildSimpleSweep(double ACamp, double DCbias, double freq_start, double freq_end, int numPoints)
{
	_defaultParams.amplitude = ACamp;
	_defaultParams.DCbias = DCbias;
	_defaultParams.range_chA = _defaultParams.range_chB = getRangeFromInputAmplitude(ACamp);

	// set up scope
	pscope.open(2);
	pscope.configureChannel(1, _defaultParams.range_chA);
	pscope.configureChannel(2, _defaultParams.range_chB);

	vector<double> freqList = getFrequencyList(freq_start, freq_end, numPoints);
	for (int i = 0; i < freqList.size(); i++)
	{
		getFrequencies(freqList[i]);
	}
}

void cal_experiment::readExperimentParamsFile(string filename)
{
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

void cal_experiment::getFrequencies(double freq)
{
	experimentParams_t defaultParams = getDefaultParameters();
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

PS5000A_RANGE getRangeFromText(string str)
{
	if (!str.compare("10mV"))
		return PS5000A_10MV;
	else if (!str.compare("20mV"))
		return PS5000A_20MV;
	else if (!str.compare("50mV"))
		return PS5000A_50MV;
	else if (!str.compare("100mV"))
		return PS5000A_100MV;
	else if (!str.compare("200mV"))
		return PS5000A_200MV;
	else if (!str.compare("500mV"))
		return PS5000A_500MV;
	else if (!str.compare("1V"))
		return PS5000A_1V;
	else if (!str.compare("2V"))
		return PS5000A_2V;
	else if (!str.compare("5V"))
		return PS5000A_5V;
	else if (!str.compare("10V"))
		return PS5000A_10V;
	else if (!str.compare("20V"))
		return PS5000A_20V;
	else
		return PS5000A_50V;
}

PS5000A_RANGE getRangeFromInputAmplitude(double ACexcitationAmp)
{
	/* ACexcitationAmp is giving in volts, pk-pk */
	const double rangeMax[12] = { 50, 20, 10, 5, 2, 1, 0.5, 0.2, 0.1, 0.05, 0.02, 0.01 };
	const PS5000A_RANGE rangeList[12] = { PS5000A_50V, PS5000A_20V, PS5000A_10V, PS5000A_5V, PS5000A_2V, PS5000A_1V, PS5000A_500MV, PS5000A_200MV, PS5000A_100MV, PS5000A_50MV, PS5000A_20MV, PS5000A_10MV };
	double margin = 0.8;
	int index = 0;
	while (ACexcitationAmp / 2 / margin < rangeMax[index + 1])
	{
		index++;
		if (index == 11)
			break;
	}
	return rangeList[index];
}

vector<double> getFrequencyList(double freq_start, double freq_end, int numPoints)
{
	vector<double> ret;
	if (numPoints < 1 || freq_start == 0 || freq_end == 0)
		return ret;

	double ratio = pow(freq_end / freq_start, 1.0 / (numPoints - 1));
	for (int i = 0; i < numPoints; i++)
	{
		ret.push_back(freq_start * pow(ratio, i));
	}

	return ret;
}