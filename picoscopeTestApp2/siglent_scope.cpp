#include "stdafx.h"
#include "siglent_scope.h"

/* sample code for connecting to the scope thru VISA */
//void testFn()
//{
//	ViSession defaultRM;
//	ViSession instr;
//	ViUInt32 numInstrs;
//	ViFindList findList;
//	ViUInt32 retCount;
//	ViUInt32 writeCount;
//	ViStatus status;////	char instrResourceString[VI_FIND_BUFLEN];
//	unsigned char buffer[100];
//	char stringinput[512];
//	int i;
//
//	/** First we must call viOpenDefaultRM to get the manager
//	* handle. We will store this handle in defaultRM.*/
//	status = viOpenDefaultRM(&defaultRM);
//	if (status < VI_SUCCESS)
//	{
//		cout << "Error opening session to the VISA resource manager. Status code = " << status << "\n";
//	}//
//	/* Find all the USB TMC VISA resources in our system and store the number of
//	resources in the system in numInstrs. */
//	status = viFindRsrc(defaultRM, "USB?*INSTR", &findList, &numInstrs, instrResourceString);
//	if (status < VI_SUCCESS)
//	{
//		cout << "An error occurred while finding resources. Status code = " << status << "\n";
//	}
//	
//	/** Now we will open VISA sessions to all USB TMC instruments.
//	* We must use the handle from viOpenDefaultRM and we must
//	* also use a string that indicates which instrument to open. This
//	* is called the instrument descriptor. The format for this string
//	* can be found in the function panel by right clicking on the
//	* descriptor parameter. After opening a session to the
//	* device, we will get a handle to the instrument which we
//	* will use in later VISA functions. The AccessMode and Timeout
//	* parameters in this function are reserved for future
//	* functionality. These two parameters are given the value VI_NULL.*/
//	for (i = 0; i<numInstrs; i++)
//	{
//		if (i > 0)
//		{
//			viFindNext(findList, instrResourceString);
//		}
//		status = viOpen(defaultRM, instrResourceString, VI_NULL, VI_NULL, &instr);
//		if (status < VI_SUCCESS)
//		{
//			cout << "Cannot open a session to the device" << (i + 1) << "\n";
//			continue;
//		}
//		/* * At this point we now have a session open to the USB TMC instrument.
//		* We will now use the viPrintf function to send the device the string "*IDN?\n",
//		* asking for the device's identification. */
//		const char * cmmand = "*IDN?\n";
//		status = viPrintf(instr, cmmand);
//		if (status < VI_SUCCESS)
//		{
//			printf("Error writing to the device %d.\n", i + 1);
//			status = viClose(instr);
//			continue;
//		}
//		/** Now we will attempt to read back a response from the device to
//		* the identification query that was sent. We will use the viScanf
//		* function to acquire the data.
//		* After the data has been read the response is displayed.*/
//		status = viScanf(instr, "%t", buffer);
//		if (status < VI_SUCCESS)
//		{
//			printf("Error reading a response from the device %d.\n", i + 1);
//		}
//		else
//		{
//			printf("\nDevice %d: %*s\n", i + 1, retCount, buffer);
//		}
//		status = viClose(instr);
//	}
//	/** Now we will close the session to the instrument using
//	* viClose. This operation frees all system resources. */
//	status = viClose(defaultRM);
//}

