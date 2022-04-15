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

void Picoscope::configureChannel(int channelNum, PS5000A_RANGE range, enPS5000ACoupling coupling)
{
	switch (channelNum)
	{
	case 0:
		ps5000aSetChannel(_InstrHandle, PS5000A_CHANNEL_A, 1, coupling, range, 0);
		break;
	case 1:
		ps5000aSetChannel(_InstrHandle, PS5000A_CHANNEL_B, 1, coupling, range, 0);
		break;
	case 2:
		ps5000aSetChannel(_InstrHandle, PS5000A_CHANNEL_C, 1, coupling, range, 0);
		break;
	case 3:
		ps5000aSetChannel(_InstrHandle, PS5000A_CHANNEL_D, 1, coupling, range, 0);
		break;
	default:
		break;
	}
}

void Picoscope::turnOnSignalGen(double frequency, double amplitude, double dcbias)
{
    //convert amplitude from volts c-pk to uV pk-pk
	ps5000aSetSigGenBuiltIn(_InstrHandle, (int32_t)(dcbias * 1e6), (uint32_t)(2 * amplitude * 1e6), PS5000A_SINE, frequency, frequency, 0, 1, PS5000A_UP, PS5000A_ES_OFF, PS5000A_SHOT_SWEEP_TRIGGER_CONTINUOUS_RUN, PS5000A_SHOT_SWEEP_TRIGGER_CONTINUOUS_RUN, PS5000A_SIGGEN_RISING, PS5000A_SIGGEN_NONE, 0);
}

void Picoscope::close()
{
	ps5000aCloseUnit(_InstrHandle);
}

void Picoscope::sample(scopeTimebase_t timebase, uint32_t* numPoints_inOut, vector<int16_t>* data_out, int num_channels)
{
    ps5000aRunBlock(_InstrHandle, 0, (*numPoints_inOut), (uint32_t)timebase, NULL, 0, NULL, NULL);
    int16_t ready = 0;
    while (!ready)
    {
        !ps5000aIsReady(_InstrHandle, &ready);
    }
    ps5000aStop(_InstrHandle);

	int16_t** data;
	data = new int16_t*[num_channels];
	for (int i = 0; i < num_channels; i++)
	{
		data[i] = new int16_t[(*numPoints_inOut)];
		ps5000aSetDataBuffer(_InstrHandle, (enPS5000AChannel)i, data[i], (*numPoints_inOut), 0, PS5000A_RATIO_MODE_NONE);
	}

	int16_t overflow;
	ps5000aGetValues(_InstrHandle, 0, numPoints_inOut, 1, PS5000A_RATIO_MODE_NONE, 0, &overflow);

	for (int i = 0; i < num_channels; i++)
	{
		data_out[i].assign(data[i], data[i] + (*numPoints_inOut));
		delete[] data[i];
	}
    delete[] data;
}

double Picoscope::getTimebase(scopeTimebase_t timebase)
{
	return (((double)timebase - 2) / 125000000.0);
}