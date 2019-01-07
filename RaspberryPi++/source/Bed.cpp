/*
 * Bed.cpp
 *
 *  Created on: Dec 10, 2018
 *      Author: matthijs
 */

#include "Bed.h"
#include <ctime>

void Bed::sync() {


	file->updateDoc();

	const char* bedLightToggle = comm->receiveValue("BedLight");
	cout << "Licht toggle: " << bedLightToggle << endl;
	const char* bedLightStatus = file->getStringValue("BedLight");
	cout << "Licht status: " << bedLightStatus << endl;


	if(!strcmp(bedLightToggle,"1"))
	{
		//cout << "Light toggled" << endl;
		if(!strcmp(bedLightStatus,"1"))
		{
			comm->sendValue("BedLight","0");
			file->edit("BedLight","0");
			bedLightToggle = "0";
		}
		if(!strcmp(bedLightStatus,"0"))
		{
			comm->sendValue("BedLight","1");
			time(&timeLightOn);
			file->edit("BedLight","1");
			bedLightToggle = "0";
		}
	}

	if(nighttimeCheck())
	{
		automaticLightOff(5);
	}


	const char* pressureSensor = comm->receiveValue("PressureSensor");
	cout << pressureSensor << endl;

	if(seizureDetection(pressureSensor)){

	}

	file->updateFile();

}

bool Bed::seizureDetection(const char* value) {
	int sensorValue = atoi(value);

	if(sensorValue > 500 && bedUpdate)
	{
		inBed = true;
		bedUpdate = false;
		movementCounter++;

		if(intervalStart)
		{
			time(&intervalStartTime);
			intervalStart = false;
		}
	}
	else if(sensorValue < 500)
	{
		inBed = false;
		bedUpdate = true;
	}

	if(difftime(time(0),intervalStartTime) > 8000)
	{

	}


	return false;
}

void Bed::automaticLightOff(int timeUntilOff) {

	const char* bedLightStatus = file->getStringValue("BedLight");

	if(difftime(time(0),timeLightOn) > timeUntilOff && !strcmp(bedLightStatus,"1"))
	{
		comm->sendValue("BedLight","0");
		file->edit("BedLight","0");
	}
}
