#include "stdafx.h"
#include "cal_experiment.h"

void cal_experiment::runExperiment(const inputParams_t& inputParams)
{
    int numChannels = inputParams.numChannels;

    for (int i = 0; i < inputParams.freqList.size(); i++)
    {
        getFrequencies(inputParams.freqList[i]);
    }
    pscope.open(numChannels);

    // set up channels
    scaledProbes.resize(numChannels);
    for (int i = 0; i < numChannels; i++)
    {
        scaledProbes[i] = validateProbe(inputParams.probe[i]);
        pscope.configureChannel(i, scaledProbes[i].range, scaledProbes[i].coupling);
    }

    rawData.resize(numChannels);
	for (samplingParams_t params : parameterList)
	{
		// turn on signal generator
		pscope.turnOnSignalGen(params.frequency, inputParams.ACamp, inputParams.DCbias);
		
		// get data, crunch the numbers
        const int NUM_REPEATS = 1;
        vector<vector<ComplexNum_polar>> averages;
        averages.resize(numChannels);
        for (int i = 0; i < NUM_REPEATS; i++)
        {
            vector<int16_t>* data = new vector<int16_t>[numChannels];
            pscope.sample(params.timebase, &params.numPoints, data, numChannels);
            for (int ch = 0; ch < numChannels; ch++)
            {
                ComplexNum_polar num = NumberCruncher::fourier(data[i], params.frequency, Picoscope::getTimebase(params.timebase));
                //todo: scale
                averages[ch].push_back(num);
            }
        }

        // normalize all channels to channel A
        for (int ch = 0; ch < numChannels; ch++)
        {
            for (int i = 0; i < averages[0].size(); i++)
            {
                averages[ch][i].addPhase(-averages[ch][i].Phase);
                averages[ch][i].scalarMultiply(getScale(scaledProbes[i].range));
            }
            rawData[ch].push_back(NumberCruncher::avgComplex(averages[ch]));
            cout << rawData[ch].back().frequency << '\t' << rawData[ch].back().Mag << '\t' << rawData[ch].back().Phase << '\n';
        }
        cout << '\n';
	}
	pscope.close();
}

void cal_experiment::getFrequencies(double freq)
{
    samplingParams_t samplingParams;
	samplingParams.frequency = freq;
	samplingParams.timebase = TIMEBASE_16NS;
	while (true)
	{
		int maxNumPoints = 1e5;
		double dt = Picoscope::getTimebase(samplingParams.timebase);
		int numCycles = 2;
		double numPoints = (numCycles / freq) / dt;
		if (numPoints > maxNumPoints)
			samplingParams.timebase = (scopeTimebase_t)((int)samplingParams.timebase + 1);
		else
		{
			samplingParams.numPoints = maxNumPoints;
			break;
		}
	}
    parameterList.push_back(samplingParams);
}

probeParams_t cal_experiment::validateProbe(const probeParams_t& probe)
{
    if (PROBE_DIV_1X == probe.div) {
        return probe;
    }
    else {
        probeParams_t scaledProbe = probe;
        scaledProbe.div = PROBE_DIV_1X;
        switch (probe.range)
        {
        case PS5000A_10MV:
        case PS5000A_20MV:
        case PS5000A_50MV:
            //error, won't work
            scaledProbe.range = PS5000A_10MV;
        default:
            scaledProbe.range = (PS5000A_RANGE)((int)probe.range - 3); //scale down by 10x
        }
        return scaledProbe;
    }
}

double cal_experiment::getScale(PS5000A_RANGE range)
{
//15 bit resolution
    static const double scale = (1 << (15 - 1)) - 1;
    switch (range)
    {
    case PS5000A_10MV:
        return scale * 0.01;
    case PS5000A_20MV:
        return scale * 0.02;
    case PS5000A_50MV:
        return scale * 0.05;
    case PS5000A_100MV:
        return scale * 0.1;
    case PS5000A_200MV:
        return scale * 0.2;
    case PS5000A_500MV:
        return scale * 0.5;
    case PS5000A_1V:
        return scale * 1;
    case PS5000A_2V:
        return scale * 2;
    case PS5000A_5V:
        return scale * 5;
    case PS5000A_10V:
        return scale * 10;
    case PS5000A_20V:
        return scale * 20;
    case PS5000A_50V:
        return scale * 50;
    default:
        return NAN;
    }
}