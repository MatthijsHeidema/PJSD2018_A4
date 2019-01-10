/*
 * Door.cpp
 *
 *  Created on: Jan 7, 2019
 *      Author: Mathieu
 */

#include "Door.h"
#include <unistd.h>

using namespace std;

void Door::sync()
{
	const char* redLight;
	// Get the values
	file->updateDoc();
	const char* doorStatus = file->getStringValue("doorStatus");
	//(100000);
	// Sync the values
	// Read
	const char* redButton = comm->receiveValue("redButton");
	cout << redButton << endl;
	if(!strcmp(redButton,"2"))
	{
		redLight = "1";
	}
	else
	{
		redLight = "0";
	}
	// Write
	comm->sendValue("doorStatus", doorStatus);
	comm->sendValue("redLight", redLight);
	// Save new values
	file->updateFile();
}

