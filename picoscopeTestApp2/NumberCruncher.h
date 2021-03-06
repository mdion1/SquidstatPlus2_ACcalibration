#pragma once

#define _USE_MATH_DEFINES
#include <cmath>
#include <cstdint>
#include <iostream>
#include <vector>

using namespace std;

typedef struct
{
	double frequency;
	double mag;
	double phase;
}ComplexNum_polar;

class NumberCruncher
{
public:
	NumberCruncher();
	~NumberCruncher();
	static ComplexNum_polar AnalyzeSignalSingle(const vector<int16_t> &sig1, double frequency, double timestep);
	static ComplexNum_polar CompareSignals(const vector<int16_t> &sig1, const vector<int16_t> &sig2, double frequency, double timestep, double A_B_scale = 1);
	static ComplexNum_polar CompareSignalsDiff(const vector<int16_t> &sig1, const vector<int16_t> &sig2, const vector<int16_t> &sig3, double frequency, double timestep, double A_B_scale = 1);
	static ComplexNum_polar CompareSignalsDiff2(const vector<int16_t> &sig1, const vector<int16_t> &sig2, const vector<int16_t> &sig3, const vector<int16_t> &sig4, double frequency, double timestep, double A_B_scale = 1);
	static ComplexNum_polar getAvg(const vector<ComplexNum_polar> &data);
	static void NormalizeMag(vector<ComplexNum_polar> &x);

private:
	static ComplexNum_polar SingleFrequencyFourier(const vector<int16_t> &data, double period);
};

