#include "stdafx.h"
#include "argParse.h"

#include <cstdint>
#include <string>
#include <algorithm>

inputParams_t argParse::getDefaultParams()
{
    inputParams_t defaultParams;
    defaultParams.ACamp = 0;
    defaultParams.DCbias = 0;
    defaultParams.outputFileName = "results.csv";
    for (int i = 0; i < 4; i++) {
        defaultParams.probe[i].coupling = PS5000A_DC;
        defaultParams.probe[i].range = PS5000A_10MV;
        defaultParams.probe[i].div = PROBE_DIV_1X;
    }
    defaultParams.numChannels = 2;
    return defaultParams;
}

argParse::argParse(string paramfilename)
{
    params = getDefaultParams();
    file.open(paramfilename);
    while (!file.eof())
    {
        string str;
        getline(file, str);
        vector<string> line = argParse::splitStr(str, ',');

        if (line.size() == 2)
        {
            parseArgPair(line[0], line[1]);
        }
        else if (line.size() == 1) {
            /* Read out frequency list */
            parseFreqList(line[0]);
        }
    }
}

void argParse::parseFreqList(string argname)
{
    if (compareStr("frequencyList", argname))
    {
        while (!file.eof()) {
            string str;
            getline(file, str);
            double freq = atof(str.c_str());
            if (freq > 0) {
                params.freqList.push_back(freq);
            }
            else {
                file.seekg(ios_base::cur, -((int)str.length())); // re-parse the item as an arg name the next time through the loop
                break;
            }
        }
    }
}

void argParse::parseArgPair(string argname, string argval)
{
    if (compareStr("ProbeARange", argname)) {
        PS5000A_RANGE range;
        if (parseProbeRangeStr(argval, &range)) {
            params.probe[0].range = range;
        }
    }
    else if (compareStr("ProbeBRange", argname)) {
        PS5000A_RANGE range;
        if (parseProbeRangeStr(argval, &range)) {
            params.probe[1].range = range;
        }
    }
    else if (compareStr("ProbeCRange", argname)) {
        PS5000A_RANGE range;
        if (parseProbeRangeStr(argval, &range)) {
            params.probe[2].range = range;
        }
    }
    else if (compareStr("ProbeDRange", argname)) {
        PS5000A_RANGE range;
        if (parseProbeRangeStr(argval, &range)) {
            params.probe[3].range = range;
        }
    }
    else if (compareStr("ProbeADiv", argname)) {
        probeDiv_t div;
        if (parseProbeDivStr(argval, &div)) {
            params.probe[0].div = div;
        }
    }
    else if (compareStr("ProbeBDiv", argname)) {
        probeDiv_t div;
        if (parseProbeDivStr(argval, &div)) {
            params.probe[1].div = div;
        }
    }
    else if (compareStr("ProbeCDiv", argname)) {
        probeDiv_t div;
        if (parseProbeDivStr(argval, &div)) {
            params.probe[2].div = div;
        }
    }
    else if (compareStr("ProbeDDiv", argname)) {
        probeDiv_t div;
        if (parseProbeDivStr(argval, &div)) {
            params.probe[3].div = div;
        }
    }
    else if (compareStr("ProbeAcoupling", argname)) {
        PS5000A_COUPLING coupling;
        if (parseProbeCouplingStr(argval, &coupling)) {
            params.probe[0].coupling = coupling;
        }
    }
    else if (compareStr("ProbeBcoupling", argname)) {
        PS5000A_COUPLING coupling;
        if (parseProbeCouplingStr(argval, &coupling)) {
            params.probe[1].coupling = coupling;
        }
    }
    else if (compareStr("ProbeCcoupling", argname)) {
        PS5000A_COUPLING coupling;
        if (parseProbeCouplingStr(argval, &coupling)) {
            params.probe[2].coupling = coupling;
        }
    }
    else if (compareStr("ProbeDcoupling", argname)) {
        PS5000A_COUPLING coupling;
        if (parseProbeCouplingStr(argval, &coupling)) {
            params.probe[3].coupling = coupling;
        }
    }
    else if (compareStr("ACamp", argname)) {
        params.ACamp = atof(argval.c_str());
    }
    else if (compareStr("numChannels", argname)) {
        params.numChannels = atoi(argval.c_str());
    }
    else if (compareStr("outputFile", argname)) {
        params.outputFileName = argval;
    }
    else if (compareStr("DCbias", argname)) {
        params.DCbias = atof(argval.c_str());
    }
}

