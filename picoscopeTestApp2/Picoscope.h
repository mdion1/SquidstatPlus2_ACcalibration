#pragma once
#include <ps5000aApi.h>
#include <PicoStatus.h>
#include <cstdint>
#include <iostream>
#include <vector>
using namespace std;

typedef enum {
	TIMEBASE_8NS = 3, TIMEBASE_16NS, TIMEBASE_24NS, TIMEBASE_32NS, TIMEBASE_40NS, TIMEBASE_48NS, TIMEBASE_56NS, TIMEBASE_64NS, TIMEBASE_72NS, TIMEBASE_80NS,
	TIMEBASE_160NS = 22, TIMEBASE_1000NS = 127, TIMEBASE_10US = 1252, TIMEBASE_100US = 12502
}scopeTimebase_t;

typedef struct
{
	double frequency;
	double amplitude;
	double DCbias;
	PS5000A_RANGE range_chA;
	PS5000A_RANGE range_chB;
	scopeTimebase_t timebase;
	uint32_t numPoints;
	int num_repeats;
}PicoscopeSamplingParams_t;

class Picoscope
{
public:
	bool open(int numChannels = 2);
	void close();
	void configureChannel(int channelNum, PS5000A_RANGE range);
	void turnOnSignalGen(double frequency, double amplitude, double dcbias);
	void getData_2ch(scopeTimebase_t timebase, uint32_t * numPoints, vector<int16_t> &chA_data, vector<int16_t> &chB_data);
	void getData_3ch(scopeTimebase_t timebase, uint32_t * numPoints, vector<int16_t> &chA_data, vector<int16_t> &chB_data, vector<int16_t> &chC_data);
	void getData_4ch(scopeTimebase_t timebase, uint32_t * pNumPoints, vector<int16_t> &chA_data, vector<int16_t> &chB_data, vector<int16_t> &chC_data, vector<int16_t> &chD_data);
	double getBinaryToVoltsScalingRatio(int channelNum) const;
	int16_t getNumChannels() { return _numChannels; };

	/* static methods */
	static double getTimebase(scopeTimebase_t timebase);
	static PS5000A_RANGE getRangeFromText(string str);

private:
	int16_t _InstrHandle;
	int16_t _numChannels;
	PS5000A_RANGE channelRanges[4];
};

