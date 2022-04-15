#include "stdafx.h"
#include <cstdio>
#include <iostream>
#include "cal_experiment.h"
#include "argParse.h"

using namespace std;

int main(int argc, char *argv[])
{
	if (argc < 2)
		return 0;

    inputParams_t params = argParse(argv[1]).getParams();

	vector<ComplexNum_polar> data;
	cal_experiment exp;
	exp.runExperiment(params);

	/* write data */
	std::ofstream fout;
	fout.open(params.outputFileName.c_str(), std::ofstream::out);
	fout.precision(8);
	cout.precision(8);

    fout << "Frequency" << ',' << "Mag" << ',' << "Phase" << '\n';
    for (int i = 0; i < data.size(); i++)
    {
        fout << exp.getRawData()[0][i].frequency;
        for (int ch = 0; ch < params.numChannels; ch++)
        {
            fout << ',' << exp.getRawData()[ch][i].Mag << ',' << exp.getRawData()[ch][i].Phase;
        }
        fout << '\n';
    }
	
exit:
	{
		system("pause");
		return 0;
	}
}