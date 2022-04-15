#include "stdafx.h"
#include "NumberCruncher.h"
#include <complex>

double getRelativePeriod(double f_samp, double f_sig)
{
    double f_Nyq = f_samp * 0.5;
    int N = f_sig / f_Nyq;
    N = (N & 1) ? N : N - 1;    //if N is even, subtract one
    double f_alias = f_sig - (N + 1) * f_Nyq;
    return f_samp / f_alias;
}

double getRelativePeriod_pos(double f_samp, double f_sig)
{
    return abs(getRelativePeriod(f_samp, f_sig));
}

std::vector<std::complex<double>> MultiFrequencyFourier(const std::vector<int16_t>& data, double f_samp, double f_sig, int nHarmonics) {

    /* Find the integer number of cycles present */
    double fundamentalPeriod = getRelativePeriod_pos(f_samp, f_sig);
    int numCycles = data.size() / fundamentalPeriod;
    float len = abs(fundamentalPeriod * numCycles);
    nHarmonics = 2 + nHarmonics;        // add dc and fundamental frequency
    std::vector<float> tau = std::vector<float>(nHarmonics);

    std::vector<double> realSums(nHarmonics, 0);
    std::vector<double> imagSums(nHarmonics, 0);

    float offset = data[0];
    for (int h = 0; h < nHarmonics; ++h) {
        tau.at(h) = 2 * M_PI / getRelativePeriod(f_samp, f_sig * h);
        //realSums[h] += data[0] * 0.5f;  //with the trapezoidal integration, the first point is divided by 2
    }

    int i = 1;
    float val;
    for (; i < len; i++) {
        val = data[i] - offset;
        for (int h = 0; h < nHarmonics; ++h) {
            realSums[h] += val * cos(i * tau.at(h));
            imagSums[h] += val * sin(i * tau.at(h));
        }
    }
    --i;    // i is index of last point

    float fraction = len - i;
    for (int h = 0; h < nHarmonics; ++h) {
        realSums[h] += val * (0.5f * fraction - 0.5f) * cos(i * tau.at(h)); // subtract half weighting due to trapezoidal
        imagSums[h] += val * (0.5f * fraction - 0.5f) * sin(i * tau.at(h));
    }

    int x0 = i;
    int x1 = i + 1;
    float y1;
    if (data.size() > x1)
        y1 = data[x1] - offset;
    else
        y1 = 2 * data[x0] - data[x0 - 1] - offset;   //extrapolate

    for (int h = 0; h < nHarmonics; ++h) {
        realSums[h] += 0.5f * fraction * y1 * cos(h * 0);
        imagSums[h] += 0.5f * fraction * y1 * sin(h * 0);
    }

    for (int h = 0; h < nHarmonics; ++h) {
        realSums[h] *= 2. / len;
        imagSums[h] *= 2. / len;
    }
    realSums[0] += 2 * offset;

    std::vector<std::complex<double>> sums;
    for (int h = 0; h < nHarmonics; ++h)
        sums.emplace_back(realSums[h], imagSums[h]);

    return sums;
}

ComplexNum_polar NumberCruncher::fourier(const vector<int16_t>& data, double frequency, double timestep)
{
    auto fourierSum = MultiFrequencyFourier(data, 1.0 / timestep, frequency, 0);
    ComplexNum_polar x;
    x.setRect(fourierSum[1].real(), fourierSum[1].imag());
    x.frequency = frequency;
    return x;
#if 0
	double period = (1 / frequency) / timestep;
    double numWholeCycles = floor(data.size() / period);
    int len = (int)floor(numWholeCycles * period);
    double sumSine = 0, sumCosine = 0;
    for (int i = 0; i < len; i++)
    {
        double arg = i * (2 * M_PI / period);
        sumSine += data[i] * sin(arg);
        sumCosine += data[i] * cos(arg);
    }
    double mag = sqrt(sumSine * sumSine + sumCosine * sumCosine);
    double phase = atan2(sumSine, sumCosine) * 180 / M_PI;
    ComplexNum_polar x;
    x.setPolar(mag, phase);
    x.frequency = frequency;
    return x;
#endif
}

ComplexNum_polar NumberCruncher::avgComplex(const vector<ComplexNum_polar> &data)
{
    ComplexNum_polar ret;
    ret.setPolar(0, 0);

	if (data.size() == 0)
		return ret;

    double im = 0;
    double re = 0;
	for (int i = 0; i < data.size(); i++)
	{
		re += data[i].Re;
		im += data[i].Im;
	}
	ret.frequency = data[0].frequency;
    re /= data.size();
    im /= data.size();
    ret.setRect(re, im);
	return ret;
}

void ComplexNum_polar::scalarMultiply(double scalar)
{
    Mag *= scalar;
    Re *= scalar;
    Im *= scalar;
}

void ComplexNum_polar::addPhase(double phase)
{
    setPolar(Mag, Phase + phase);
}

void ComplexNum_polar::setPolar(double mag, double phase)
{
    Mag = mag;
    Phase = phase;
    Re = Mag * cos(phase * M_PI / 180);
    Im = Mag * sin(phase * M_PI / 180);
}

void ComplexNum_polar::setRect(double re, double im)
{
    Re = re;
    Im = im;
    Mag = sqrt(Re * Re + Im * Im);
    Phase = atan2(Im, Re) * 180.0 / M_PI;
}