bool argParse::parseProbeCouplingStr(string str, PS5000A_COUPLING* coupling_out)
{

    if (nullptr == coupling_out) {
        return false;
    }
    else if (compareStr(str, "DC")) {
        *coupling_out = PS5000A_DC;
        return true;
    }
    else if (compareStr(str, "AC")) {
        *coupling_out = PS5000A_AC;
        return true;
    }
    else {
        return false;
    }
}

bool argParse::parseProbeDivStr(string str, probeDiv_t* div_out)
{
    if (nullptr == div_out) {
        return false;
    }
    else if (compareStr(str, "1x")) {
        *div_out = PROBE_DIV_1X;
        return true;
    }
    else if (compareStr(str, "10x")) {
        *div_out = PROBE_DIV_10X;
        return true;
    }
}

bool argParse::parseProbeRangeStr(string str, PS5000A_RANGE* range_out)
{
    if (nullptr == range_out) {
        return false;
    }
    else if (compareStr(str, "10mV")) {
        *range_out = PS5000A_10MV;
        return true;
    }
    else if (compareStr(str, "20mV")) {
        *range_out = PS5000A_20MV;
        return true;
    }
    else if (compareStr(str, "50mV")) {
        *range_out = PS5000A_50MV;
        return true;
    }
    else if (compareStr(str, "100mV")) {
        *range_out = PS5000A_100MV;
        return true;
    }
    else if (compareStr(str, "200mV")) {
        *range_out = PS5000A_200MV;
        return true;
    }
    else if (compareStr(str, "500mV")) {
        *range_out = PS5000A_500MV;
        return true;
    }
    else if (compareStr(str, "1V")) {
        *range_out = PS5000A_1V;
        return true;
    }
    else if (compareStr(str, "2V")) {
        *range_out = PS5000A_2V;
        return true;
    }
    else if (compareStr(str, "5V")) {
        *range_out = PS5000A_5V;
        return true;
    }
    else if (compareStr(str, "10V")) {
        *range_out = PS5000A_10V;
        return true;
    }
    else if (compareStr(str, "20V")) {
        *range_out = PS5000A_20V;
        return true;
    }
    else if (compareStr(str, "50V")) {
        *range_out = PS5000A_50V;
        return true;
    }
    else
        return false;
}

char argParse::toLower(char upper)
{
    if (upper >= 'A' && upper <= 'Z') {
        return upper + 0x20;
    }
    return upper;
}

string argParse::toLower(const string& str)
{
    string strcpy = string(str);
    for (int i = 0; i < strcpy.length(); i++)
    {
        strcpy[i] = toLower(strcpy[i]);
    }
    return strcpy;
}

bool argParse::compareStr(const string& a, const string& b)
{
    string a_lower = toLower(a);
    string b_lower = toLower(b);
    return 0 == a_lower.compare(b_lower);
}

vector<string> argParse::splitStr(const string& str, char sep)
{
    vector<string> strList;
    for (int idxNext = 0; idxNext < str.length();)
    {
        int idxStart = idxNext;
        idxNext = str.find(sep, idxStart + 1);
        if (-1 == idxNext) {
            idxNext = str.size();
        }
        strList.push_back(str.substr(idxStart, idxNext - idxStart));
        idxNext++;  //get rid of the leading sep character
    }
    return strList;
}