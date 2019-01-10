/*
 * Window.cpp
 *
 *  Created on: Jan 10, 2019
 *      Author: programmer
 */


#include "Window.h"
using namespace std;

void Window::sync()
{
	// Get values from file
	file->updateDoc();
	const char* windowStatus = file->getStringValue("windowStatus");
	const char* ledBrightness = file->getStringValue("ledBrightness");
	// Read values from Wemos
	const char* licht0 = comm->receiveValue("analog0");
	const char* licht1 = comm->receiveValue("analog0");
	// Write values to Wemos
	comm->sendValue("windowStatus", windowStatus);
	comm->sendValue("ledBrightness", ledBrightness);
	// Update the file
	file->updateFile();
}
