/*
 * Lamp.cpp
 *
 *  Created on: Dec 11, 2018
 *      Author: matthijs
 */

#include "Lamp.h"

void Lamp::sync() {

	file->updateDoc();
	comm->connectToServer();

	const char* toggleLed = file->getStringValue("ToggleLed");
	const char* color = file->getStringValue("Color");
	comm->sendValue("ToggleLed", toggleLed);
	comm->sendValue("Color", color);

	comm->disconnectFromServer();
	file->updateFile();
}
