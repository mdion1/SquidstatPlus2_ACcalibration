#include "stdafx.h"
#include "NumberCruncher.h"

NumberCruncher::NumberCruncher()
{
}

NumberCruncher::~NumberCruncher()
{
}

ComplexNum_polar NumberCruncher::CompareSignals(const vector<int16_t> &sig1, const vector<int16_t> &sig2, double frequency, double timestep)
{
	double period = (1 / frequency) / timestep;
	ComplexNum_polar sig1_complex = SingleFrequencyFourier(sig1, period);
	ComplexNum_polar sig2_complex = SingleFrequencyFourier(sig2, period);
	ComplexNum_polar ret;
	ret.mag = sig2_complex.mag / sig1_complex.mag;
	ret.phase = fmod(sig2_complex.phase - sig1_complex.phase, 360);
	if (ret.phase > 180)
		ret.phase -= 360;
	return ret;
}

ComplexNum_polar NumberCruncher::SingleFrequencyFourier(const vector<int16_t> &data, double period)
{
	//determine number of samples to ignore
	double numWholeCycles = floor(data.size() / period);
	int len = (int)floor(numWholeCycles * period);
	double sumSine = 0, sumCosine = 0;
	for (int i = 0; i < len; i++)
	{
		double arg = i * (2 * M_PI / period);
		sumSine += data[i] * sin(arg);
		sumCosine += data[i] * cos(arg);
	}
	ComplexNum_polar x;
	x.mag = sqrt(sumSine * sumSine + sumCosine * sumCosine);
	x.phase = atan2(sumSine, sumCosine) * 180 / M_PI;
	return x;
}