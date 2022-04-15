#pragma once

#include "ps5000aApi.h"
#include <vector>
#include <fstream>

using namespace std;

typedef enum
{
    PROBE_DIV_1X, PROBE_DIV_10X
}probeDiv_t;

typedef struct
{
    PS5000A_RANGE range;
    PS5000A_COUPLING coupling;
    probeDiv_t div;
}probeParams_t;

typedef struct
{
    probeParams_t probe[4];
    vector<double> freqList;
    double ACamp;
    int numChannels;
    string outputFileName;
    double DCbias;
}inputParams_t;

class argParse
{
public:
    argParse(string paramfilename);
    inputParams_t getParams() {
        return params;
    }

private:
    inputParams_t getDefaultParams();
    void parseArgPair(string argName, string argval);
    void parseFreqList(string argname);
    bool parseProbeCouplingStr(string str, PS5000A_COUPLING* coupling_out);
    bool parseProbeDivStr(string str, probeDiv_t* div_out);
    bool parseProbeRangeStr(string str, PS5000A_RANGE* range_out);
    char toLower(char upper);
    string toLower(const string& str);
    bool compareStr(const string& a, const string& b);
    vector<string> splitStr(const string& str, char sep);

    inputParams_t params;
    ifstream file;
};

