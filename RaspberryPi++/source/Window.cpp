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
	const char* aangepast = file->getStringValue("aangepast");
	//string ledBrightness = file->getStringValue("ledBrightness");

	// Read values from Wemos
	int lichtSensor = atoi(comm->receiveValue("analog0"));
	cout << "lichtSensorWaarde = " << lichtSensor << endl;
	// Mode
	if (!nighttimeCheck())	// Als het dag is
	{
		if (!strcmp(aangepast, "1"))
		{
			mode = 1;										// 1 = handmatige modus
			aangepast = "0";
			//aangepast json schrijven
			time(&timeChanged);
			cout << "Dag" << endl;
		} else if (difftime(time(0), timeChanged) > 4)
		{
			mode = 0;									// 0 = auto mode
			cout << "Auto mode" << endl;
		}
	}
	else	// Als het nacht is
	{
		mode = 0;
		cout << "het is zogenaamd nacht!" << endl;
	}
	cout << "de modus is " << mode << endl;
	setWindow(lichtSensor);

	// Write values to Wemos
	//comm->sendValue("windowStatus", windowStatus);
	//comm->sendValue("ledBrightness", ledBrightness);
	// Update the file
	file->edit("aangepast", aangepast);
	file->updateFile();
}

void Window::setWindow(int lichtSensor)
{
	string JA = "1";
	string NEE = "0";

	string windowStatus;
	if(mode == 0)		// Als automatische mode
	{
		if (nighttimeCheck())
		{
			windowStatus = JA;
			comm->sendValue("ledBrightness", "0");
		} else if (lichtSensor > 600)
		{
			windowStatus = JA;
			comm->sendValue("ledBrightness", "100");
		} else
		{
			windowStatus = NEE;
			comm->sendValue("ledBrightness", "50");
		}
	} else
	{
		windowStatus = file->getStringValue("windowStatus");
		cout << "handmodus windowStatus = " << windowStatus << endl;
	}

	file->edit("windowStatus", windowStatus);
	cout << "dit staat in de windowStatus: " << file->getStringValue("windowStatus");
	file->updateFile();
	comm->sendValue("windowStatus", windowStatus);
}
