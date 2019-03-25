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

#if PICOSCOPE_RESOLUTION == 15
	PS5000A_DEVICE_RESOLUTION resolution = PS5000A_DR_15BIT;
#elif PICOSCOPE_RESOLUTION == 12
	PS5000A_DEVICE_RESOLUTION resolution = PS5000A_DR_12BIT;
#elif PICOSCOPE_RESOLUTION == 8
	PS5000A_DEVICE_RESOLUTION resolution = PS5000A_DR_8BIT;
#endif



	status = ps5000aOpenUnit(&_InstrHandle, NULL, resolution);

	if (status != PICO_OK && _InstrHandle <= 0)
	{
		std::cout << "Picoscope not found (or other error)...\n";
		std::cout << "Picoscope status: " << status << "\n";
		std::cout << "Instr handle: " << _InstrHandle << "\n";
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

void Picoscope::disableChannel(PS5000A_CHANNEL channel)
{
	ps5000aSetChannel(_InstrHandle, channel, 0, PS5000A_DC, PS5000A_1V, 0);
}

void Picoscope::turnOnSignalGen(double frequency, double amplitude)
{
	ps5000aSetSigGenBuiltIn(_InstrHandle, 0, amplitude * 1e6, PS5000A_SINE, frequency, frequency, 0, 1, PS5000A_UP, PS5000A_ES_OFF, PS5000A_SHOT_SWEEP_TRIGGER_CONTINUOUS_RUN, PS5000A_SHOT_SWEEP_TRIGGER_CONTINUOUS_RUN, PS5000A_SIGGEN_RISING, PS5000A_SIGGEN_NONE, 0);
}

void Picoscope::close()
{
	ps5000aCloseUnit(_InstrHandle);
}

void Picoscope::getData_1ch(scopeTimebase_t timebase, uint32_t * pNumPoints, vector<int16_t> &chA_data, PS5000A_CHANNEL channel)
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
	int16_t overflow;
	ps5000aSetDataBuffer(_InstrHandle, channel, channelA_data, numPoints, 0, PS5000A_RATIO_MODE_NONE);
	ps5000aGetValues(_InstrHandle, 0, pNumPoints, 1, PS5000A_RATIO_MODE_NONE, 0, &overflow);
	numPoints = *pNumPoints;
	chA_data.assign(channelA_data, channelA_data + numPoints);

	delete[] channelA_data;
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

void Picoscope::getData_4ch(scopeTimebase_t timebase, uint32_t * pNumPoints, vector<int16_t> &chA_data, vector<int16_t> &chB_data, vector<int16_t> &chC_data, vector<int16_t> &chD_data)
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
	int16_t * channelD_data = new int16_t[numPoints];
	int16_t overflow;
	ps5000aSetDataBuffer(_InstrHandle, PS5000A_CHANNEL_A, channelA_data, numPoints, 0, PS5000A_RATIO_MODE_NONE);
	ps5000aSetDataBuffer(_InstrHandle, PS5000A_CHANNEL_B, channelB_data, numPoints, 0, PS5000A_RATIO_MODE_NONE);
	ps5000aSetDataBuffer(_InstrHandle, PS5000A_CHANNEL_C, channelC_data, numPoints, 0, PS5000A_RATIO_MODE_NONE);
	ps5000aSetDataBuffer(_InstrHandle, PS5000A_CHANNEL_D, channelD_data, numPoints, 0, PS5000A_RATIO_MODE_NONE);
	ps5000aGetValues(_InstrHandle, 0, pNumPoints, 1, PS5000A_RATIO_MODE_NONE, 0, &overflow);
	numPoints = *pNumPoints;
	chA_data.assign(channelA_data, channelA_data + numPoints);
	chB_data.assign(channelB_data, channelB_data + numPoints);
	chC_data.assign(channelC_data, channelC_data + numPoints);
	chD_data.assign(channelD_data, channelD_data + numPoints);

	delete[] channelA_data;
	delete[] channelB_data;
	delete[] channelC_data;
	delete[] channelD_data;
}

double Picoscope::getTimebase(scopeTimebase_t timebase)
{
#if PICOSCOPE_RESOLUTION == 15
	return (((double)timebase - 2) / 125.e6);
#elif PICOSCOPE_RESOLUTION == 12
	int32_t x;
	switch ((int)timebase)
	{
	case 1:
	case 2:
	case 3:
		x = 1 << (timebase - 1);
		return ((double)x) / 5.0e8;
		break;
	case 4:
	default:
		return (((double)timebase - 3) / 62.5e6);
		break;
	}
#elif PICOSCOPE_RESOLUTION == 8
	int32_t x;
	switch ((int)timebase)
	{
	case 0:
	case 1:
	case 2:
		x = 1 << timebase;
		return ((double)x) / 1.0e9;
		break;
	case 3:
	default:
		return (((double)timebase - 2) / 125.e6);
		break;
	}
#endif
}

double Picoscope::getScale(PS5000A_RANGE range)
{
	double ret = 1;
	switch (range)
	{
	case PS5000A_10MV:
		ret = 0.01;
		break;
	case PS5000A_20MV:
		ret = 0.02;
		break;
	case PS5000A_50MV:
		ret = 0.05;
		break;
	case PS5000A_100MV:
		ret = 0.1;
		break;
	case PS5000A_200MV:
		ret = 0.2;
		break;
	case PS5000A_500MV:
		ret = 0.51;
		break;
	case PS5000A_1V:
		ret = 1;
		break;
	case PS5000A_2V:
		ret = 2;
		break;
	case PS5000A_5V:
		ret = 5;
		break;
	case PS5000A_10V:
		ret = 10;
		break;
	case PS5000A_20V:
		ret = 20;
		break;
	case PS5000A_50V:
		ret = 50;
		break;
	default:
		break;
	}
	return ret;
}