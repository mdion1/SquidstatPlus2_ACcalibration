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
	if (ret.phase < -180)
		ret.phase += 360;
	return ret;
}

ComplexNum_polar NumberCruncher::CompareSignalsDiff(const vector<int16_t> &sig1, const vector<int16_t> &sig2, const vector<int16_t> &sig3, double frequency, double timestep)
{
	if (sig1.size() != sig2.size())
		return ComplexNum_polar();

	vector<int16_t> diff;
	diff.reserve(sig1.size());
	for (int i = 0; i < sig1.size(); i++)
	{
		diff.push_back(sig1[i] - sig2[i]);
	}

	return CompareSignals(diff, sig3, frequency, timestep);
}

ComplexNum_polar NumberCruncher::CompareSignalsDiff2(const vector<int16_t> &sig1, const vector<int16_t> &sig2, const vector<int16_t> &sig3, const vector<int16_t> &sig4, double frequency, double timestep)
{
	if (sig1.size() != sig2.size())
		return ComplexNum_polar();

	vector<int16_t> diff1, diff2;
	diff1.reserve(sig1.size());
	diff2.reserve(sig3.size());
	for (int i = 0; i < sig1.size(); i++)
	{
		diff1.push_back(sig1[i] - sig2[i]);
		diff2.push_back(sig3[i] - sig4[i]);
	}

	return CompareSignals(diff1, diff2, frequency, timestep);
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

void NumberCruncher::NormalizeMag(vector<ComplexNum_polar> &x)
{
	double baseline = x.back().mag;
	for (int i = 0; i < x.size(); i++)
		x[i].mag /= baseline;
}