void siglent_scope::open()
{
	ViUInt32 numInstrs;
	ViFindList findList;
	ViUInt32 retCount;
	ViUInt32 writeCount;
	ViStatus status;	char instrResourceString[VI_FIND_BUFLEN];
	unsigned char buffer[100];
	char stringinput[512];
	int i;

	/* Call viOpenDefaultRM to get the manager handle */
	status = viOpenDefaultRM(&defaultRM);																//http://zone.ni.com/reference/en-XX/help/370131S-01/ni-visa/viopendefaultrm/
	if (status < VI_SUCCESS)
		cout << "Error opening session to the VISA resource manager. Status code = " << status << "\n";
	/* Find all the USB TMC VISA resources in our system and store the number of
	resources in the system in numInstrs. */
	status = viFindRsrc(defaultRM, "USB?*INSTR", &findList, &numInstrs, instrResourceString);			//http://zone.ni.com/reference/en-XX/help/370131S-01/ni-visa/vifindrsrc/
	if (status < VI_SUCCESS)
	{
		cout << "An error occurred while finding resources. Status code = " << status << "\n";
		return;
	}
	if (numInstrs == 0)
	{
		cout << "No instruments found\n";
		return;
	}
	if (numInstrs > 1)
	{
		cout << "More than one instrument found, we didn't write code to handle this...";
		return;
	}

	/* Assuming only one scope is attached, and it is first on the list, open the instrument and query "*IDN?" to get device num */
	status = viOpen(defaultRM, instrResourceString, VI_NULL, VI_NULL, &instr);					//http://zone.ni.com/reference/en-XX/help/370131S-01/ni-visa/viopen/
	if (status < VI_SUCCESS)
	{
		cout << "Cannot open a session to the device\n";
		return;
	}

	/* Query the device for the identification number, print the response */
	writeToScope("*IDN?");
	cout << readFromScope();
}

void siglent_scope::close()
{
	viClose(instr);
	viClose(defaultRM);
}

void siglent_scope::configureChannel(int channelNum, PS5000A_RANGE range)
{
	//Example string: "C1:VDIV 50mV";

	string str_cmd, str_channel, str_range;
	switch (channelNum)
	{
	case 1:
		str_channel.assign("C1");
		break;
	case 2:
		str_channel.assign("C2");
		break;
	default:
		cout << "Channel not supported";
		return;
	}

	switch (range)
	{
	case PS5000A_10MV:
		str_range.assign("10mV");
		break;
	case PS5000A_20MV:
		str_range.assign("20mV");
		break;
	case PS5000A_50MV:
		str_range.assign("50mV");
		break;
	case PS5000A_100MV:
		str_range.assign("100mV");
		break;
	case PS5000A_200MV:
		str_range.assign("200mV");
		break;
	case PS5000A_500MV:
		str_range.assign("500mV");
		break;
	case PS5000A_1V:
		str_range.assign("1V");
		break;
	case PS5000A_2V:
		str_range.assign("2V");
		break;
	case PS5000A_5V:
		str_range.assign("5V");
		break;
	case PS5000A_10V:
		str_range.assign("10V");
		break;
	case PS5000A_20V:
		str_range.assign("20V");
		break;
	case PS5000A_50V:
		str_range.assign("50V");
		break;
	default:
		cout << "Range not supported";
		return;
	}

	str_cmd.append(str_channel);
	str_cmd.append(":VDIV ");
	str_cmd.append(str_range);
	writeToScope(str_cmd);
}

void siglent_scope::turnOnSignalGen(double frequency, double amplitude)
{
	writeToScope(string("WAVEGENERATOR FREQ,") + to_string(frequency));
	writeToScope(string("WAVEGENERATOR AMPL,") + to_string(amplitude));
	
	static bool once = true;
	if (once)
	{
		writeToScope("WAVEGENERATOR OUTP,ON");
		writeToScope("WAVEGENERATOR WVTP,SINE");
		writeToScope("WAVEGENERATOR LOAD,50");			//options are "50" (50 ohm load) or "HZ" (hi-z)
	}
}

