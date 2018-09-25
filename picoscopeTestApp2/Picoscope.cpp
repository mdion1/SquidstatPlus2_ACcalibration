#include "stdafx.h"
#include "Picoscope.h"

Picoscope::Picoscope()
{
}

Picoscope::~Picoscope()
{
}

bool Picoscope::open(int numChannels)
{
	PICO_STATUS status = PICO_OK;
	_numChannels = numChannels;
	PS5000A_DEVICE_RESOLUTION resolution = PS5000A_DR_15BIT;
	if (numChannels > 2)
		resolution = PS5000A_DR_14BIT;
	status = ps5000aOpenUnit(&_InstrHandle, NULL, resolution);

	if (status != PICO_OK && _InstrHandle <= 0)
	{
		std::cout << "Picoscope not found (or other error)...\n";
		return false;
	}
	return true;
}

void Picoscope::configureChannel(int channelNum, PS5000A_RANGE range)
{
	switch (channelNum)
	{
	case 1:
		ps5000aSetChannel(_InstrHandle, PS5000A_CHANNEL_A, 1, PS5000A_DC, range, 0);
		break;
	case 2:
		ps5000aSetChannel(_InstrHandle, PS5000A_CHANNEL_B, 1, PS5000A_DC, range, 0);
		break;
	case 3:
		ps5000aSetChannel(_InstrHandle, PS5000A_CHANNEL_C, 1, PS5000A_DC, range, 0);
		break;
	case 4:
		ps5000aSetChannel(_InstrHandle, PS5000A_CHANNEL_D, 1, PS5000A_DC, range, 0);
		break;
	default:
		break;
	}
}

void Picoscope::turnOnSignalGen(double frequency, double amplitude)
{
	ps5000aSetSigGenBuiltIn(_InstrHandle, 0, amplitude * 1e6, PS5000A_SINE, frequency, frequency, 0, 1, PS5000A_UP, PS5000A_ES_OFF, PS5000A_SHOT_SWEEP_TRIGGER_CONTINUOUS_RUN, PS5000A_SHOT_SWEEP_TRIGGER_CONTINUOUS_RUN, PS5000A_SIGGEN_RISING, PS5000A_SIGGEN_NONE, 0);
}

void Picoscope::close()
{
	ps5000aCloseUnit(_InstrHandle);
}

void Picoscope::getData_2ch(scopeTimebase_t timebase, uint32_t * pNumPoints, vector<int16_t> &chA_data, vector<int16_t> &chB_data)
{
	uint32_t numPoints = *pNumPoints;
	ps5000aRunBlock(_InstrHandle, 0, numPoints, (uint32_t)timebase, NULL, 0, NULL, NULL);
	int16_t ready = 0;
	while (!ready)
	{
		!ps5000aIsReady(_InstrHandle, &ready);
	}
	ps5000aStop(_InstrHandle);

	// 5) Get data
	int16_t * channelA_data = new int16_t[numPoints];
	int16_t * channelB_data = new int16_t[numPoints];
	int16_t overflow;
	ps5000aSetDataBuffer(_InstrHandle, PS5000A_CHANNEL_A, channelA_data, numPoints, 0, PS5000A_RATIO_MODE_NONE);
	ps5000aSetDataBuffer(_InstrHandle, PS5000A_CHANNEL_B, channelB_data, numPoints, 0, PS5000A_RATIO_MODE_NONE);
	ps5000aGetValues(_InstrHandle, 0, pNumPoints, 1, PS5000A_RATIO_MODE_NONE, 0, &overflow);
	numPoints = *pNumPoints;
	chA_data.assign(channelA_data, channelA_data + numPoints);
	chB_data.assign(channelB_data, channelB_data + numPoints);

	delete[] channelA_data;
	delete[] channelB_data;
}

void Picoscope::getData_3ch(scopeTimebase_t timebase, uint32_t * pNumPoints, vector<int16_t> &chA_data, vector<int16_t> &chB_data, vector<int16_t> &chC_data)
{
	uint32_t numPoints = *pNumPoints;
	ps5000aRunBlock(_InstrHandle, 0, numPoints, (uint32_t)timebase, NULL, 0, NULL, NULL);
	int16_t ready = 0;
	while (!ready)
	{
		!ps5000aIsReady(_InstrHandle, &ready);
	}
	ps5000aStop(_InstrHandle);

	// 5) Get data
	int16_t * channelA_data = new int16_t[numPoints];
	int16_t * channelB_data = new int16_t[numPoints];
	int16_t * channelC_data = new int16_t[numPoints];
	int16_t overflow;
	ps5000aSetDataBuffer(_InstrHandle, PS5000A_CHANNEL_A, channelA_data, numPoints, 0, PS5000A_RATIO_MODE_NONE);
	ps5000aSetDataBuffer(_InstrHandle, PS5000A_CHANNEL_B, channelB_data, numPoints, 0, PS5000A_RATIO_MODE_NONE);
	ps5000aSetDataBuffer(_InstrHandle, PS5000A_CHANNEL_C, channelC_data, numPoints, 0, PS5000A_RATIO_MODE_NONE);
	ps5000aGetValues(_InstrHandle, 0, pNumPoints, 1, PS5000A_RATIO_MODE_NONE, 0, &overflow);
	numPoints = *pNumPoints;
	chA_data.assign(channelA_data, channelA_data + numPoints);
	chB_data.assign(channelB_data, channelB_data + numPoints);
	chC_data.assign(channelC_data, channelC_data + numPoints);

	delete[] channelA_data;
	delete[] channelB_data;
	delete[] channelC_data;
}

double Picoscope::getTimebase(scopeTimebase_t timebase)
{
	return (((double)timebase - 2) / 125000000.0);
}