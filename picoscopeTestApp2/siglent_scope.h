#include <cstdint>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <stdio.h>
#include "Picoscope.h"
#include "visa.h"
#pragma comment(lib, "visa32.lib")

using namespace std;

class siglent_scope
{
public:
	void open();
	void close();
	void configureChannel(int channelNum, PS5000A_RANGE range);
	void turnOnSignalGen(double frequency, double amplitude);
	void getData_2ch(scopeTimebase_t timebase, uint32_t * numPoints, vector<int16_t> &chA_data, vector<int16_t> &chB_data);
	static double getTimebase(scopeTimebase_t timebase);

private:
	void writeToScope(string msg);
	void writeToScope(const char * msg);
	void writeToScope(ViConstBuf msg, int len);
	string readFromScope(uint32_t maxLength = 256);

	ViSession defaultRM;
	ViSession instr;
};