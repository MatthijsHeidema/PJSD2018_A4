/*
 * Stoel.cpp
 *
 *  Created on: Jan 7, 2019
 *      Author: programmer
 */

#include "Stoel.h"
#include <ctime>

void Stoel::sync() {

	file->updateDoc();
	const char* pressureSensor = comm->receiveValue("PressureSensor");
	if(pressureSensorLogic(pressureSensor))
	{
		file->edit("EpilepsieAanvalStoel","1");
	}
	file->updateFile();
}

