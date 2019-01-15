/*
 * Window.cpp
 *
 *  Created on: Jan 10, 2019
 *      Author: Mathieu Gatsonides
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
	//cout << "lichtSensorWaarde = " << lichtSensor << endl;
	// Mode
	if (!nighttimeCheck())	// When it is day time
	{
		if (!strcmp(aangepast, "1"))
		{
			mode = 1;										// 1 = Manual mode
			aangepast = "0";
			// aangepast json writing
			time(&timeChanged);								// Set the time changed to NOW
			//cout << "Dag" << endl;						//Daytime
		} else if (difftime(time(0), timeChanged) > 4)		// Return to manual mode when 4 seconds without playing have passed
		{
			mode = 0;										// 0 = automatic mode
			//cout << "Auto mode" << endl;
		}
	}
	else	// When it is night time
	{
		mode = 0;											// 0 = automatic mode
		//cout << "het is zogenaamd nacht!" << endl;
	}
	//cout << "de modus is " << mode << endl;
	setWindow(lichtSensor);									// Set window to appropriate brightness

	// Write values to Wemos
	//comm->sendValue("windowStatus", windowStatus);
	//comm->sendValue("ledBrightness", ledBrightness);
	// Update the file
	file->edit("aangepast", aangepast);
	file->updateFile();
}

void Window::setWindow(int lichtSensor)						// Function that checks the mode and makes the decision what to do with the LED's
{
	string JA = "1";			// There were some problems with just using "1" and "0"
	string NEE = "0";

	string windowStatus;
	if(mode == 0)				// When the mode is automatic
	{
		if (nighttimeCheck())	// When it is night
		{
			windowStatus = JA;							// Turn on dimming
			comm->sendValue("ledBrightness", "0");
		} else if (lichtSensor > 600)
		{
			windowStatus = JA;							// Turn on dimming
			comm->sendValue("ledBrightness", "100");
		} else
		{
			windowStatus = NEE;							// Turn off dimming
			comm->sendValue("ledBrightness", "50");
		}
	} else						// When in manual mode take the value from the webserver (JSON FILE)
	{
		windowStatus = file->getStringValue("windowStatus");
		//cout << "handmodus windowStatus = " << windowStatus << endl;
	}

	file->edit("windowStatus", windowStatus);			// Edit the window status
	//cout << "dit staat in de windowStatus: " << file->getStringValue("windowStatus");
	file->updateFile();									// Update the file
	comm->sendValue("windowStatus", windowStatus);		// Send the decided windowValue to the Arduino
}
