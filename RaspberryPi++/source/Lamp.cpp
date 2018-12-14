/*
 * Lamp.cpp
 *
 *  Created on: Dec 11, 2018
 *      Author: matthijs
 */

#include "Lamp.h"

void Lamp::sync() {

	file->updateDoc();

	const char* toggleLed = file->getStringValue("ToggleLed");
	const char* color = file->getStringValue("Color");
	comm->sendValue("ToggleLed", toggleLed);
	cout << "ToggleLed" << toggleLed << endl;
	comm->sendValue("Color", color);
	cout << "Color" << color << endl;

	//comm->disconnectFromServer();
	file->updateFile();
}
