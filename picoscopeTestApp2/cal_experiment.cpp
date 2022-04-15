#include "stdafx.h"
#include "cal_experiment.h"

void cal_experiment::runExperiment(inputParams_t& inputParams)
{
    int numChannels = inputParams.numChannels;

    for (int i = 0; i < inputParams.freqList.size(); i++)
    {
        getFrequencies(inputParams.freqList[i]);
    }
    pscope.open(numChannels);

    // set up channels
    for (int ch = 0; ch < numChannels; ch++)
    {
        validateProbe(inputParams.probe[ch]);
        pscope.configureChannel(ch, adjustProbeScaleWithDiv(inputParams.probe[ch]), inputParams.probe[ch].coupling);
    }

    rawData.resize(numChannels);
	for (samplingParams_t params : parameterList)
	{
		// turn on signal generator
		pscope.turnOnSignalGen(params.frequency, inputParams.ACamp, inputParams.DCbias);
		
		// get data, crunch the numbers
        const int NUM_REPEATS = 5;
        vector<vector<ComplexNum_polar>> averages;
        averages.resize(numChannels);
        for (int i = 0; i < NUM_REPEATS; i++)
        {
            vector<int16_t>* data = new vector<int16_t>[numChannels];
            pscope.sample(params.timebase, &params.numPoints, data, numChannels);
            for (int ch = 0; ch < numChannels; ch++)
            {
                ComplexNum_polar num = NumberCruncher::fourier(data[ch], params.frequency, Picoscope::getTimebase(params.timebase));
                //todo: scale
                averages[ch].push_back(num);
            }
        }

        // normalize all channels to channel A
        for (int ch = numChannels - 1; ch >= 0; ch--) // loop backwards, since chA phase needs to be retained
        {
            for (int i = 0; i < averages[0].size(); i++)   
            {
                averages[ch][i].addPhase(-averages[0][i].Phase);   //channel A is the "reference" phase
                averages[ch][i].scalarMultiply(getScale(inputParams.probe[ch]));
            }
            rawData[ch].push_back(NumberCruncher::avgComplex(averages[ch]));
            
        }

        //print output
        cout << rawData[0].back().frequency << '\t';
        for (int ch = 0; ch < numChannels; ch++)
        {
            cout << rawData[ch].back().Mag << '\t' << rawData[ch].back().Phase << '\t';
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

void cal_experiment::validateProbe(probeParams_t& probe)
{
    if (PROBE_DIV_1X == probe.div) {
        return;
    }
    else {
        switch (probe.range)
        {
        case PS5000A_10MV:
        case PS5000A_20MV:
        case PS5000A_50MV:
            // 10x probes cannot go below 100mV range
            probe.range = PS5000A_100MV;
            break;
        default:
            break;
        }
    }
}

PS5000A_RANGE cal_experiment::adjustProbeScaleWithDiv(const probeParams_t& probe)
{
    if (PROBE_DIV_1X == probe.div) {
        return probe.range;
    }
    else {
        return (PS5000A_RANGE)((int)probe.range - 3); //scale down by 10x
    }
}

double cal_experiment::getScale(const probeParams_t& probe)
{
//15 bit resolution
    double scale = (1.0 / INT16_MAX);// *((PROBE_DIV_10X == probe.div) ? 10 : 1);
    switch (probe.range)
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