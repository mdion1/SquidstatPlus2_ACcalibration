#include "stdafx.h"
#include "Picoscope.h"

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
	if (channelNum <= 4 && channelNum > 0)
		channelRanges[channelNum - 1] = range;	//'channelNum' is NOT zero-indexed
}

void Picoscope::turnOnSignalGen(double frequency, double amplitude, double dcbias)
{
	ps5000aSetSigGenBuiltIn(_InstrHandle, (int32_t)(dcbias * 1e6), (uint32_t)(amplitude * 1e6 * 2), PS5000A_SINE, frequency, frequency, 0, 1, PS5000A_UP, PS5000A_ES_OFF, PS5000A_SHOT_SWEEP_TRIGGER_CONTINUOUS_RUN, PS5000A_SHOT_SWEEP_TRIGGER_CONTINUOUS_RUN, PS5000A_SIGGEN_RISING, PS5000A_SIGGEN_NONE, 0);
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

double Picoscope::getBinaryToVoltsScalingRatio(int channelNum) const
{
	if (channelNum > 4 || channelNum < 1)	//'channelNum' is NOT zero-indexed
		return 0;

	short minVal = 0, maxVal = 0;
	PICO_STATUS status;
	status = ps5000aMinimumValue(_InstrHandle, &minVal);
	if (status != PICO_OK)
		return 0;
	status = ps5000aMaximumValue(_InstrHandle, &maxVal);
	if (status != PICO_OK)
		return 0;

	double maxVoltage = 0;
	switch (channelRanges[channelNum - 1])
	{
		case PS5000A_10MV:
			maxVoltage = 0.01;
			break;
		case PS5000A_20MV:
			maxVoltage = 0.02;
			break;
		case PS5000A_50MV:
			maxVoltage = 0.05;
			break;
		case PS5000A_100MV:
			maxVoltage = 0.1;
			break;
		case PS5000A_200MV:
			maxVoltage = 0.2;
			break;
		case PS5000A_500MV:
			maxVoltage = 0.5;
			break;
		case PS5000A_1V:
			maxVoltage = 1;
			break;
		case PS5000A_2V:
			maxVoltage = 2;
			break;
		case PS5000A_5V:
			maxVoltage = 5;
			break;
		case PS5000A_10V:
			maxVoltage = 10;
			break;
		case PS5000A_20V:
			maxVoltage = 20;
			break;
		case PS5000A_50V:
			maxVoltage = 50;
			break;
		default:
			break;
	}

	return maxVoltage * 2 / (maxVal - minVal);
}

double Picoscope::getTimebase(scopeTimebase_t timebase)
{
	return (((double)timebase - 2) / 125000000.0);
}

PS5000A_RANGE Picoscope::getRangeFromText(string str)
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