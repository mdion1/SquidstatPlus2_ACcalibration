#pragma once

#define _USE_MATH_DEFINES
#include <cmath>
#include <cstdint>
#include <iostream>
#include <vector>

using namespace std;

class ComplexNum_polar
{
public:
    void scalarMultiply(double scalar);
    void addPhase(double phase);
    void setPolar(double mag, double phase);
    void setRect(double re, double im);
    double frequency;
    double Mag;
    double Phase;
    double Re;
    double Im;
};

class NumberCruncher
{
public:
    static ComplexNum_polar fourier(const vector<int16_t>& sig, double frequency, double timestep);
	static ComplexNum_polar avgComplex(const vector<ComplexNum_polar> &data);
};

