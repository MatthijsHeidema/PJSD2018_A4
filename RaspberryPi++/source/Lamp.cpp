/*
 * Lamp.cpp
 *
 *  Created on: Dec 11, 2018
 *      Author: matthijs
 */

#include "Lamp.h"
#include <unistd.h>

void Lamp::sync() {

	file->updateDoc();

	const char* toggleLed = file->getStringValue("ToggleLed");
	const char* color = file->getStringValue("Color");
	comm->sendValue("ToggleLed", toggleLed);
	usleep(1000000);
	cout << "ToggleLed" << toggleLed << endl;
	comm->sendValue("Color", color);
	usleep(1000000);
	cout << "Color" << color << endl;
	const char* PIR = comm->receiveValue("PIR");
	usleep(1000000);
	cout << "PIR: " << PIR << endl;
	file->edit("PIR", PIR);

	//comm->disconnectFromServer();
	file->updateFile();
}
