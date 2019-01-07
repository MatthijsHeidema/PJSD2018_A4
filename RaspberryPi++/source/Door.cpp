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
	// Get the values
	file->updateDoc();
	const char* doorStatus = file->getStringValue("doorStatus");
	usleep(100000);
	// Sync the values
	// Read
	const char* redButton = comm->receiveValue("redButton");
	cout << redButton << endl;
	const char* redLight = redButton;
	// Write
	comm->sendValue("doorStatus", doorStatus);
	comm->sendValue("redLight", redLight);
	// Save new values
	file->updateFile();
}

