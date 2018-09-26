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
	std::ofstream fout;
	fout.open(_outputFilename, std::ofstream::out);
	fout << "Frequency" << ',' << "Mag" << ',' << "Phase" << '\n';

	vector<ComplexNum_polar> dataResults;
	for (experimentParams_t params : parameterList)
	{
		// set up channels
		pscope.configureChannel(1, params.range_chA);
		if (pscope.getNumChannels() == 2)
		{
			pscope.configureChannel(2, params.range_chB);
		}
		else
		{
			pscope.configureChannel(2, params.range_chA);
			pscope.configureChannel(3, params.range_chB);
		}

		// turn on signal generator
		pscope.turnOnSignalGen(params.frequency, params.amplitude);
		
		// get data, crunch the numbers
		vector<int16_t> A, B, C;
		
		if (pscope.getNumChannels() == 2)
		{
			pscope.getData_2ch(params.timebase, &params.numPoints, A, B);
			dataResults.push_back(NumberCruncher::CompareSignals(A, B, params.frequency, Picoscope::getTimebase(params.timebase)));
		}
		else
		{
			pscope.getData_3ch(params.timebase, &params.numPoints, A, B, C);
			dataResults.push_back(NumberCruncher::CompareSignalsDiff(A, B, C, params.frequency, Picoscope::getTimebase(params.timebase)));
		}
		cout << params.frequency << '\t' << dataResults.back().mag << '\t' << dataResults.back().phase << '\n';
		
	}
	NumberCruncher::NormalizeMag(dataResults);
	for (int i = 0; i < dataResults.size(); i++)
	{
		fout << parameterList[i].frequency << ',' << dataResults[i].mag << ',' << dataResults[i].phase << '\n';
	}
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

PS5000A_RANGE getRangeFromText(string str);
scopeTimebase_t getTimebaseFromText(string str);

void cal_experiment::readExperimentParamsFile(string filename)
{
	ifstream file;
	file.open(filename);
	
	string str;
	getline(file, _outputFilename);		//get output filename
	getline(file, str, ',');			//get default frequency (discard)
	getline(file, str, ',');			//get excitation signal amplitude
	_defaultParams.amplitude = atof(str.c_str());
	getline(file, str, ',');			//get signal 1 range setting
	_defaultParams.range_chA = getRangeFromText(str);
	getline(file, str, ',');			//get signal 2 range setting
	_defaultParams.range_chB = getRangeFromText(str);
	getline(file, str, ',');			//get timebase setting
	_defaultParams.timebase = getTimebaseFromText(str);
	getline(file, str);			//get number of samples
	_defaultParams.numPoints = atoi(str.c_str());
	getline(file, str);			//get number of signals/channels
	int numChannels = atoi(str.c_str());
	pscope.open(numChannels);

	str.clear();
	getline(file, str);		//get command-line argument for squidstat-controlling program
	if (!str.empty())
		system(str.c_str());
}

PS5000A_RANGE getRangeFromText(string str)
{
	if (!str.compare("PS5000A_10MV"))
		return PS5000A_10MV;
	else if (!str.compare("PS5000A_20MV"))
		return PS5000A_20MV;
	else if (!str.compare("PS5000A_50MV"))
		return PS5000A_50MV;
	else if (!str.compare("PS5000A_100MV"))
		return PS5000A_100MV;
	else if (!str.compare("PS5000A_200MV"))
		return PS5000A_200MV;
	else if (!str.compare("PS5000A_500MV"))
		return PS5000A_500MV;
	else if (!str.compare("PS5000A_1V"))
		return PS5000A_1V;
	else if (!str.compare("PS5000A_2V"))
		return PS5000A_2V;
	else if (!str.compare("PS5000A_5V"))
		return PS5000A_5V;
	else if (!str.compare("PS5000A_10V"))
		return PS5000A_10V;
	else if (!str.compare("PS5000A_20V"))
		return PS5000A_20V;
	else
		return PS5000A_50V;
}

scopeTimebase_t getTimebaseFromText(string str)
{
	if (!str.compare("TIMEBASE_8NS"))
		return TIMEBASE_8NS;
	else if (!str.compare("TIMEBASE_16NS"))
		return TIMEBASE_16NS;
	else if (!str.compare("TIMEBASE_24NS"))
		return TIMEBASE_24NS;
	else if (!str.compare("TIMEBASE_40NS"))
		return TIMEBASE_40NS;
	else if (!str.compare("TIMEBASE_48NS"))
		return TIMEBASE_48NS;
	else if (!str.compare("TIMEBASE_56NS"))
		return TIMEBASE_56NS;
	else if (!str.compare("TIMEBASE_64NS"))
		return TIMEBASE_64NS;
	else if (!str.compare("TIMEBASE_72NS"))
		return TIMEBASE_72NS;
	else if (!str.compare("TIMEBASE_80NS"))
		return TIMEBASE_80NS;
	else if (!str.compare("TIMEBASE_160NS"))
		return TIMEBASE_160NS;
	else if (!str.compare("TIMEBASE_1000NS"))
		return TIMEBASE_1000NS;
	else
		return TIMEBASE_10US;
}