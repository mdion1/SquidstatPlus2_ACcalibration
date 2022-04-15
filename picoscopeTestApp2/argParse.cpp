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
            double freq;
            if (parseDouble(str, &freq)) {
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
    else if (compareStr("ACamp", argname)) {
        double amp;
        if (parseDouble(argval, &amp)) {
            params.ACamp = amp;
        }
    }
    else if (compareStr("numChannels", argname)) {
        int numChannels;
        if (parseInt(argval, &numChannels)) {
            params.numChannels = numChannels;
        }
    }
    else if (compareStr("outputFile", argname)) {
        params.outputFileName = argval;
    }
    else if (compareStr("DCbias", argname)) {
        double dcbias;
        if (parseDouble(argval, &dcbias)) {
            params.DCbias = dcbias;
        }
    }
}

bool argParse::parseInt(string str, int* int_out)
{
    if (nullptr == int_out) {
        return false;
    }
    int result = atoi(str.c_str());
    if (0 == result) {
        return false;
    }
    else {
        *int_out = result;
        return true;
    }
}

bool argParse::parseDouble(string str, double* double_out)
{
    if (nullptr == double_out) {
        return false;
    }
    int result = atof(str.c_str());
    if (0 == result) {
        return false;
    }
    else {
        *double_out = result;
        return true;
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
    for (int i = 0; i < str.length(); i++)
    {
        int findIdx_start = str.find(sep, i);
        if (-1 == findIdx_start) {
            break;
        }
        else {
            int findIdx_end = str.find(sep, i);
            if (-1 == findIdx_end) {
                findIdx_end = str.length();
            }
            strList.push_back(str.substr(findIdx_start, findIdx_end - findIdx_start));
        }
    }
    return strList;
}