#pragma once
#include <ps5000aApi.h>
#include <PicoStatus.h>
#include <cstdint>
#include <iostream>
#include <vector>
using namespace std;

typedef enum {
	TIMEBASE_8NS = 3, TIMEBASE_16NS, TIMEBASE_24NS, TIMEBASE_32NS, TIMEBASE_40NS, TIMEBASE_48NS, TIMEBASE_56NS, TIMEBASE_64NS, TIMEBASE_72NS, TIMEBASE_80NS,
	TIMEBASE_160NS = 22, TIMEBASE_1000NS = 127, TIMEBASE_10US = 1252
}scopeTimebase_t;

class Picoscope
{
public:
	Picoscope();
	~Picoscope();
	bool open(int numChannels = 2);
	void close();
	void configureChannel(int channelNum, PS5000A_RANGE range);
	void turnOnSignalGen(double frequency, double amplitude);
	void getData_2ch(scopeTimebase_t timebase, uint32_t * numPoints, vector<int16_t> &chA_data, vector<int16_t> &chB_data);
	static double getTimebase(scopeTimebase_t timebase);

private:
	int16_t _InstrHandle;
	int16_t _numChannels;
};

