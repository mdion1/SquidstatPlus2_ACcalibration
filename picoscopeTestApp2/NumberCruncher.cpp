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
	ret.frequency = frequency;
	ret.mag = sig2_complex.mag / sig1_complex.mag;
	ret.phase = fmod(sig2_complex.phase - sig1_complex.phase, 360);
	ret.inputAmplitude = sig1_complex.mag;
	ret.biasIn = sig1_complex.biasIn;
	ret.biasOut = sig1_complex.biasOut;
	if (ret.phase > 180)
		ret.phase -= 360;
	if (ret.phase <= -180)
		ret.phase += 360;
	return ret;
}

ComplexNum_polar NumberCruncher::CompareSignals(const vector<int16_t> &sig1, const vector<int16_t> &sig2, const Picoscope * picoscope, PicoscopeSamplingParams_t params)
{
	double timestep = Picoscope::getTimebase(params.timebase);
	double period = (1 / params.frequency) / timestep;
	ComplexNum_polar sig1_complex = SingleFrequencyFourier(sig1, period);
	sig1_complex.mag *= picoscope->getBinaryToVoltsScalingRatio(1);
	sig1_complex.biasIn *= picoscope->getBinaryToVoltsScalingRatio(1);
	ComplexNum_polar sig2_complex = SingleFrequencyFourier(sig2, period);
	sig2_complex.mag *= picoscope->getBinaryToVoltsScalingRatio(2);
	sig2_complex.biasIn *= picoscope->getBinaryToVoltsScalingRatio(2);
	ComplexNum_polar ret;
	ret.frequency = params.frequency;
	ret.mag = sig2_complex.mag / sig1_complex.mag;
	ret.phase = fmod(sig2_complex.phase - sig1_complex.phase, 360);
	ret.inputAmplitude = sig1_complex.mag;
	ret.biasIn = sig1_complex.biasIn;
	ret.biasOut = sig2_complex.biasIn;
	if (ret.phase > 180)
		ret.phase -= 360;
	if (ret.phase <= -180)
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
	double sumSine = 0, sumCosine = 0, sumDC = 0;
	for (int i = 0; i < len; i++)
	{
		double arg = i * (2 * M_PI / period);
		sumSine += data[i] * sin(arg);
		sumCosine += data[i] * cos(arg);
		sumDC += data[i];
	}
	ComplexNum_polar x;
	x.mag = sqrt(sumSine * sumSine + sumCosine * sumCosine) * 2 / len;
	x.phase = atan2(sumSine, sumCosine) * 180 / M_PI;
	x.biasIn = sumDC / len;
	return x;
}

ComplexNum_polar NumberCruncher::getAvg(const vector<ComplexNum_polar> &data)
{
	ComplexNum_polar ret; //todo: constructor
	memset(&ret, 0, sizeof(ComplexNum_polar));
	if (data.size() == 0)
		return ret;
	double x = 0, y = 0;
	ret.inputAmplitude = 0;
	for (int i = 0; i < data.size(); i++)
	{
		x += data[i].mag * cos(data[i].phase * M_PI / 180);
		y += data[i].mag * sin(data[i].phase * M_PI / 180);
		ret.inputAmplitude += data[i].inputAmplitude;
		ret.biasIn += data[i].biasIn;
		ret.biasOut += data[i].biasOut;
	}
	x /= data.size();
	y /= data.size();
	ret.inputAmplitude /= data.size();
	ret.biasIn /= data.size();
	ret.biasOut /= data.size();

	ret.frequency = data[0].frequency;
	ret.mag = sqrt(x * x + y * y);
	ret.phase = atan2(y, x) * 180 / M_PI;
	
	return ret;
}

void NumberCruncher::NormalizeMag(vector<ComplexNum_polar> &x)
{
	ComplexNum_polar baseline = x.back();
	for (int i = 0; i < x.size(); i++)
	{
		x[i].mag /= baseline.mag;
		x[i].phase -= baseline.phase;
	}
}