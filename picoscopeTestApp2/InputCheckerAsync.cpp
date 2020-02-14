#include "stdafx.h"
#include "InputCheckerAsync.h"

InputCheckerAsync::InputCheckerAsync()
{
	cinPollThread = new thread(&InputCheckerAsync::Poll_cin, this);
	_keyPressed = (char)0;
}

InputCheckerAsync::~InputCheckerAsync()
{
	cinPollThread->join();
	delete cinPollThread;
}

void InputCheckerAsync::Poll_cin()
{
	string inputStr;
	std::cin >> inputStr;
	setKeyPressed(inputStr[0]);
}

void InputCheckerAsync::setKeyPressed(char a)
{
	_keyPressedMutex.lock();
	_keyPressed = a;
	_keyPressedMutex.unlock();
}

char InputCheckerAsync::getKeyPressed()
{
	char ret = false;
	_keyPressedMutex.lock();
	ret = _keyPressed;
	_keyPressed = (char)0;		//flag is cleared after each read
	_keyPressedMutex.unlock();
	return ret;
}