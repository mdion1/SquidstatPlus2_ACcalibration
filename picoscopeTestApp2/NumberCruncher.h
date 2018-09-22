#pragma once

#define _USE_MATH_DEFINES
#include <cmath>
#include <cstdint>
#include <iostream>
#include <vector>

using namespace std;

typedef struct
{
	double mag;
	double phase;
}ComplexNum_polar;

class NumberCruncher
{
public:
	NumberCruncher();
	~NumberCruncher();
	static ComplexNum_polar CompareSignals(const vector<int16_t> &sig1, const vector<int16_t> &sig2, double frequency, double timestep);

private:
	static ComplexNum_polar SingleFrequencyFourier(const vector<int16_t> &data, double period);
};

