#pragma once
#include <iostream>
#include <string>
#include <thread>
#include <mutex>  
using namespace std;

class InputCheckerAsync
{
public:
	InputCheckerAsync();
	~InputCheckerAsync();
	char getKeyPressed();

private:
	void Poll_cin();
	char _keyPressed;
	std::mutex _keyPressedMutex;
	thread * cinPollThread;
	void setKeyPressed(char a);
};