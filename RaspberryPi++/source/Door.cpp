/*
 * Door.cpp
 *
 *  Created on: Jan 7, 2019
 *      Author: Mathieu Gatsonides
 */

#include "Door.h"
#include <unistd.h>

using namespace std;

void Door::sync()
{
	const char* redLight;
	// Get the values
	file->updateDoc();
	const char* doorStatus = file->getStringValue("doorStatus");		// Read door open/close from webpage (JSON FILE)
	// Sync the values
		// Read
	const char* redButton = comm->receiveValue("redButton");
	//cout << redButton << endl;
	if(!strcmp(redButton,"2"))	// When received button is pressed, only button 2 is used.
	{
		redLight = "1";			// Turn on the led
	}
	else
	{
		redLight = "0";			// Turn off the led
	}
		// Write
	comm->sendValue("doorStatus", doorStatus);							// Sending that open/close signal to the wemos
	comm->sendValue("redLight", redLight);				// Send the wemos to turn on/off doorLED
	// Save new values
	file->updateFile();
}