void siglent_scope::getData_2ch(scopeTimebase_t timebase, uint32_t * numPoints, vector<int16_t> &chA_data, vector<int16_t> &chB_data)
{
	//7M points per channel, 500MSa/s max
	//14 divisions/screen
	//use SARA? query, then divide into correct num points

	writeToScope("SARA?");
	readFromScope();





	//uint32_t numPoints = *pNumPoints;
	//ps5000aRunBlock(_InstrHandle, 0, numPoints, (uint32_t)timebase, NULL, 0, NULL, NULL);
	//int16_t ready = 0;
	//while (!ready)
	//{
	//	!ps5000aIsReady(_InstrHandle, &ready);
	//}
	//ps5000aStop(_InstrHandle);

	//// 5) Get data
	//int16_t * channelA_data = new int16_t[numPoints];
	//int16_t * channelB_data = new int16_t[numPoints];
	//int16_t overflow;
	//ps5000aSetDataBuffer(_InstrHandle, PS5000A_CHANNEL_A, channelA_data, numPoints, 0, PS5000A_RATIO_MODE_NONE);
	//ps5000aSetDataBuffer(_InstrHandle, PS5000A_CHANNEL_B, channelB_data, numPoints, 0, PS5000A_RATIO_MODE_NONE);
	//ps5000aGetValues(_InstrHandle, 0, pNumPoints, 1, PS5000A_RATIO_MODE_NONE, 0, &overflow);
	//numPoints = *pNumPoints;
	//chA_data.assign(channelA_data, channelA_data + numPoints);
	//chB_data.assign(channelB_data, channelB_data + numPoints);

	//delete[] channelA_data;
	//delete[] channelB_data;
}

double siglent_scope::getTimebase(scopeTimebase_t timebase)
{
	return ((double)timebase) * 2.e-9;
}

void siglent_scope::writeToScope(string msg)
{
	writeToScope((ViConstBuf)msg.data(), msg.length());
}

void siglent_scope::writeToScope(const char * msg)
{
	/* find the "/0" string terminator in order to find the string length */
	int terminator_index = 0;
	while (msg[terminator_index])
		terminator_index++;

	writeToScope((ViConstBuf)msg, terminator_index);
}

void siglent_scope::writeToScope(ViConstBuf msg, int len)
{
	ViStatus status;
	ViConstBuf buf;
	ViUInt32 bytesWritten;
	status = viWrite(instr, msg, len, &bytesWritten);															//http://zone.ni.com/reference/en-XX/help/370131S-01/ni-visa/viwrite/
	if (status < VI_SUCCESS)
	{
		cout << "Error writing to the device\n";
		status = viClose(instr);
	}
}

string siglent_scope::readFromScope(uint32_t maxLength)
{
	ViStatus status;
	ViUInt32 bytesRead;
	ViByte * buf = new ViByte[maxLength];
	status = viRead(instr, buf, maxLength, &bytesRead);														//http://zone.ni.com/reference/en-XX/help/370131S-01/ni-visa/viread/
	if (status < VI_SUCCESS)
	{
		cout << "Error reading a response from the device\n";
	}
	
	string ret;
	ret.assign((char*)buf, (size_t)bytesRead);
	delete[] buf;
	return ret;
}


//bool isOperationComplete()
//{
//	const char * cmmand = "*OPC?\n";
//	status = viPrintf(instr, cmmand);
//
//	status = viScanf(instr, "%t", buffer);
//	if (buffer[0] == '1')
//		return true;
//	else
//		return false;
//}
//
//void reset()
//{
//	const char * cmmand = "*RST\n";
//	status = viPrintf(instr, cmmand);
//
//	status = viScanf(instr, "%t", buffer);
//	if (buffer[0] == '1')
//		return true;
//	else
//		return false;
//}


void aqcuire()
{
	// "TIME_DIV {1 NS, 2NS, 5NS, 10NS ... where you can use NS, US, MS, and S}
	//"WFSU SP,3"

	// "ACQUIRE_WAY SAMPLING"		//is this the default anyway?

	// "MEMORY_SIZE {7K, 70K, 700K, 7M}"		set memory size
	// "SAST?"									query data acquisition status
	
	// "C1:WF? DAT2"			calls for all data from channel 1
	// response will be "C1:WF ALL,#9000700000.......0A 0A" where 000700000 indicates that the next 700,000 bytes are data, and two bytes "0A 0A" terminate the transmission
	// raw data needs to be converted to signed 8-bit ints: if value > 127, subtract 255 (not 256, 0xff and 0x00 are equivalent)
	// voltage = rawValue * (vdiv / 25) - voffset
	
	// use "SARA?" query to get samples/second			//see manual to determine how sampling rate and time_div are related


}