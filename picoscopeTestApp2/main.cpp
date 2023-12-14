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

    /* write header */
    char chAlias[4] = { 'A', 'B', 'C', 'D' };
    fout << "Frequency,";
    for (int ch = 0; ch < params.numChannels; ch++)
    {
        char buffer[100];
        sprintf_s(buffer, 100, "Ch%c Mag (V),Ch%c Phase (degrees),Ch%c Bias(V),Ch%c THD(percent),", chAlias[ch], chAlias[ch], chAlias[ch], chAlias[ch]);
        fout << buffer;
    }
    fout << '\n';

    /* Write body */
    for (int i = 0; i < exp.getRawData()[0].size(); i++)
    {
        fout << exp.getRawData()[0][i].frequency;
        for (int ch = 0; ch < params.numChannels; ch++)
        {
            fout << ',' << exp.getRawData()[ch][i].Mag << ',' << exp.getRawData()[ch][i].Phase << ',' << exp.getRawData()[ch][i].Bias << ',' << exp.getRawData()[ch][i].THD;
        }
        fout << '\n';
    }
    fout.close();
    
exit:
    {
        system("pause");
        return 0;
    }
}