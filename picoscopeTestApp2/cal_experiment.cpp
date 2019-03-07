#include "stdafx.h"
#include "cal_experiment.h"


cal_experiment::cal_experiment()
{
}

cal_experiment::~cal_experiment()
{
}

int getNumAvgs(double freq)
{
	//#define QUICK_RUN
#ifdef QUICK_RUN
	return 5;
#else
	if (freq < 10)
		return 5;
	else if (freq < 100)
		return 10;
	else
		return 20;
#endif
}

void cal_experiment::runExperiment()
{
	vector<ComplexNum_polar> v_chA, v_chB, v_chAB;
	double A_B_scaler = Picoscope::getScale(parameterList[0].range_chB) / Picoscope::getScale(parameterList[0].range_chA);

	/* Get magnitude by running channels A and B separately */
	pscope.configureChannel(1, parameterList[0].range_chA);
	pscope.disableChannel(PS5000A_CHANNEL_B);
	for (experimentParams_t params : parameterList)
	{
		// turn on signal generator
		pscope.turnOnSignalGen(params.frequency, params.amplitude);

		int numAvgs = getNumAvgs(params.frequency);
		vector<ComplexNum_polar> avg;
		for (int i = 0; i < numAvgs; i++)
		{
			vector<int16_t> A;
			pscope.getData_1ch(params.timebase, &params.numPoints, A, PS5000A_CHANNEL_A);
			avg.push_back(NumberCruncher::AnalyzeSignalSingle(A, params.frequency, Picoscope::getTimebase(params.timebase)));
		}
		v_chA.push_back(NumberCruncher::getAvg(avg));
		cout << v_chA.back().frequency << '\t' << v_chA.back().mag << '\t' << v_chA.back().phase << '\n';
	}

	pscope.configureChannel(2, parameterList[0].range_chB);
	pscope.disableChannel(PS5000A_CHANNEL_A);
	for (experimentParams_t params : parameterList)
	{
		// turn on signal generator
		pscope.turnOnSignalGen(params.frequency, params.amplitude);

		int numAvgs = getNumAvgs(params.frequency);
		vector<ComplexNum_polar> avg;
		for (int i = 0; i < numAvgs; i++)
		{
			vector<int16_t> B;
			pscope.getData_1ch(params.timebase, &params.numPoints, B, PS5000A_CHANNEL_B);
			avg.push_back(NumberCruncher::AnalyzeSignalSingle(B, params.frequency, Picoscope::getTimebase(params.timebase)));
		}
		v_chB.push_back(NumberCruncher::getAvg(avg));
		cout << v_chB.back().frequency << '\t' << v_chB.back().mag << '\t' << v_chB.back().phase << '\n';
	}

	/* Get phase by running channels A and B together */
	pscope.configureChannel(1, parameterList[0].range_chA);
	for (experimentParams_t params : parameterList)
	{
		// turn on signal generator
		pscope.turnOnSignalGen(params.frequency, params.amplitude);

		int numAvgs = getNumAvgs(params.frequency);
		vector<ComplexNum_polar> avg;
		for (int i = 0; i < numAvgs; i++)
		{
			vector<int16_t> A, B;
			pscope.getData_2ch(params.timebase, &params.numPoints, A, B);
			avg.push_back(NumberCruncher::CompareSignals(A, B, params.frequency, Picoscope::getTimebase(params.timebase), A_B_scaler));
		}
		v_chAB.push_back(NumberCruncher::getAvg(avg));
		cout << v_chAB.back().frequency << '\t' << v_chAB.back().mag << '\t' << v_chAB.back().phase << '\n';
	}

	for (int i = 0; i < v_chA.size(); i++)
	{
		ComplexNum_polar x = v_chAB[i];
		x.mag = v_chB[i].mag / v_chA[i].mag / A_B_scaler;
		rawData.push_back(x);
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

void cal_experiment::readExperimentParamsFile(string comPortAddr, string filename)
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
	{
		str.insert(75, " ");
		str.insert(75, comPortAddr);
		system(str.c_str());
	}

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
#define CONTROL_RUN
#ifdef CONTROL_RUN
		int maxNumPoints = 2.5e5;
#else
		int maxNumPoints = 1e5;
#endif
		double dt = Picoscope::getTimebase(defaultParams.timebase);
		int numCycles = 2;
		double numPoints = (numCycles / freq) / dt;
		if (numPoints > maxNumPoints)
			defaultParams.timebase = (scopeTimebase_t)((int)defaultParams.timebase + 1);
		else
		{
			//numPoints = MIN(maxNumPoints, 0.1 / dt);
			//defaultParams.numPoints = numPoints;

			/********debugging*************/
			/*double period = 1 / freq / dt;
			double numWholeCycles = floor(maxNumPoints / period);
			int len = (int)floor(numWholeCycles * period);
			cout << freq << "\t" << len << "\n";*/


			defaultParams.numPoints = maxNumPoints;
			break;
		}
	}

	appendParameters(defaultParams);
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